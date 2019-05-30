//
// Created by terae on 01/03/19.
//

#include "PumpsInterfacerPrimary.h"
#include "../PetriLab/ServosPrimary.h"

namespace Strategy::Interfacer {

	PumpsInterfacerPrimary::PumpsInterfacerPrimary(interfaced_type& module_pumps)
	        : AbstractInterfacer()
	        , _module(module_pumps)
	        , _pump(0)
	        , _front_left_valve(3)
	        , _front_right_valve(2)
	        , _back_left_valve(0)
	        , _back_right_valve(1) {
		_front_external_storage.fill(AtomType::Nothing);
		_back_external_storage.fill(AtomType::Nothing);
		_internal_storage.fill(AtomType::Nothing);
		_front_hand.fill(AtomType::Nothing);
		_back_hand.fill(AtomType::Nothing);

		_module.deactivate_pump(_pump);
	}

	PumpsInterfacerPrimary::PumpsInterfacerPrimary(const std::shared_ptr<PhysicalRobot::Robot>& robot)
	        : PumpsInterfacerPrimary(*robot) {}

	PumpsInterfacerPrimary::PumpsInterfacerPrimary(PhysicalRobot::Robot& robot)
	        : PumpsInterfacerPrimary(robot.get_module<interfaced_type>()) {}

	ActionResult PumpsInterfacerPrimary::catch_front(AtomType left, AtomType right) {
		// Check if each sucker is free
		if((_front_hand[0] != AtomType::Nothing && left != AtomType::Nothing) ||
		   (_front_hand[1] != AtomType::Nothing && right != AtomType::Nothing)) {
			logWarn("A front sucker is not free");
			return ActionResult::FAILURE;
		}

		if(left != AtomType::Nothing) {
			_module.activate_valve(_front_left_valve);
		}
		if(right != AtomType::Nothing) {
			_module.activate_valve(_front_right_valve);
		}

		_module.activate_pump(_pump);

		ActionResult result;
		switch(_module.is_pump_activated(_pump)) {
			case PhysicalRobot::IOState::On:
				if(left != AtomType::Nothing) {
					_front_hand[0] = left;
				}
				if(right != AtomType::Nothing) {
					_front_hand[1] = right;
				}
				result = ActionResult::SUCCESS;
				break;
			case PhysicalRobot::IOState::Off:
				//_module.deactivate_pump(_pump);
				result = ActionResult::FAILURE;
				break;
		}
		return result;
	}

	ActionResult PumpsInterfacerPrimary::catch_back(AtomType left, AtomType right) {
		// Check if each sucker is free
		if((_back_hand[0] != AtomType::Nothing && left != AtomType::Nothing) ||
		   (_back_hand[1] != AtomType::Nothing && right != AtomType::Nothing)) {
			logWarn("A back sucker is not free");
			return ActionResult::FAILURE;
		}

		_module.activate_pump(_pump);

		if(left != AtomType::Nothing) {
			_module.activate_valve(_back_left_valve);
		}
		if(right != AtomType::Nothing) {
			_module.activate_valve(_back_right_valve);
		}

		ActionResult result;
		switch(_module.is_pump_activated(_pump)) {
			case PhysicalRobot::IOState::On:
				if(left != AtomType::Nothing) {
					_back_hand[0] = left;
				}
				if(right != AtomType::Nothing) {
					_back_hand[1] = right;
				}
				result = ActionResult::SUCCESS;
				break;
			case PhysicalRobot::IOState::Off:
			default:
				//_module.deactivate_pump(_pump);
				result = ActionResult::FAILURE;
				break;
		}
		return result;
	}

	ActionResult PumpsInterfacerPrimary::release_all() {
		valve_t evacuation = std::numeric_limits<valve_t>::max();
		for(valve_t id = 0; id < PhysicalRobot::Pumps::NBR_MAX_VALVE; ++id) {
			if(_module.is_valve_activated(id) == PhysicalRobot::IOState::Off) {
				evacuation = id;
				break;
			}
		}
		if(evacuation == std::numeric_limits<valve_t>::max()) {
			logWarn("All valves are activated, impossible to release all atoms");
			return ActionResult::FAILURE;
		}
		_module.activate_valve(evacuation);
		sleep(200_ms);

		switch(get_front_position()) {
			case ArmPosition::TopExternalStorage: {
				if(_front_hand[0] != AtomType::Nothing) {
					_front_external_storage[0] = _front_hand[0];
					logInfo("The left front external storage now has a ", toString(_front_external_storage[0]), " atom");
				}
				if(_front_hand[1] != AtomType::Nothing) {
					_front_external_storage[1] = _front_hand[1];
					logInfo("The right front external storage now has a ", toString(_front_external_storage[1]), " atom");
				}
				break;
			}
			case ArmPosition::TopInternalStorage: {
				if(_front_hand[0] != AtomType::Nothing) {
					_internal_storage[0] = _front_hand[0];
					_front_hand[0] = AtomType::Nothing;
					logInfo("The left internal storage now has a ", toString(_internal_storage[0]), " atom");
				}
				if(_front_hand[1] != AtomType::Nothing) {
					_internal_storage[1] = _front_hand[1];
					_front_hand[1] = AtomType::Nothing;
					logInfo("The right internal storage now has a ", toString(_internal_storage[1]), " atom");
				}
				break;
			}
			case ArmPosition::ReleaseGoldenium: {
				if(_front_hand[0] == AtomType::Goldenium) {
					logInfo("The Goldenium was released from the left front hand_position");
				} else if(_front_hand[1] == AtomType::Goldenium) {
					logInfo("The Goldenium was released from the right front hand_position");
				}
				break;
			}
			default:
				break;
		}

		switch(get_back_position()) {
			case ArmPosition::TopExternalStorage: {
				if(_back_hand[0] != AtomType::Nothing) {
					_back_external_storage[0] = _back_hand[0];
					logInfo("The left back external storage now has a ", toString(_back_external_storage[0]), " atom");
				}
				if(_back_hand[1] != AtomType::Nothing) {
					_back_external_storage[1] = _back_hand[1];
					logInfo("The right back external storage now has a ", toString(_back_external_storage[1]), " atom");
				}
				break;
			}
			case ArmPosition::TopInternalStorage: {
				if(_back_hand[0] != AtomType::Nothing) {
					_internal_storage[0] = _back_hand[0];
					logInfo("The left internal storage now has a ", toString(_internal_storage[0]), " atom");
				}
				if(_back_hand[1] != AtomType::Nothing) {
					_internal_storage[1] = _back_hand[1];
					logInfo("The right internal storage now has a ", toString(_internal_storage[1]), " atom");
				}
				break;
			}
			case ArmPosition::ReleaseGoldenium: {
				if(_back_hand[0] == AtomType::Goldenium) {
					logInfo("The Goldenium was released from the left back hand_position");
				} else if(_back_hand[1] == AtomType::Goldenium) {
					logInfo("The Goldenium was released from the right back hand_position");
				}
				break;
			}
			default:
				break;
		}

		_front_hand[0] = AtomType::Nothing;
		_front_hand[1] = AtomType::Nothing;
		_back_hand[0] = AtomType::Nothing;
		_back_hand[1] = AtomType::Nothing;

		_module.deactivate_pump(_pump);

		return ActionResult::SUCCESS;
	}

} // namespace Strategy::Interfacer
