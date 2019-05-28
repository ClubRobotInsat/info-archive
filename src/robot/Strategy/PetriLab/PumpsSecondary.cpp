//
// Created by terae on 24/03/19.
//

#include "PumpsSecondary.h"
#include "../ModuleInterfacers/RobotManager.h"

namespace {
	using namespace Strategy::Interfacer;

	std::shared_ptr<RobotManager> _manager;

	PumpsInterfacerSecondary& pumps() {
		return _manager->get_interfacer<PumpsInterfacerSecondary>();
	}
} // namespace

void init_petri_pumps_secondary(std::shared_ptr<RobotManager> manager) {
	_manager = std::move(manager);
}

ActionResult catch_atom(AtomType atom) {
	return pumps().catch_atom(atom);
}

ActionResult release_atom() {
	return pumps().release_atom();
}
