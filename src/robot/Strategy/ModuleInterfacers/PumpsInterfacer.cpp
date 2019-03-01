//
// Created by terae on 01/03/19.
//

#include "PumpsInterfacer.h"

namespace Strategy {
	namespace Interfacer {

		PumpsInterfacer::PumpsInterfacer(interfaced_type& module_pumps) : AbstractInterfacer(), _module(module_pumps) {}

		PumpsInterfacer::PumpsInterfacer(std::shared_ptr<PhysicalRobot::Robot> robot) : PumpsInterfacer(*robot) {}

		PumpsInterfacer::PumpsInterfacer(PhysicalRobot::Robot& robot)
		        : PumpsInterfacer(robot.get_module<interfaced_type>()) {}

		PumpsInterfacer::interfaced_type* PumpsInterfacer::operator->() {
			return &_module;
		}
	} // namespace Interfacer
} // namespace Strategy
