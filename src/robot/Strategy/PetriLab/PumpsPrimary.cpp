//
// Created by terae on 24/03/19.
//

#include "PumpsPrimary.h"
#include "../ModuleInterfacers/RobotManager.h"

namespace {
	using namespace Strategy::Interfacer;

	std::shared_ptr<RobotManager> _manager;
	Constants::RobotColor _color;

	PumpsInterfacerPrimary& pumps() {
		return _manager->get_interfacer<PumpsInterfacerPrimary>();
	}
} // namespace

void init_petri_pumps_primary(std::shared_ptr<RobotManager> manager, Constants::RobotColor color) {
	_manager = std::move(manager);
	_color = color;
}

ActionResult catch_front(AtomType left, AtomType right) {
	if(GLOBAL_CONSTANTS().is_inverted(_color)) {
		return pumps().catch_front(right, left);
	} else {
		return pumps().catch_front(left, right);
	}
}

ActionResult catch_back(AtomType left, AtomType right) {
	if(GLOBAL_CONSTANTS().is_inverted(_color)) {
		return pumps().catch_back(right, left);
	} else {
		return pumps().catch_back(left, right);
	}
}

ActionResult release_all() {
	return pumps().release_all();
}
