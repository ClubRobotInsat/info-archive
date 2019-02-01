//
// Created by terae on 01/02/19.
//

#include "Avoidance.h"
#include "../ModuleInterfacers/GlobalManager.h"

namespace {
	std::shared_ptr<Strategy::Interfacer::GlobalManager> _manager;

	Strategy::Interfacer::Avoidance& avoidance() {
		return _manager->get_interfacer<Strategy::Interfacer::Avoidance>();
	}
} // namespace

void init_petri_avoidance(std::shared_ptr<Strategy::Interfacer::GlobalManager> manager) {
	_manager = manager;
}

bool adversary_detected() {
	return avoidance().adversary_detected(30_cm);
}

bool adversary_detected(Distance threshold) {
	return avoidance().adversary_detected(threshold);
}
