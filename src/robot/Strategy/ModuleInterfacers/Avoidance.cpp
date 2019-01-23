//
// Created by terae on 23/01/19.
//

#include "Avoidance.h"

namespace Strategy {
	namespace Interfacer {
		Avoidance::Avoidance(std::shared_ptr<PhysicalRobot::Robot> robot, Environment& env, Vector2m turret_shift)
		        : _is_running(true), _robot(robot), _env(env), _turret_shift(turret_shift) {
			_find_robots = std::thread(std::bind(&Avoidance::thread_lidar, this));
		}

		Avoidance::~Avoidance() {
			_is_running = false;
			_find_robots.join();
		}

		void Avoidance::set_turret_shift(Vector2m shift) {
			_turret_shift = shift;
		}

		std::vector<repere::Position> Avoidance::get_adversary_positions() const {
			std::lock_guard<std::mutex> lk(_mutex_adversary);
			return _adversary_positions;
		}

		bool Avoidance::adversary_detected(Distance threshold) const {
			repere::Position robot_position; // = this->_robot->get_module<PhysicalRobot::Moving>()::get_position();
			auto adversaries = get_adversary_positions();

			for(repere::Position pos : adversaries) {
				if((robot_position.getPos2D() - pos.getPos2D()).norm() < threshold) {
					return true;
				}
			}
			return false;
		}

		void Avoidance::thread_lidar() {
			std::unique_ptr<OccupGrid> lidar_map =
			    std::make_unique<OccupGrid>(toVec2(GLOBAL_CONSTANTS().get_table_size()), 100, 66);

			const int first_dynamic = 10;
			int last_dynamic = 0;

			while(_is_running) {
				repere::Coordinates coords; // = this->_robot->get_module<PhysicalRobot::Moving>()::get_coordinates() + _turret_shift;
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
				sleep(200_ms);
			}
		}
	} // namespace Interfacer
} // namespace Strategy
