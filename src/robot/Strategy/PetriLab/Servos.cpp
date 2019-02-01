//
// Created by terae on 01/02/19.
//

#include "Servos.h"
#include "../ModuleInterfacers/GlobalManager.h"

namespace {
	std::shared_ptr<Strategy::Interfacer::GlobalManager> _manager;
	Constants::RobotColor _color;

	Strategy::Interfacer::ServosManager& servos() {
		return _manager->get_interfacer<Strategy::Interfacer::ServosManager>();
	}
} // namespace

void init_petri_servos(std::shared_ptr<Strategy::Interfacer::GlobalManager> manager, Constants::RobotColor color) {
	_manager = manager;
	_color = color;
}

ActionResult open_right_door() {
	if(_color == Constants::RobotColor::Yellow) {
		return servos().set_position(6, 20_deg);
	} else {
		return servos().set_position(8, 80_deg);
	}
}

ActionResult open_left_door() {
	return servos().set_position(5, 160_deg);
}

ActionResult close_right_door() {
	return servos().set_position(6, 80_deg);
}

ActionResult close_left_door() {
	return servos().set_position(5, 50_deg);
}
