//
// Created by terae on 25/03/19.
//

#include "IO.h"
#include "../ModuleInterfacers/RobotManager.h"

namespace {
	std::shared_ptr<Strategy::Interfacer::RobotManager> _manager;

	Strategy::Interfacer::IOInterfacer& io() {
		return _manager->get_interfacer<Strategy::Interfacer::IOInterfacer>();
	}
} // namespace

void init_petri_io(std::shared_ptr<Strategy::Interfacer::RobotManager> manager) {
	_manager = manager;
}

Outcome play_success_sound() {
	io().play_success_sound();
	sleep(2_s);
	return Outcome::SUCCESS;
}

Outcome play_error_sound() {
	io().play_error_sound();
	sleep(2_s);
	return Outcome::SUCCESS;
}
