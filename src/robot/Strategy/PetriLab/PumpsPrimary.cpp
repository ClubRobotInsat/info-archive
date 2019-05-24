//
// Created by terae on 24/03/19.
//

#include "PumpsPrimary.h"
#include "../ModuleInterfacers/RobotManager.h"

namespace {
	using namespace Strategy::Interfacer;

	std::shared_ptr<RobotManager> _manager;

	PumpsInterfacer& pumps() {
		return _manager->get_interfacer<PumpsInterfacer>();
	}
} // namespace

void init_petri_pumps(std::shared_ptr<RobotManager> manager) {
	_manager = manager;
}

ActionResult catch_front(AtomType left, AtomType right) {
	return pumps().catch_front(left, right);
}

ActionResult catch_back(AtomType left, AtomType right) {
	return pumps().catch_back(left, right);
}

ActionResult release_front() {
	return pumps().release_front();
}

ActionResult release_back() {
	return pumps().release_back();
}
