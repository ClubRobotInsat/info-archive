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

	std::shared_ptr<std::atomic<Distance>> _actuator_offset;

	Distance get_actuator_offset() {
		if(_actuator_offset == nullptr) {
			return 0_m;
		} else {
			return _actuator_offset->load();
		}
	}
} // namespace

void init_petri_avoidance(std::shared_ptr<Strategy::Interfacer::RobotManager> manager,
                          std::shared_ptr<std::atomic<Distance>> actuator_offset) {
	_manager = manager;
	_actuator_offset = actuator_offset;
}

bool adversary_detected() {
	return avoidance().adversary_detected(30_cm + get_actuator_offset());
}

bool adversary_detected(Distance threshold) {
	return avoidance().adversary_detected(threshold + get_actuator_offset());
}
