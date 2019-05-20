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

ActionResult catch_forward(StockingRail left, StockingRail right) {
	return pumps().catch_forward(left, right);
}

ActionResult catch_backward(StockingRail left, StockingRail right) {
	return pumps().catch_backward(left, right);
}

ActionResult release_forward() {
	return pumps().release_forward();
}

ActionResult release_backward() {
	return pumps().release_backward();
}
