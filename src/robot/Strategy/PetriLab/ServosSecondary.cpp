
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
} // namespace

void init_petri_servos(std::shared_ptr<RobotManager> manager, Constants::RobotColor color) {
	_manager = manager;
	_color = color;
}


ActionResult raise_arm(Angle angle) {
	return servos().set_position(ID_SERVO_RAISE_ARM, angle);
}

ActionResult rotate_arm(Angle angle) {
	return servos().set_position(ID_SERVO_ROTATE_ARM, angle);
}

ActionResult elevator(Angle angle) {
	return servos().set_position(ID_SERVO_ELEVATOR, angle);
}

ActionResult arm_ground() {
	std::vector<fun_ar> actions;

	ADD_FN(actions, rotate_arm, 0_deg);
	ADD_FN(actions, raise_arm, -50_deg);

	return _combine_actions(actions);
}

ActionResult arm_top_right() {
	std::vector<fun_ar> actions;

	ADD_FN(actions, raise_arm, 50_deg);
	ADD_FN(actions, rotate_arm, 20_deg);

	return _combine_actions(actions);
}

// Contrôle en vitesse ?
ActionResult raise_elevator() {
	return elevator(5200_deg);
}

ActionResult lower_elevator() {
	return elevator(-5300_deg);
}