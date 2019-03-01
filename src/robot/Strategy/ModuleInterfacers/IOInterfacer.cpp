//
// Created by terae on 13/02/19.
//

#include "IOInterfacer.h"

namespace Strategy {
	namespace Interfacer {
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
			while(get_tirette_state() == IOInterfacer::TriggerState::Waiting) {
				sleep(100_ms);
			}
			logInfo("Tirette inserted, please take off it!");
		}

		void IOInterfacer::wait_deletion_tirette() const {
			while(get_tirette_state() == IOInterfacer::TriggerState::Triggered) {
				sleep(100_ms);
			}
			logInfo("Tirette outside!");
		}
	} // namespace Interfacer
} // namespace Strategy
