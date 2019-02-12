//
// Created by terae on 01/02/19.
//

#include "Navigation.h"
#include "../ModuleInterfacers/RobotManager.h"

namespace {
	std::shared_ptr<Strategy::Interfacer::RobotManager> _manager;
	Constants::RobotColor _color;

	Strategy::Interfacer::NavigationInterfacer& navigation() {
		return _manager->get_interfacer<Strategy::Interfacer::NavigationInterfacer>();
	}
} // namespace

void init_petri_navigation(std::shared_ptr<Strategy::Interfacer::RobotManager> manager, Constants::RobotColor color) {
	_manager = manager;
	_color = color;
}

using PhysicalRobot::SensAdvance;
using PhysicalRobot::SensRotation;

ActionResult forward(Distance distance) {
	return navigation().forward(distance, SensAdvance::Forward);
}

ActionResult backward(Distance distance) {
	return navigation().forward(distance, SensAdvance::Backward);
}
