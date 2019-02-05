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
		        , _angle_detection_adversary(GLOBAL_CONSTANTS()["primary"].get_angle_adversary_detection()) {
			_find_robots = std::thread(std::bind(&AvoidanceInterfacer::thread_lidar, this));
			std::string name = (_robot->name != "guest" ? _robot->name : "primary");
			set_turret_shift(GLOBAL_CONSTANTS()[name].get_turret_position());
		}

		AvoidanceInterfacer::~AvoidanceInterfacer() {
			_is_running = false;
			_find_robots.join();
		}

		void AvoidanceInterfacer::set_turret_shift(Vector2m shift) {
			_turret_shift = shift;
		}

		std::vector<repere::Position> AvoidanceInterfacer::get_adversary_positions() const {
			std::lock_guard<std::mutex> lk(_mutex_adversary);
			return _adversary_positions;
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

		void AvoidanceInterfacer::thread_lidar() {
			std::unique_ptr<OccupGrid> lidar_map =
			    std::make_unique<OccupGrid>(toVec2(GLOBAL_CONSTANTS().get_table_size()), 100, 66);

			const int first_dynamic = 10;
			int last_dynamic = 0;

			StopWatch chrono;
			while(_is_running) {
				chrono.reset();

				auto pos = get_robot_position().getPos2D() + _turret_shift;
				repere::Coordinates coords({pos}, get_robot_orientation());
				lidar_map->reset();

				auto frame = _robot->get_lidar_frame();
				if(frame != std::nullopt) {
					lidar_map->accumulate(frame.value(), coords);
					FindRobots robots;
					robots.accumulate(*lidar_map);

					_mutex_adversary.lock();
					_adversary_positions = robots.get_results();
				} else {
					_mutex_adversary.lock();
					_adversary_positions.clear();
				}

				for(int i = first_dynamic; i < first_dynamic + last_dynamic; ++i) {
					_env.removeDynamicShape(i);
				}
				for(repere::Position pos : _adversary_positions) {
					_env.addDynamicShape(first_dynamic + last_dynamic++,
					                     std::make_unique<Circle>(Environment::DANGER_INFINITY, 20_cm, pos.getPos2D()));
				}
				_mutex_adversary.unlock();

				auto to_sleep = GLOBAL_CONSTANTS().get_lidar_actualization_period() - chrono.getElapsedTime();
				if(to_sleep < 0_s) {
					logWarn("The Lidar frame's acquisition is too slow of ", to_sleep);
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
