//
// Created by terae on 13/02/19.
//

#include "IOInterfacer.h"

namespace Strategy::Interfacer {

	IOInterfacer::IOInterfacer(interfaced_type& module_io) : AbstractInterfacer(), _module(module_io) {}

	IOInterfacer::IOInterfacer(std::shared_ptr<PhysicalRobot::Robot> robot) : IOInterfacer(*robot) {}

	IOInterfacer::IOInterfacer(PhysicalRobot::Robot& robot) : IOInterfacer(robot.get_module<interfaced_type>()) {}

	IOInterfacer::TriggerState IOInterfacer::get_tirette_state() const {
		return _module.read_tirette();
	}

	bool IOInterfacer::is_tirette_triggered() const {
		return get_tirette_state() == TriggerState::Triggered;
	}

	void IOInterfacer::wait_insertion_tirette() const {
		logInfo("Wait the insertion of the tirette.");
		while(!is_tirette_triggered()) {
			sleep(50_ms);
		}
		logInfo("Tirette inserted, please take off it!");
	}

	void IOInterfacer::wait_deletion_tirette() const {
		while(is_tirette_triggered()) {
			sleep(50_ms);
		}
		logInfo("Tirette outside!");
	}

	void IOInterfacer::reset_buzzer() {
		_module.set_sound(BuzzerState::Rest);
	}

	void IOInterfacer::play_success_sound() {
		_module.set_sound(BuzzerState::Rest);
		sleep(50_ms);
		_module.set_sound(BuzzerState::PlaySuccessSound);
	}

	void IOInterfacer::play_error_sound() {
		_module.set_sound(BuzzerState::Rest);
		sleep(50_ms);
		_module.set_sound(BuzzerState::PlayErrorSound);
	}

} // namespace Strategy::Interfacer
