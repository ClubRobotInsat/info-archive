//
// Created by terae on 01/02/19.
//

#include "Navigation.h"
#include "../ModuleInterfacers/RobotManager.h"

namespace {
	std::shared_ptr<Strategy::Interfacer::RobotManager> _manager;
	Constants::RobotColor _color;

	Strategy::Interfacer::NavigationInterfacer& navigation() {
		return _manager->get_interfacer<Strategy::Interfacer::NavigationInterfacer>();
	}

	const repere::Repere& get_reference() {
		return navigation()->get_reference();
	}

	static const Duration ADD_TIMEOUT_SECURITY = 1_s;
} // namespace

void init_petri_navigation(std::shared_ptr<Strategy::Interfacer::RobotManager> manager, Constants::RobotColor color) {
	_manager = manager;
	_color = color;
}

ActionResult forward(Distance distance, SensAdvance sens) {
	logInfo("Forward ", distance, ", ", sens, ":");
	Duration timeout = distance / get_linear_speed() + ADD_TIMEOUT_SECURITY;
	auto res = navigation().forward(distance, sens, timeout);
	if(res != ActionResult::SUCCESS) {
		logError("Failure of Forward ", distance, ", ", sens, ": ", res);
	}
	return res;
}

ActionResult forward(Distance distance) {
	return forward(distance, SensAdvance::Forward);
}

ActionResult backward(Distance distance) {
	return forward(distance, SensAdvance::Backward);
}

ActionResult forward_infinity(Duration timeout) {
	return navigation().forward_infinity(SensAdvance::Forward, timeout);
}

ActionResult backward_infinity(Duration timeout) {
	return navigation().forward_infinity(SensAdvance::Backward, timeout);
}

ActionResult stop() {
	return navigation().stop();
}

ActionResult emergency_stop() {
	return navigation().emergency_stop();
}

ActionResult turn_relative(Angle angle) {
	logInfo("Relative Turn ", angle, ":");
	Duration timeout = abs(angle) / get_angular_speed() + ADD_TIMEOUT_SECURITY;
	auto res = navigation().turn_relative(angle, timeout);
	if(res != ActionResult::SUCCESS) {
		logError("Failure of Relative Turn ", angle, ": ", res);
	}
	return res;
}

ActionResult turn_absolute(Angle angle) {
	logInfo("Absolute Turn ", angle, ":");
	Duration timeout = abs((angle - get_angle()).toMinusPiPi()) / get_angular_speed() + ADD_TIMEOUT_SECURITY;
	auto res = navigation().turn_absolute(angle, timeout);
	if(res != ActionResult::SUCCESS) {
		logError("Failure of Absolute Turn ", angle, ": ", res);
	}
	return res;
}

ActionResult turn_absolute(Angle angle, SensRotation sens) {
	logInfo("Absolute Turn ", angle, ", ", sens, ":");
	Duration timeout = abs(360_deg - (angle - get_angle()).toMinusPiPi()) / get_angular_speed() + ADD_TIMEOUT_SECURITY;
	auto res = navigation().turn_absolute(angle, sens, timeout);
	if(res != ActionResult::SUCCESS) {
		logError("Failure of Absolute Turn ", angle, ", ", sens, ": ", res);
	}
	return res;
}

ActionResult move_to(Distance x, Distance y, Angle final_angle) {
	return move_to_vec({x, y}, final_angle, SensAdvance::Forward);
}

ActionResult move_to(Distance x, Distance y, Angle final_angle, SensAdvance sens) {
	return move_to_vec({x, y}, final_angle, sens);
}

ActionResult move_to(Distance x, Distance y, Angle final_angle, SensAdvance sens, Duration timeout) {
	return move_to_vec({x, y}, final_angle, sens, timeout);
}

ActionResult move_to_vec(Vector2m pos, Angle final_angle) {
	return move_to_vec(pos, final_angle, SensAdvance::Forward);
}

ActionResult move_to_vec(Vector2m pos, Angle final_angle, SensAdvance sens, Duration timeout) {
	logInfo("Move To ", pos);
	set_slow_angular_speed();
	auto res = navigation().move_to(repere::Coordinates(pos, final_angle, get_reference()), sens, timeout);
	if(res != ActionResult::SUCCESS && res != ActionResult::POSITION_MODIFIED) {
		logError("Failure of Move To ", pos, ": ", res);
	}
	reset_angular_speed();
	return res;
}

Vector2m get_position() {
	return navigation()->get_position().getPos2D(get_reference());
}

Angle get_angle() {
	return navigation()->get_orientation().getAngle(get_reference());
}

Distance get_distance_robot_position(Distance x, Distance y) {
	return (get_position() - repere::Position(x, y, get_reference()).getPos2D(get_reference())).norm();
}

ActionResult set_fast_linear_speed() {
	try {
		navigation().push_linear_speed(get_linear_speed() * 2);
		return ActionResult::SUCCESS;
	} catch(Strategy::Interfacer::NavigationInterfacer::ExceptionStack& e) {
		return ActionResult::FAILURE;
	}
}

ActionResult set_slow_linear_speed() {
	try {
		navigation().push_linear_speed(get_linear_speed() / 2);
		return ActionResult::SUCCESS;
	} catch(Strategy::Interfacer::NavigationInterfacer::ExceptionStack& e) {
		return ActionResult::FAILURE;
	}
}

ActionResult set_linear_speed(Speed speed) {
	try {
		navigation().push_linear_speed(speed);
		return ActionResult::SUCCESS;
	} catch(Strategy::Interfacer::NavigationInterfacer::ExceptionStack& e) {
		return ActionResult::FAILURE;
	}
}

ActionResult reset_linear_speed() {
	try {
		navigation().pop_linear_speed();
		return ActionResult::SUCCESS;
	} catch(Strategy::Interfacer::NavigationInterfacer::ExceptionStack& e) {
		return ActionResult::FAILURE;
	}
}

Speed get_linear_speed() {
	return navigation().get_linear_speed();
}

ActionResult set_fast_angular_speed() {
	try {
		navigation().push_angular_speed(get_angular_speed() * 2);
		return ActionResult::SUCCESS;
	} catch(Strategy::Interfacer::NavigationInterfacer::ExceptionStack& e) {
		return ActionResult::FAILURE;
	}
}

ActionResult set_slow_angular_speed() {
	try {
		navigation().push_angular_speed(get_angular_speed() / 2);
		return ActionResult::SUCCESS;
	} catch(Strategy::Interfacer::NavigationInterfacer::ExceptionStack& e) {
		return ActionResult::FAILURE;
	}
}

ActionResult set_angular_speed(AngularSpeed speed) {
	try {
		navigation().push_angular_speed(speed);
		return ActionResult::SUCCESS;
	} catch(Strategy::Interfacer::NavigationInterfacer::ExceptionStack& e) {
		return ActionResult::FAILURE;
	}
}

ActionResult reset_angular_speed() {
	try {
		navigation().pop_angular_speed();
		return ActionResult::SUCCESS;
	} catch(Strategy::Interfacer::NavigationInterfacer::ExceptionStack& e) {
		return ActionResult::FAILURE;
	}
}

AngularSpeed get_angular_speed() {
	return navigation().get_angular_speed();
}
