//
// Created by terae on 28/05/19.
//

#include "PumpsInterfacerSecondary.h"
#include "../PetriLab/ServosSecondary.h"

namespace Strategy::Interfacer {

	PumpsInterfacerSecondary::PumpsInterfacerSecondary(interfaced_type& module_pumps)
	        : AbstractInterfacer(), _module(module_pumps), _hand(AtomType::Nothing), _pump(0), _evacuation(0) {
		_module.deactivate_pump(_pump);
	}

	PumpsInterfacerSecondary::PumpsInterfacerSecondary(const std::shared_ptr<PhysicalRobot::Robot>& robot)
	        : PumpsInterfacerSecondary(*robot) {}

	PumpsInterfacerSecondary::PumpsInterfacerSecondary(PhysicalRobot::Robot& robot)
	        : PumpsInterfacerSecondary(robot.get_module<interfaced_type>()) {}

	ActionResult PumpsInterfacerSecondary::catch_atom(AtomType atom) {
		_module.deactivate_valve(_evacuation);
		_module.activate_pump(_pump);
		_hand = atom;
		return ActionResult::SUCCESS;
	}


	ActionResult PumpsInterfacerSecondary::release_atom() {
		_module.deactivate_pump(_pump);
		_module.activate_valve(_evacuation);

		switch(get_elevator_position()) {
			case ElevatorPosition::TOP_LEFT: {
				_left_container.push_back(_hand);
				break;
			}
			case ElevatorPosition::TOP_RIGHT: {
				_right_container.push_back(_hand);
				break;
			}
		}
		_hand = AtomType::Nothing;

		return ActionResult::SUCCESS;
	}

} // namespace Strategy::Interfacer
