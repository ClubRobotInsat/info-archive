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
	if(arm == Arm::Front) {
		return front_arm(angle);
	} else {
		return back_arm(angle);
	}
}

ActionResult hand_position(Arm arm, Angle angle) {
	if(arm == Arm::Front) {
		return front_hand(angle);
	} else {
		return back_hand(angle);
	}
}

ArmPosition get_back_position() {
	return back_position;
}

ArmPosition get_front_position() {
	return front_position;
}

void set_position(Arm arm, ArmPosition position) {
	if(arm == Arm::Front) {
		front_position = position;
	} else {
		back_position = position;
	}
}

// Combined functions
ActionResult arm_bottom_horizontal(Arm arm) {
	std::vector<fun_ar> actions;

	switch(arm) {
		case Arm::Front: {
			logWarn("Front servos aren't calibrated yet");
			ADD_FN(actions, []() { return ActionResult::FAILURE; });
			break;
		}
		case Arm::Back: {
			ADD_FN(actions, arm_position, arm, 90_deg);
			ADD_FN(actions, hand_position, arm, -30_deg);
			break;
		}
	}

	ActionResult res = _combine_actions(actions);
	if(res != ActionResult::SUCCESS) {
		set_position(arm, ArmPosition::Unknown);
	} else {
		set_position(arm, ArmPosition::BottomHorizontal);
	}

	return res;
}

ActionResult arm_bottom_vertical(Arm arm) {
	std::vector<fun_ar> actions;

	switch(arm) {
		case Arm::Front: {
			logWarn("Front servos aren't calibrated yet");
			ADD_FN(actions, []() { return ActionResult::FAILURE; });
			break;
		}
		case Arm::Back: {
			ADD_FN(actions, arm_position, arm, 65_deg);
			ADD_FN(actions, hand_position, arm, -100_deg);
			break;
		}
	}

	ActionResult res = _combine_actions(actions);
	if(res != ActionResult::SUCCESS) {
		set_position(arm, ArmPosition::Unknown);
	} else {
		set_position(arm, ArmPosition::BottomVertical);
	}

	return res;
}

ActionResult arm_top_vertical(Arm arm) {
	std::vector<fun_ar> actions;

	switch(arm) {
		case Arm::Front: {
			logWarn("Front servos aren't calibrated yet");
			ADD_FN(actions, []() { return ActionResult::FAILURE; });
			break;
		}
		case Arm::Back: {
			ADD_FN(actions, arm_position, arm, 20_deg);
			ADD_FN(actions, hand_position, arm, -40_deg);
			break;
		}
	}

	ActionResult res = _combine_actions(actions);
	if(res != ActionResult::SUCCESS) {
		set_position(arm, ArmPosition::Unknown);
	} else {
		set_position(arm, ArmPosition::TopVertical);
	}

	return res;
}

ActionResult arm_catch_goldenium(Arm arm) {
	std::vector<fun_ar> actions;

	switch(arm) {
		case Arm::Front: {
			logWarn("Front servos aren't calibrated yet");
			ADD_FN(actions, []() { return ActionResult::FAILURE; });
			break;
		}
		case Arm::Back: {
			ADD_FN(actions, arm_position, arm, 15_deg);
			ADD_FN(actions, hand_position, arm, -50_deg);
			break;
		}
	}

	ActionResult res = _combine_actions(actions);
	if(res != ActionResult::SUCCESS) {
		set_position(arm, ArmPosition::Unknown);
	} else {
		set_position(arm, ArmPosition::CatchGoldenium);
	}

	return res;
}

ActionResult arm_release_goldenium(Arm arm) {
	std::vector<fun_ar> actions;

	switch(arm) {
		case Arm::Front: {
			logWarn("Front servos aren't calibrated yet");
			ADD_FN(actions, []() { return ActionResult::FAILURE; });
			break;
		}
		case Arm::Back: {
			/// TODO : déposer le goldenium sur la balance
			logWarn("Back servos aren't calibrated yet for the goldenium release");
			ADD_FN(actions, []() { return ActionResult::FAILURE; });
			// ADD_FN(actions, arm_position, arm, 65_deg);
			// ADD_FN(actions, hand_position, arm, -100_deg);
			break;
		}
	}

	ActionResult res = _combine_actions(actions);
	if(res != ActionResult::SUCCESS) {
		set_position(arm, ArmPosition::Unknown);
	} else {
		set_position(arm, ArmPosition::ReleaseGoldenium);
	}

	return res;
}

ActionResult arm_external_storage(Arm arm) {
	std::vector<fun_ar> actions;

	switch(arm) {
		case Arm::Front: {
			logWarn("Front servos aren't calibrated yet");
			ADD_FN(actions, []() { return ActionResult::FAILURE; });
			break;
		}
		case Arm::Back: {
			ADD_FN(actions, hand_position, arm, -50_deg);
			ADD_FN(actions, arm_position, arm, -30_deg);
			ADD_FN(actions, hand_position, arm, -15_deg);
			ADD_FN(actions, arm_position, arm, -50_deg);
			ADD_FN(actions, hand_position, arm, -5_deg);
			ADD_FN(actions, arm_position, arm, -80_deg);
			ADD_FN(actions, hand_position, arm, 10_deg);
			break;
		}
	}

	ActionResult res = _combine_actions(actions);
	sleep(300_ms);
	if(res != ActionResult::SUCCESS) {
		set_position(arm, ArmPosition::Unknown);
	} else {
		set_position(arm, ArmPosition::TopExternalStorage);
	}

	return res;
}

ActionResult arm_internal_storage(Arm arm) {
	std::vector<fun_ar> actions;

	logWarn("The internal storage isn't available");
	return ActionResult::FAILURE;
}

ActionResult open_external_storage(ExternalStorage storage) {
	bool inverted = GLOBAL_CONSTANTS().is_inverted(_color);
	ActionResult res;

	const Angle ANGLE_BACK_LEFT = -20_deg;
	const Angle ANGLE_BACK_RIGHT = -15_deg;
	const Angle ANGLE_FRONT_LEFT = 0_deg;
	const Angle ANGLE_FRONT_RIGHT = 0_deg;

	switch(storage) {
		case ExternalStorage::FrontLeft: {
			if(inverted) {
				// res = servos().set_position(ID_SERVO_FRONT_RIGHT_STORAGE, ANGLE_FRONT_RIGHT);
			} else {
				// res = servos().set_position(ID_SERVO_FRONT_LEFT_STORAGE, ANGLE_FRONT_LEFT);
			}

			logWarn("The front external storage isn't available yet");
			res = ActionResult::FAILURE;
			break;
		}
		case ExternalStorage::FrontRight: {
			if(inverted) {
				// res = servos().set_position(ID_SERVO_FRONT_LEFT_STORAGE, ANGLE_FRONT_LEFT);
			} else {
				// res = servos().set_position(ID_SERVO_FRONT_RIGHT_STORAGE, ANGLE_FRONT_RIGHT);
			}

			logWarn("The front external storage isn't available yet");
			res = ActionResult::FAILURE;
			break;
		}
		case ExternalStorage::BackLeft: {
			if(inverted) {
				res = servos().set_position(ID_SERVO_BACK_RIGHT_STORAGE, ANGLE_BACK_RIGHT);
			} else {
				res = servos().set_position(ID_SERVO_BACK_LEFT_STORAGE, ANGLE_BACK_LEFT);
			}
			break;
		}
		case ExternalStorage::BackRight: {
			if(inverted) {
				res = servos().set_position(ID_SERVO_BACK_LEFT_STORAGE, ANGLE_BACK_LEFT);
			} else {
				res = servos().set_position(ID_SERVO_BACK_RIGHT_STORAGE, ANGLE_BACK_RIGHT);
			}
			break;
		}
	}

	return res;
}

ActionResult close_external_storage(ExternalStorage storage) {
	bool inverted = GLOBAL_CONSTANTS().is_inverted(_color);
	ActionResult res;

	const Angle ANGLE_BACK_LEFT = 10_deg;
	const Angle ANGLE_BACK_RIGHT = 30_deg;
	const Angle ANGLE_FRONT_LEFT = 0_deg;
	const Angle ANGLE_FRONT_RIGHT = 0_deg;

	switch(storage) {
		case ExternalStorage::FrontLeft: {
			if(inverted) {
				// res = servos().set_position(ID_SERVO_FRONT_RIGHT_STORAGE, ANGLE_FRONT_RIGHT);
			} else {
				// res = servos().set_position(ID_SERVO_FRONT_LEFT_STORAGE, ANGLE_FRONT_LEFT);
			}

			logWarn("The front external storage isn't available yet");
			res = ActionResult::FAILURE;
			break;
		}
		case ExternalStorage::FrontRight: {
			if(inverted) {
				// res = servos().set_position(ID_SERVO_FRONT_LEFT_STORAGE, ANGLE_FRONT_LEFT);
			} else {
				// res = servos().set_position(ID_SERVO_FRONT_RIGHT_STORAGE, ANGLE_FRONT_RIGHT);
			}

			logWarn("The front external storage isn't available yet");
			res = ActionResult::FAILURE;
			break;
		}
		case ExternalStorage::BackLeft: {
			if(inverted) {
				res = servos().set_position(ID_SERVO_BACK_RIGHT_STORAGE, ANGLE_BACK_RIGHT);
			} else {
				res = servos().set_position(ID_SERVO_BACK_LEFT_STORAGE, ANGLE_BACK_LEFT);
			}
			break;
		}
		case ExternalStorage::BackRight: {
			if(inverted) {
				res = servos().set_position(ID_SERVO_BACK_LEFT_STORAGE, ANGLE_BACK_LEFT);
			} else {
				res = servos().set_position(ID_SERVO_BACK_RIGHT_STORAGE, ANGLE_BACK_RIGHT);
			}
			break;
		}
	}

	return res;
}

ActionResult open_internal_storage() {
	logWarn("The internal storage isn't available");
	return ActionResult::FAILURE;
}

ActionResult close_internal_storage() {
	logWarn("The internal storage isn't available");
	return ActionResult::FAILURE;
}

double get_angle_by_id(int id) {
	return servos()->read_position(static_cast<uint8_t>(id)).toMinusPiPi().toDeg();
}

ActionResult set_angle_by_id(Angle angle, int id) {
	return servos().set_position(id, angle);
}
