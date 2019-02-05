//
// Created by terae on 23/01/19.
//

#include "RobotManager.h"

namespace Strategy {
	namespace Interfacer {

		RobotManager::RobotManager(std::shared_ptr<PhysicalRobot::Robot> robot) : _robot(std::move(robot)) {
			if(_robot == nullptr) {
				throw std::runtime_error("Interfacer::RobotManager called with a nullptr as a robot.");
			}
		}

		std::shared_ptr<PhysicalRobot::Robot> RobotManager::get_robot() const {
			return _robot;
		}
	} // namespace Interfacer
} // namespace Strategy
