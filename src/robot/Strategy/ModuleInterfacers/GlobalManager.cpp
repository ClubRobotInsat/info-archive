//
// Created by terae on 23/01/19.
//

#include "GlobalManager.h"

namespace Strategy {
	namespace Interfacer {

		GlobalManager::GlobalManager(std::unique_ptr<PhysicalRobot::Robot> robot) : _robot(std::move(robot)) {}

		PhysicalRobot::Robot& GlobalManager::get_robot() const {
			return *_robot;
		}
	} // namespace Interfacer
} // namespace Strategy
