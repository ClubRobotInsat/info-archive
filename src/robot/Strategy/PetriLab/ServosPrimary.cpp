//
// Created by terae on 01/02/19.
//

#include "ServosPrimary.h"
#include "../ModuleInterfacers/RobotManager.h"

namespace {

	using namespace Strategy::Interfacer;

	std::shared_ptr<RobotManager> _manager;
	Constants::RobotColor _color;

	ServosInterfacer& servos() {
		return _manager->get_interfacer<ServosInterfacer>();
	}

	const ServosInterfacer::servo_t ID_SERVO_FRONT_ARM = 1;
	const ServosInterfacer::servo_t ID_SERVO_FRONT_HAND = 0;
	const ServosInterfacer::servo_t ID_SERVO_BACK_ARM = 2;
	const ServosInterfacer::servo_t ID_SERVO_BACK_HAND = 6;
	const ServosInterfacer::servo_t ID_SERVO_FRONT_LEFT_STORAGE = 4;
	const ServosInterfacer::servo_t ID_SERVO_FRONT_RIGHT_STORAGE = 3;
	const ServosInterfacer::servo_t ID_SERVO_BACK_LEFT_STORAGE = 9;
	const ServosInterfacer::servo_t ID_SERVO_BACK_RIGHT_STORAGE = 7;
	const ServosInterfacer::servo_t ID_SERVO_INTERNAL_STORAGE = 5;

	ArmPosition front_position;
	ArmPosition back_position;

} // namespace

void init_petri_servos_primary(std::shared_ptr<RobotManager> manager, Constants::RobotColor color) {
	_manager = std::move(manager);
	_color = color;
}

// Control of each servo
ActionResult front_arm(Angle angle) {
	return servos().set_position(ID_SERVO_FRONT_ARM, angle);
}

ActionResult front_hand(Angle angle) {
	return servos().set_position(ID_SERVO_FRONT_HAND, angle);
}

ActionResult back_arm(Angle angle) {
	return servos().set_position(ID_SERVO_BACK_ARM, angle);
}

ActionResult back_hand(Angle angle) {
	return servos().set_position(ID_SERVO_BACK_HAND, angle);
}

ActionResult arm_position(Arm arm, Angle angle) {
	if(arm == Arm::FRONT) {
		return front_arm(angle);
	} else {
		return back_arm(angle);
	}
}

ActionResult hand(Arm arm, Angle angle) {
	if(arm == Arm::FRONT) {
		return front_hand(angle);
	} else {
		return back_hand(angle);
	}
}


ActionResult external_storage(ExternalStorage storage, Angle angle) {
	ServosInterfacer::servo_t id;
	bool inverted = GLOBAL_CONSTANTS().is_inverted(_color);

	switch(storage) {
		case ExternalStorage::FRONT_LEFT:
			id = inverted ? ID_SERVO_FRONT_RIGHT_STORAGE : ID_SERVO_FRONT_LEFT_STORAGE;
			break;
		case ExternalStorage::FRONT_RIGHT:
			id = inverted ? ID_SERVO_FRONT_LEFT_STORAGE : ID_SERVO_FRONT_RIGHT_STORAGE;
			break;
		case ExternalStorage::BACK_LEFT:
			id = inverted ? ID_SERVO_BACK_RIGHT_STORAGE : ID_SERVO_BACK_LEFT_STORAGE;
			break;
		case ExternalStorage::BACK_RIGHT:
			id = inverted ? ID_SERVO_BACK_LEFT_STORAGE : ID_SERVO_BACK_RIGHT_STORAGE;
			break;
	}

	return servos().set_position(id, angle);
}

ActionResult internal_storage(Angle angle) {
	return servos().set_position(ID_SERVO_INTERNAL_STORAGE, angle);
}

ArmPosition get_back_position() {
	return back_position;
}

ArmPosition get_front_position() {
	return front_position;
}

void set_position(Arm arm, ArmPosition position) {
	if(arm == Arm::FRONT) {
		front_position = position;
	} else {
		back_position = position;
	}
}

// Combined functions
ActionResult arm_bottom_horizontal(Arm arm) {
	std::vector<fun_ar> actions;

	ADD_FN(actions, hand, arm, 50_deg);
	ADD_FN(actions, arm_position, arm, 50_deg);

	set_position(arm, ArmPosition::BOTTOM_HORIZONTAL);

	return _combine_actions(actions);
}

ActionResult arm_bottom_vertical(Arm arm) {
	std::vector<fun_ar> actions;

	ADD_FN(actions, hand, arm, 50_deg);
	ADD_FN(actions, arm_position, arm, 50_deg);

	set_position(arm, ArmPosition::BOTTOM_VERTICAL);

	return _combine_actions(actions);
}

ActionResult arm_top_vertical(Arm arm) {
	std::vector<fun_ar> actions;

	ADD_FN(actions, hand, arm, 50_deg);
	ADD_FN(actions, arm_position, arm, 50_deg);

	set_position(arm, ArmPosition::TOP_VERTICAL);

	return _combine_actions(actions);
}

ActionResult arm_catch_goldenium(Arm arm) {
	std::vector<fun_ar> actions;

	ADD_FN(actions, hand, arm, 50_deg);
	ADD_FN(actions, arm_position, arm, 50_deg);

	set_position(arm, ArmPosition::CATCH_GOLDENIUM);

	return _combine_actions(actions);
}

ActionResult arm_release_goldenium(Arm arm) {
	std::vector<fun_ar> actions;

	ADD_FN(actions, hand, arm, 50_deg);
	ADD_FN(actions, arm_position, arm, 50_deg);

	set_position(arm, ArmPosition::RELEASE_GOLDENIUM);

	return _combine_actions(actions);
}

ActionResult arm_external_storage(Arm arm) {
	std::vector<fun_ar> actions;

	ADD_FN(actions, hand, arm, 50_deg);
	ADD_FN(actions, arm_position, arm, 50_deg);

	set_position(arm, ArmPosition::TOP_EXTERNAL_STORAGE);

	return _combine_actions(actions);
}

ActionResult arm_internal_storage(Arm arm) {
	std::vector<fun_ar> actions;

	ADD_FN(actions, hand, arm, 50_deg);
	ADD_FN(actions, arm_position, arm, 50_deg);

	set_position(arm, ArmPosition::TOP_INTERNAL_STORAGE);

	return _combine_actions(actions);
}

ActionResult open_external_storage(ExternalStorage storage) {
	return external_storage(storage, 50_deg);
}

ActionResult close_external_storage(ExternalStorage storage) {
	return external_storage(storage, 0_deg);
}

ActionResult open_internal_storage() {
	return internal_storage(0_deg);
}

ActionResult close_internal_storage() {
	return internal_storage(20_deg);
}

double get_angle_by_id(int id) {
	return servos()->read_position(static_cast<uint8_t>(id)).toMinusPiPi().toDeg();
}
