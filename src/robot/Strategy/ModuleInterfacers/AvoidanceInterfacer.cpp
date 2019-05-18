//
// Created by terae on 23/01/19.
//

#include "AvoidanceInterfacer.h"
#include "../../../Lidar/FindRobots.h"

namespace Strategy {
	namespace Interfacer {
		AvoidanceInterfacer::AvoidanceInterfacer(std::shared_ptr<PhysicalRobot::Robot> robot, Environment& env, Vector2m turret_shift)
		        : _is_running(true)
		        , _robot(robot)
		        , _env(env)
		        , _turret_shift(turret_shift)
		        , _angle_detection_adversary(GLOBAL_CONSTANTS()[_robot->name].get_angle_adversary_detection()) {
			_thread_lidar = std::thread(std::bind(&AvoidanceInterfacer::thread_lidar_main, this));
			set_turret_shift(GLOBAL_CONSTANTS()[_robot->name].get_turret_position());
		}

		AvoidanceInterfacer::~AvoidanceInterfacer() {
			_is_running = false;
			_thread_lidar.join();
		}

		void AvoidanceInterfacer::set_turret_shift(Vector2m shift) {
			_turret_shift = shift;
		}

		std::vector<repere::Position> AvoidanceInterfacer::get_adversary_positions() const {
			std::lock_guard<std::mutex> lk(_mutex_adversary);
			if(!_fake_positions.empty()) {
				auto vec = _fake_positions;
				vec.insert(vec.end(), _adversary_positions.begin(), _adversary_positions.end());
				return vec;
			} else {
				return _adversary_positions;
			}
		}

		void AvoidanceInterfacer::add_fake_adversary_position(const repere::Position& position) {
			_fake_positions.emplace_back(position);
		}

		bool AvoidanceInterfacer::adversary_detected(Distance threshold) const {
			repere::Position robot_position = get_robot_position();
			auto adversaries = get_adversary_positions();

			for(repere::Position pos : adversaries) {
				if((robot_position.getPos2D() - pos.getPos2D()).norm() < threshold) {
					return true;
				}
			}
			return false;
		}

		bool AvoidanceInterfacer::adversary_detected(PhysicalRobot::SensAdvance sens) const {
			return adversary_detected(GLOBAL_CONSTANTS().get_threshold_adversary_detection(), sens);
		}

		bool AvoidanceInterfacer::adversary_detected(Distance threshold, PhysicalRobot::SensAdvance sens) const {
			repere::Position robot_position = get_robot_position();
			repere::Orientation robot_orientation = get_robot_orientation();
			auto adversaries = get_adversary_positions();

			for(const repere::Position& pos : adversaries) {
				Vector2m diff = pos.getPos2D() - robot_position.getPos2D();
				bool inside_radius = diff.norm() < threshold;
				Angle angle = atan2(diff.y, diff.x);

				Angle moving_direction = sens == PhysicalRobot::SensAdvance::Forward ? robot_orientation.getAngle() :
				                                                                       robot_orientation.getAngle() + 1_PI;
				Angle angle_diff = (angle - moving_direction).toMinusPiPi();

				bool in_front_of = angle_diff < _angle_detection_adversary && -angle_diff < _angle_detection_adversary;

				if(inside_radius && in_front_of) {
					return true;
				}
			}
			return false;
		}

		std::vector<int> AvoidanceInterfacer::update_environment(Environment& env) const {
			std::vector<int> result;
			const int first_dynamic = 10;
			int last_dynamic = 0;

			const Distance adv_radius = 0.80f * env.getRobotRadius();

			_mutex_adversary.lock();
			for(const repere::Position& pos : _adversary_positions) {
				int id = first_dynamic + last_dynamic++;
				env.addDynamicShape(id, std::make_unique<Circle>(Environment::DANGER_INFINITY, adv_radius, pos.getPos2D()));
			}
			_mutex_adversary.unlock();
			return result;
		}

		void AvoidanceInterfacer::set_adversary_detection_angle(Angle angle) {
			if(abs(angle) < 0.5_PI) {
				_angle_detection_adversary.exchange(angle);
			} else {
				logWarn("Adversary detection angle too large! ", angle);
			}
		}

		Angle AvoidanceInterfacer::get_adversary_detection_angle() const {
			return _angle_detection_adversary;
		}

		void AvoidanceInterfacer::thread_lidar_main() {
			std::unique_ptr<OccupGrid> lidar_map =
			    std::make_unique<OccupGrid>(toVec2(GLOBAL_CONSTANTS().get_table_size()), 100, 66);

			StopWatch chrono;
			while(_is_running) {
				chrono.reset();

				auto turret_pos = get_robot_position().getPos2D() + _turret_shift;
				repere::Coordinates coords({turret_pos}, get_robot_orientation());
				lidar_map->reset();

				auto frame = _robot->get_lidar_frame();

				if(frame.has_value()) {
					lidar_map->accumulate(frame.value(), coords);
					FindRobots robots;
					robots.accumulate(*lidar_map);

					_mutex_adversary.lock();
					_adversary_positions = robots.get_results();
				} else {
					_mutex_adversary.lock();
					_adversary_positions.clear();
				}

				_mutex_adversary.unlock();

				auto to_sleep = GLOBAL_CONSTANTS().get_lidar_actualization_period() - chrono.getElapsedTime();
				if(to_sleep < 0_s) {
					logWarn("The Lidar frame's acquisition is too slow of ", -to_sleep);
				}
				sleep(to_sleep);
			}
		}

		repere::Position AvoidanceInterfacer::get_robot_position() const {
			try {
				return _robot->get_module<PhysicalRobot::Navigation>().get_position();
			} catch(const std::runtime_error&) {
				logWarn("The module `Navigation` doesn't exist.");
				return repere::Position();
			}
		}

		repere::Orientation AvoidanceInterfacer::get_robot_orientation() const {
			try {
				return _robot->get_module<PhysicalRobot::Navigation>().get_orientation();
			} catch(const std::runtime_error&) {
				logWarn("The module `Navigation` doesn't exist.");
				return repere::Orientation();
			}
		}
	} // namespace Interfacer
} // namespace Strategy
