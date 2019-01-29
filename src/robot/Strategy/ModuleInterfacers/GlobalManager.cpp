//
// Created by terae on 23/01/19.
//

#include "GlobalManager.h"

namespace Strategy {
	namespace Interfacer {

		GlobalManager::GlobalManager(std::shared_ptr<PhysicalRobot::Robot> robot) : _robot(std::move(robot)) {
			if(_robot == nullptr) {
				throw std::runtime_error("Interfacer::GlobalManager called with a nullptr as a robot.");
			}
		}

		std::shared_ptr<PhysicalRobot::Robot> GlobalManager::get_robot() const {
			return _robot;
		}
	} // namespace Interfacer
} // namespace Strategy
