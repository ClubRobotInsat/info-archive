//
// Created by terae on 01/03/19.
//

#include "PumpsInterfacer.h"

namespace Strategy {
	namespace Interfacer {

		PumpsInterfacer::PumpsInterfacer(interfaced_type& module_pumps)
		        : AbstractInterfacer(), _module(module_pumps), _pump_forward(0, 0, 1, 2), _pump_backward(1, 3, 4, 5) {
			_external_rail_forward.fill(StockingRail::Nothing);
			_external_rail_backward.fill(StockingRail::Nothing);
			_internal_rail.fill(StockingRail::Nothing);
			_hand_forward.fill(StockingRail::Nothing);
			_hand_backward.fill(StockingRail::Nothing);

			_module.deactivate_pump(_pump_forward.pump);
			_module.deactivate_pump(_pump_backward.pump);
		}

		PumpsInterfacer::PumpsInterfacer(std::shared_ptr<PhysicalRobot::Robot> robot) : PumpsInterfacer(*robot) {}

		PumpsInterfacer::PumpsInterfacer(PhysicalRobot::Robot& robot)
		        : PumpsInterfacer(robot.get_module<interfaced_type>()) {}

		ActionResult PumpsInterfacer::catch_forward(StockingRail left, StockingRail right) {
			// Check if each sucker is free
			if((_hand_forward[0] != Nothing && left != Nothing) || (_hand_forward[1] != Nothing && right != Nothing)) {
				return ActionResult::FAILURE;
			}

			_module.deactivate_valve(_pump_forward.evacuation);
			if(left != Nothing) {
				_module.activate_valve(_pump_forward.valve_left);
			}
			if(right != Nothing) {
				_module.activate_valve(_pump_forward.valve_right);
			}

			_module.activate_pump(_pump_forward.pump);

			ActionResult result;
			switch(_module.is_pump_activated(_pump_forward.pump)) {
				case PhysicalRobot::IOState::On:
					if(left != Nothing) {
						_hand_forward[0] = left;
					}
					if(right != Nothing) {
						_hand_forward[1] = right;
					}
					result = ActionResult::SUCCESS;
					break;
				case PhysicalRobot::IOState::Off:
					_module.deactivate_pump(_pump_forward.pump);
					result = ActionResult::FAILURE;
					break;
			}
			return result;
		}

		ActionResult PumpsInterfacer::catch_backward(StockingRail left, StockingRail right) {
			// Check if each sucker is free
			if((_hand_backward[0] != Nothing && left != Nothing) || (_hand_backward[1] != Nothing && right != Nothing)) {
				return ActionResult::FAILURE;
			}

			_module.deactivate_valve(_pump_backward.evacuation);
			if(left != Nothing) {
				_module.activate_valve(_pump_backward.valve_left);
			}
			if(right != Nothing) {
				_module.activate_valve(_pump_backward.valve_right);
			}

			_module.activate_pump(_pump_backward.pump);

			ActionResult result;
			switch(_module.is_pump_activated(_pump_backward.pump)) {
				case PhysicalRobot::IOState::On:
					if(left != Nothing) {
						_hand_backward[0] = left;
					}
					if(right != Nothing) {
						_hand_backward[1] = right;
					}
					result = ActionResult::SUCCESS;
					break;
				case PhysicalRobot::IOState::Off:
					_module.deactivate_pump(_pump_backward.pump);
					result = ActionResult::FAILURE;
					break;
			}
			return result;
		}

		ActionResult PumpsInterfacer::release_forward() {
			_module.deactivate_valve(_pump_forward.valve_left);
			_module.deactivate_valve(_pump_forward.valve_right);
			_module.activate_valve(_pump_forward.evacuation);

			_module.deactivate_pump(_pump_forward.pump);

			ActionResult result;
			switch(_module.is_pump_activated(_pump_forward.pump)) {
				case PhysicalRobot::IOState::On:
					result = ActionResult::FAILURE;
					break;
				case PhysicalRobot::IOState::Off:
					switch(get_forward_position()) {
						case ArmPosition::TOP_EXTERNAL_RAIL:
							_external_rail_forward[0] = _hand_forward[0];
							_external_rail_forward[1] = _hand_forward[1];
							break;
						case ArmPosition::TOP_INTERNAL_RAIL:
							// TODO: les palets peuvent rouler donc il faut gérer plus dynamiquement leur place
							_internal_rail[0] = _hand_forward[0];
							_internal_rail[1] = _hand_forward[1];
							break;
						default:
							logWarn("Bad forward arm position to release atoms: ", toString(get_forward_position()));
							break;
					}
					_hand_backward[0] = Nothing;
					_hand_backward[1] = Nothing;
					result = ActionResult::SUCCESS;
					break;
			}
			return result;
		}

		ActionResult PumpsInterfacer::release_backward() {
			_module.deactivate_valve(_pump_backward.valve_left);
			_module.deactivate_valve(_pump_backward.valve_right);
			_module.activate_valve(_pump_backward.evacuation);

			_module.deactivate_pump(_pump_backward.pump);

			ActionResult result;
			switch(_module.is_pump_activated(_pump_backward.pump)) {
				case PhysicalRobot::IOState::On:
					result = ActionResult::FAILURE;
					break;
				case PhysicalRobot::IOState::Off:
					switch(get_backward_position()) {
						case ArmPosition::TOP_EXTERNAL_RAIL:
							_external_rail_backward[0] = _hand_backward[0];
							_external_rail_backward[1] = _hand_backward[1];
							break;
						case ArmPosition::TOP_INTERNAL_RAIL:
							// TODO: les palets peuvent rouler donc il faut gérer plus dynamiquement leur place
							_internal_rail[0] = _hand_backward[0];
							_internal_rail[1] = _hand_backward[1];
							break;
						default:
							logWarn("Bad backward arm position to release atoms: ", toString(get_backward_position()));
							break;
					}
					_hand_backward[0] = Nothing;
					_hand_backward[1] = Nothing;
					result = ActionResult::SUCCESS;
					break;
			}
			return result;
		}

	} // namespace Interfacer
} // namespace Strategy
