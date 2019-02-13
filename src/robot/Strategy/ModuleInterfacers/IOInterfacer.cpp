//
// Created by terae on 13/02/19.
//

#include "IOInterfacer.h"

namespace Strategy {
	namespace Interfacer {
		IOInterfacer::IOInterfacer(Strategy::Interfacer::IOInterfacer::interfaced_type& module_io)
		        : AbstractInterfacer(), _module(module_io) {}

		IOInterfacer::IOInterfacer(std::shared_ptr<PhysicalRobot::Robot> robot) : IOInterfacer(*robot) {}

		IOInterfacer::IOInterfacer(PhysicalRobot::Robot& robot) : IOInterfacer(robot.get_module<interfaced_type>()) {}

		bool IOInterfacer::is_tirette_inserted() const {
			return _module.read_tirette();
		}

		void IOInterfacer::wait_insertion_tirette() const {
			logInfo("Wait the insertion of the tirette.");
			while(!is_tirette_inserted()) {
				sleep(100_ms);
			}
			logInfo("Tirette inserted, please take off it!");
		}

		void IOInterfacer::wait_deletion_tirette() const {
			while(!is_tirette_inserted()) {
				sleep(100_ms);
			}
			logInfo("Tirette outside!");
		}
	} // namespace Interfacer
} // namespace Strategy