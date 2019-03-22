//
// Created by terae on 01/02/19.
//

#include "Servos.h"
#include "../ModuleInterfacers/RobotManager.h"

namespace {
	using namespace Strategy::Interfacer;

	std::shared_ptr<RobotManager> _manager;
	Constants::RobotColor _color;

	ServosInterfacer& servos() {
		return _manager->get_interfacer<ServosInterfacer>();
	}

	const ServosInterfacer::servo_t ID_SERVO_FORWARD_ARM = 1;
	const ServosInterfacer::servo_t ID_SERVO_FORWARD_HAND = 2;
	const ServosInterfacer::servo_t ID_SERVO_BACKWARD_ARM = 3;
	const ServosInterfacer::servo_t ID_SERVO_BACKWARD_HAND = 4;

	ArmPosition forward_position;
	ArmPosition backward_position;
} // namespace

void init_petri_servos(std::shared_ptr<RobotManager> manager, Constants::RobotColor color) {
	_manager = manager;
	_color = color;
}

// Control of each servo
ActionResult forward_arm(Angle angle) {
	return servos().set_position(ID_SERVO_FORWARD_ARM, angle);
}

ActionResult forward_hand(Angle angle) {
	return servos().set_position(ID_SERVO_FORWARD_HAND, angle);
}

ActionResult backward_arm(Angle angle) {
	return servos().set_position(ID_SERVO_BACKWARD_ARM, angle);
}

ActionResult backward_hand(Angle angle) {
	return servos().set_position(ID_SERVO_BACKWARD_HAND, angle);
}

// Different positions of the forward-arm servo
ActionResult forward_arm_bottom_horizontal() {
	return forward_arm(50_deg);
}

ActionResult forward_arm_bottom_vertical() {
	return forward_arm(10_deg);
}

ActionResult forward_arm_bottom_goldenium() {
	return forward_arm(-10_deg);
}

ActionResult forward_arm_top_external_rail() {
	return forward_arm(40_deg);
}

ActionResult forward_arm_top_internal_rail() {
	return forward_arm(-50_deg);
}

// Different positions of the forward-hand servo
ActionResult forward_hand_bottom_horizontal() {
	return forward_hand(50_deg);
}

ActionResult forward_hand_bottom_vertical() {
	return forward_hand(10_deg);
}

ActionResult forward_hand_bottom_goldenium() {
	return forward_hand(-10_deg);
}

ActionResult forward_hand_top_external_rail() {
	return forward_hand(40_deg);
}

ActionResult forward_hand_top_internal_rail() {
	return forward_hand(-50_deg);
}

// Different positions of the backward-arm servo
ActionResult backward_arm_bottom_horizontal() {
	return backward_arm(50_deg);
}

ActionResult backward_arm_bottom_vertical() {
	return backward_arm(10_deg);
}

ActionResult backward_arm_bottom_goldenium() {
	return backward_arm(-10_deg);
}

ActionResult backward_arm_top_external_rail() {
	return backward_arm(40_deg);
}

ActionResult backward_arm_top_internal_rail() {
	return backward_arm(-50_deg);
}

// Different positions of the backward-hand servo
ActionResult backward_hand_bottom_horizontal() {
	return backward_hand(50_deg);
}

ActionResult backward_hand_bottom_vertical() {
	return backward_hand(10_deg);
}

ActionResult backward_hand_bottom_goldenium() {
	return backward_hand(-10_deg);
}

ActionResult backward_hand_top_external_rail() {
	return backward_hand(40_deg);
}

ActionResult backward_hand_top_internal_rail() {
	return backward_hand(-50_deg);
}

// Combined functions
ActionResult forward_bottom_horizontal() {
	std::vector<fun_ra> actions;

	ADD_FN(actions, forward_hand_bottom_horizontal);
	ADD_FN(actions, forward_arm_bottom_horizontal);

	forward_position = ArmPosition::BOTTOM_HORIZONTAL;

	return _combine_actions(actions);
}

ActionResult forward_bottom_vertical() {
	std::vector<fun_ra> actions;

	ADD_FN(actions, forward_hand_bottom_vertical);
	ADD_FN(actions, forward_arm_bottom_vertical);

	forward_position = ArmPosition::BOTTOM_VERTICAL;

	return _combine_actions(actions);
}

ActionResult forward_bottom_goldenium() {
	std::vector<fun_ra> actions;

	ADD_FN(actions, forward_hand_bottom_goldenium);
	ADD_FN(actions, forward_arm_bottom_goldenium);

	forward_position = ArmPosition::BOTTOM_GOLDENIUM;

	return _combine_actions(actions);
}

ActionResult forward_top_external_rail() {
	std::vector<fun_ra> actions;

	ADD_FN(actions, forward_hand_top_external_rail);
	ADD_FN(actions, forward_arm_top_external_rail);

	forward_position = ArmPosition::TOP_EXTERNAL_RAIL;

	return _combine_actions(actions);
}

ActionResult forward_top_internal_rail() {
	std::vector<fun_ra> actions;

	ADD_FN(actions, forward_hand_top_internal_rail);
	ADD_FN(actions, forward_arm_top_internal_rail);

	forward_position = ArmPosition::TOP_INTERNAL_RAIL;

	return _combine_actions(actions);
}

ArmPosition get_forward_position() {
	return forward_position;
}

ActionResult backward_bottom_horizontal() {
	std::vector<fun_ra> actions;

	ADD_FN(actions, backward_hand_bottom_horizontal);
	ADD_FN(actions, backward_arm_bottom_horizontal);

	backward_position = ArmPosition::BOTTOM_HORIZONTAL;

	return _combine_actions(actions);
}

ActionResult backward_bottom_vertical() {
	std::vector<fun_ra> actions;

	ADD_FN(actions, backward_hand_bottom_vertical);
	ADD_FN(actions, backward_arm_bottom_vertical);

	backward_position = ArmPosition::BOTTOM_VERTICAL;

	return _combine_actions(actions);
}

ActionResult backward_bottom_goldenium() {
	std::vector<fun_ra> actions;

	ADD_FN(actions, backward_hand_bottom_goldenium);
	ADD_FN(actions, backward_arm_bottom_goldenium);

	backward_position = ArmPosition::BOTTOM_GOLDENIUM;

	return _combine_actions(actions);
}

ActionResult backward_top_external_rail() {
	std::vector<fun_ra> actions;

	ADD_FN(actions, backward_hand_top_external_rail);
	ADD_FN(actions, backward_arm_top_external_rail);

	backward_position = ArmPosition::TOP_EXTERNAL_RAIL;

	return _combine_actions(actions);
}

ActionResult backward_top_internal_rail() {
	std::vector<fun_ra> actions;

	ADD_FN(actions, backward_hand_top_internal_rail);
	ADD_FN(actions, backward_arm_top_internal_rail);

	backward_position = ArmPosition::TOP_INTERNAL_RAIL;

	return _combine_actions(actions);
}

ArmPosition get_backward_position() {
	return backward_position;
}
