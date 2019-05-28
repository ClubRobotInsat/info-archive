//
// Created by terae on 24/03/19.
//

#include "PumpsPrimary.h"
#include "../ModuleInterfacers/RobotManager.h"

namespace {
	using namespace Strategy::Interfacer;

	std::shared_ptr<RobotManager> _manager;

	PumpsInterfacerPrimary& pumps() {
		return _manager->get_interfacer<PumpsInterfacerPrimary>();
	}
} // namespace

void init_petri_pumps_primary(std::shared_ptr<RobotManager> manager) {
	_manager = std::move(manager);
}

ActionResult catch_front(AtomType left, AtomType right) {
	return pumps().catch_front(left, right);
}

ActionResult catch_back(AtomType left, AtomType right) {
	return pumps().catch_back(left, right);
}

ActionResult release_all() {
	return pumps().release_all();
}
