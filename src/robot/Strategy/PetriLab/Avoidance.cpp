//
// Created by terae on 01/02/19.
//

#include "Avoidance.h"
#include "../ModuleInterfacers/RobotManager.h"

namespace {
	std::shared_ptr<Strategy::Interfacer::RobotManager> _manager;

	Strategy::Interfacer::AvoidanceInterfacer& avoidance() {
		return _manager->get_interfacer<Strategy::Interfacer::AvoidanceInterfacer>();
	}
} // namespace

void init_petri_avoidance(std::shared_ptr<Strategy::Interfacer::RobotManager> manager) {
	_manager = manager;
}

bool adversary_detected() {
	return avoidance().adversary_detected(30_cm);
}

bool adversary_detected(Distance threshold) {
	return avoidance().adversary_detected(threshold);
}
