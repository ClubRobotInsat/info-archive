
#include "ServosSecondary.h"
#include "../ModuleInterfacers/RobotManager.h"

namespace {
	using namespace Strategy::Interfacer;

	std::shared_ptr<RobotManager> _manager;
	Constants::RobotColor _color;

	ServosInterfacer& servos() {
		return _manager->get_interfacer<ServosInterfacer>();
	}

	const ServosInterfacer::servo_t ID_SERVO_RAISE_ARM = 1;
	const ServosInterfacer::servo_t ID_SERVO_ROTATE_ARM = 2;
	const ServosInterfacer::servo_t ID_SERVO_ELEVATOR = 3;

	ElevatorPosition elevator_position;
} // namespace

void init_petri_servos_secondary(std::shared_ptr<RobotManager> manager, Constants::RobotColor color) {
	_manager = manager;
	_color = color;
}


Outcome raise_arm(Angle angle) {
	return servos().set_position(ID_SERVO_RAISE_ARM, angle);
}

Outcome rotate_arm(Angle angle) {
	return servos().set_position(ID_SERVO_ROTATE_ARM, angle);
}

Outcome elevator(Angle angle) {
	return servos().set_position(ID_SERVO_ELEVATOR, angle);
}

Outcome arm_ground() {
	std::vector<fun_ar> actions;

	ADD_FN(actions, rotate_arm, 0_deg);
	ADD_FN(actions, raise_arm, -50_deg);

	elevator_position = ElevatorPosition::GROUND;

	return _combine_actions(actions);
}

Outcome arm_top_right() {
	std::vector<fun_ar> actions;

	ADD_FN(actions, raise_arm, 50_deg);
	ADD_FN(actions, rotate_arm, 20_deg);

	elevator_position = ElevatorPosition::TOP_RIGHT;

	return _combine_actions(actions);
}

Outcome arm_top_left() {
	std::vector<fun_ar> actions;

	ADD_FN(actions, raise_arm, 50_deg);
	ADD_FN(actions, rotate_arm, 20_deg);

	elevator_position = ElevatorPosition::TOP_LEFT;

	return _combine_actions(actions);
}

// TODO : Contrôle en vitesse ?
Outcome raise_elevator() {
	return elevator(5200_deg);
}

Outcome lower_elevator() {
	return elevator(-5300_deg);
}

ElevatorPosition get_elevator_position() {
	return elevator_position;
}
