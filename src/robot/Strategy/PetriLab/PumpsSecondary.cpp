//
// Created by terae on 24/03/19.
//

#include "PumpsSecondary.h"
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

ActionResult catch_atom(AtomType atom) {
	// TODO
	logError("Not implemented!");
	return ActionResult::FAILURE;
}

ActionResult release_atom() {
	// TODO
	logError("Not implemented!");
	return ActionResult::FAILURE;
}