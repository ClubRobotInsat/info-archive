//
// Created by terae on 01/03/19.
//

#include "PumpsInterfacer.h"

namespace Strategy {
	namespace Interfacer {

		PumpsInterfacer::PumpsInterfacer(interfaced_type& module_pumps)
		        : AbstractInterfacer(), _module(module_pumps), _front_pump(0, 0, 1, 2), _back_pump(1, 3, 4, 5) {
			_front_external_storage.fill(AtomType::Nothing);
			_back_external_storage.fill(AtomType::Nothing);
			_internal_storage.fill(AtomType::Nothing);
			_front_hand.fill(AtomType::Nothing);
			_back_hand.fill(AtomType::Nothing);

			_module.deactivate_pump(_front_pump.pump);
			_module.deactivate_pump(_back_pump.pump);
		}

		PumpsInterfacer::PumpsInterfacer(std::shared_ptr<PhysicalRobot::Robot> robot) : PumpsInterfacer(*robot) {}

		PumpsInterfacer::PumpsInterfacer(PhysicalRobot::Robot& robot)
		        : PumpsInterfacer(robot.get_module<interfaced_type>()) {}

		ActionResult PumpsInterfacer::catch_front(AtomType left, AtomType right) {
			// Check if each sucker is free
			if((_front_hand[0] != Nothing && left != Nothing) || (_front_hand[1] != Nothing && right != Nothing)) {
				return ActionResult::FAILURE;
			}

			_module.deactivate_valve(_front_pump.evacuation);
			if(left != Nothing) {
				_module.activate_valve(_front_pump.valve_left);
			}
			if(right != Nothing) {
				_module.activate_valve(_front_pump.valve_right);
			}

			_module.activate_pump(_front_pump.pump);

			ActionResult result;
			switch(_module.is_pump_activated(_front_pump.pump)) {
				case PhysicalRobot::IOState::On:
					if(left != Nothing) {
						_front_hand[0] = left;
					}
					if(right != Nothing) {
						_front_hand[1] = right;
					}
					result = ActionResult::SUCCESS;
					break;
				case PhysicalRobot::IOState::Off:
					_module.deactivate_pump(_front_pump.pump);
					result = ActionResult::FAILURE;
					break;
			}
			return result;
		}

		ActionResult PumpsInterfacer::catch_back(AtomType left, AtomType right) {
			// Check if each sucker is free
			if((_back_hand[0] != Nothing && left != Nothing) || (_back_hand[1] != Nothing && right != Nothing)) {
				return ActionResult::FAILURE;
			}

			_module.deactivate_valve(_back_pump.evacuation);
			if(left != Nothing) {
				_module.activate_valve(_back_pump.valve_left);
			}
			if(right != Nothing) {
				_module.activate_valve(_back_pump.valve_right);
			}

			_module.activate_pump(_back_pump.pump);

			ActionResult result;
			switch(_module.is_pump_activated(_back_pump.pump)) {
				case PhysicalRobot::IOState::On:
					if(left != Nothing) {
						_back_hand[0] = left;
					}
					if(right != Nothing) {
						_back_hand[1] = right;
					}
					result = ActionResult::SUCCESS;
					break;
				case PhysicalRobot::IOState::Off:
				default:
					_module.deactivate_pump(_back_pump.pump);
					result = ActionResult::FAILURE;
					break;
			}
			return result;
		}

		ActionResult PumpsInterfacer::release_front() {
			_module.deactivate_valve(_front_pump.valve_left);
			_module.deactivate_valve(_front_pump.valve_right);
			_module.activate_valve(_front_pump.evacuation);

			_module.deactivate_pump(_front_pump.pump);

			ActionResult result;
			switch(_module.is_pump_activated(_front_pump.pump)) {
				case PhysicalRobot::IOState::On:
					result = ActionResult::FAILURE;
					break;
				case PhysicalRobot::IOState::Off:
					switch(get_front_position()) {
						case ArmPosition::TOP_EXTERNAL_STORAGE:
							_front_external_storage[0] = _front_hand[0];
							_front_external_storage[1] = _front_hand[1];
							break;
						case ArmPosition::TOP_INTERNAL_STORAGE:
							// TODO: les palets peuvent rouler donc il faut gérer plus dynamiquement leur place
							_internal_storage[0] = _front_hand[0];
							_internal_storage[1] = _front_hand[1];
							break;
						default:
							logWarn("Bad forward arm position to release atoms: ", toString(get_front_position()));
							break;
					}
					_back_hand[0] = Nothing;
					_back_hand[1] = Nothing;
					result = ActionResult::SUCCESS;
					break;
			}
			return result;
		}

		ActionResult PumpsInterfacer::release_back() {
			_module.deactivate_valve(_back_pump.valve_left);
			_module.deactivate_valve(_back_pump.valve_right);
			_module.activate_valve(_back_pump.evacuation);

			_module.deactivate_pump(_back_pump.pump);

			ActionResult result;
			switch(_module.is_pump_activated(_back_pump.pump)) {
				case PhysicalRobot::IOState::Off:
					switch(get_back_position()) {
						case ArmPosition::TOP_EXTERNAL_STORAGE:
							_back_external_storage[0] = _back_hand[0];
							_back_external_storage[1] = _back_hand[1];
							break;
						case ArmPosition::TOP_INTERNAL_STORAGE:
							// TODO: les palets peuvent rouler donc il faut gérer plus dynamiquement leur place
							_internal_storage[0] = _back_hand[0];
							_internal_storage[1] = _back_hand[1];
							break;
						default:
							logWarn("Bad backward arm position to release atoms: ", toString(get_back_position()));
							break;
					}
					_back_hand[0] = Nothing;
					_back_hand[1] = Nothing;
					result = ActionResult::SUCCESS;
					break;
				case PhysicalRobot::IOState::On:
				default:
					result = ActionResult::FAILURE;
					break;
			}
			return result;
		}

	} // namespace Interfacer
} // namespace Strategy
