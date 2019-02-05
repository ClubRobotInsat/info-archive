//
// Created by terae on 01/02/19.
//

#include "Navigation.h"
#include "../ModuleInterfacers/RobotManager.h"

namespace {
	std::shared_ptr<Strategy::Interfacer::GlobalManager> _manager;
	Constants::RobotColor _color;

	Strategy::Interfacer::NavigationInterfacer& navigation() {
		return _manager->get_interfacer<Strategy::Interfacer::NavigationInterfacer>();
	}
} // namespace

void init_petri_navigation(std::shared_ptr<Strategy::Interfacer::GlobalManager> manager, Constants::RobotColor color) {
	_manager = manager;
	_color = color;
}

ActionResult go_forward(Distance distance) {
	sleep(1_s);
	return ActionResult::TIMEOUT;
}

ActionResult go_backward(Distance distance) {
	sleep(1_s);
	return ActionResult::TIMEOUT;
}
