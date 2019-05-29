//
// Created by terae on 28/05/19.
//

#include "PumpsInterfacerSecondary.h"
#include "../PetriLab/ServosSecondary.h"

namespace Strategy::Interfacer {

	PumpsInterfacerSecondary::PumpsInterfacerSecondary(interfaced_type& module_pumps, CaptorsInterfacerSecondary& captors)
	        : AbstractInterfacer(), _module(module_pumps), _captors(captors), _hand(AtomType::Nothing), _pump(0), _evacuation(0) {
		_module.deactivate_pump(_pump);
	}

	PumpsInterfacerSecondary::PumpsInterfacerSecondary(const std::shared_ptr<PhysicalRobot::Robot>& robot,
	                                                   CaptorsInterfacerSecondary& captors)
	        : PumpsInterfacerSecondary(*robot, captors) {}

	PumpsInterfacerSecondary::PumpsInterfacerSecondary(PhysicalRobot::Robot& robot, CaptorsInterfacerSecondary& captors)
	        : PumpsInterfacerSecondary(robot.get_module<interfaced_type>(), captors) {}

	ActionResult PumpsInterfacerSecondary::catch_atom() {
		_module.deactivate_valve(_evacuation);
		_module.activate_pump(_pump);
		_hand = _color_to_atom(_captors->read_color());
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

	AtomType PumpsInterfacerSecondary::get_color_hand() const {
		return _hand;
	}

	AtomType PumpsInterfacerSecondary::get_color_top_left() const {
		if(!_left_container.empty()) {
			return _left_container[_left_container.size() - 1];
		}
		return AtomType::Nothing;
	}

	AtomType PumpsInterfacerSecondary::get_color_top_right() const {
		if(!_right_container.empty()) {
			return _right_container[_right_container.size() - 1];
		}
		return AtomType::Nothing;
	}

	AtomType PumpsInterfacerSecondary::_color_to_atom(PhysicalRobot::CaptorColor color) const {
		switch(color) {
			case PhysicalRobot::CaptorColor::Red:
				return AtomType::Redium;
			case PhysicalRobot::CaptorColor::Blue:
				return AtomType::Blueium;
			case PhysicalRobot::CaptorColor::Green:
				return AtomType::Greenium;
		}
		return AtomType::Nothing;
	}

} // namespace Strategy::Interfacer
