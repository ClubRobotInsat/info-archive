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

ActionResult play_success_sound() {
	io().reset_buzzer();
	sleep(50_ms);
	io().play_success_sound();
	sleep(2_s);
	return ActionResult::SUCCESS;
}

ActionResult play_error_sound() {
	io().reset_buzzer();
	sleep(50_ms);
	io().play_error_sound();
	sleep(2_s);
	return ActionResult::SUCCESS;
}
