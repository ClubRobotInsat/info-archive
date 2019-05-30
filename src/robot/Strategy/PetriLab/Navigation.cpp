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
		return GLOBAL_CONSTANTS().get_reference(_color);
	}

	const Duration ADD_TIMEOUT_SECURITY = 10_s;
} // namespace

void init_petri_navigation(std::shared_ptr<Strategy::Interfacer::RobotManager> manager, Constants::RobotColor color) {
	_manager = manager;
	_color = color;
}

Outcome forward(Distance distance, SensAdvance sens, Duration timeout) {
	logInfo("Forward ", distance, ", ", sens, ":");
	auto res = navigation().forward(distance, sens, timeout);
	if(res != Outcome::SUCCESS) {
		logError("Failure of Forward ", distance, ", ", sens, ": ", res);
	}
	return res;
}

Outcome forward(Distance distance, SensAdvance sens) {
	Duration timeout = distance / get_linear_speed() + ADD_TIMEOUT_SECURITY;
	return forward(distance, sens, timeout);
}

Outcome forward(Distance distance) {
	return forward(distance, SensAdvance::Forward);
}

Outcome forward(Distance distance, Duration duration) {
	return forward(distance, SensAdvance::Forward, duration);
}

Outcome backward(Distance distance) {
	return forward(distance, SensAdvance::Backward);
}

Outcome forward_infinity(Duration timeout) {
	return navigation().forward_infinity(SensAdvance::Forward, timeout);
}

Outcome backward_infinity(Duration timeout) {
	return navigation().forward_infinity(SensAdvance::Backward, timeout);
}

Outcome stop() {
	return navigation().stop();
}

Outcome emergency_stop() {
	return navigation().emergency_stop();
}

Outcome turn_relative(Angle angle) {
	logInfo("Relative Turn ", angle, ":");
	Duration timeout = abs(angle) / get_angular_speed() + ADD_TIMEOUT_SECURITY;
	auto res = navigation().turn_relative(angle, timeout, get_reference());
	if(res != Outcome::SUCCESS) {
		logError("Failure of Relative Turn ", angle, ": ", res);
	}
	return res;
}

Outcome turn_absolute(Angle angle) {
	logInfo("Absolute Turn ", angle, ":");
	Duration timeout = abs((angle - get_angle()).toMinusPiPi()) / get_angular_speed() + ADD_TIMEOUT_SECURITY;
	auto res = navigation().turn_absolute(repere::Orientation(angle, get_reference()), timeout);
	if(res != Outcome::SUCCESS) {
		logError("Failure of Absolute Turn ", angle, ": ", res);
	}
	return res;
}

Outcome turn_absolute(Angle angle, SensRotation sens) {
	logInfo("Absolute Turn ", angle, ", ", sens, ":");
	Duration timeout = abs(360_deg - (angle - get_angle()).toMinusPiPi()) / get_angular_speed() + ADD_TIMEOUT_SECURITY;
	auto res = navigation().turn_absolute(repere::Orientation(angle, get_reference()), sens, timeout);
	if(res != Outcome::SUCCESS) {
		logError("Failure of Absolute Turn ", angle, ", ", sens, ": ", res);
	}
	return res;
}

Outcome move_to(Distance x, Distance y, Angle final_angle) {
	return move_to_vec({x, y}, final_angle, SensAdvance::Forward);
}

Outcome move_to(Distance x, Distance y, Angle final_angle, SensAdvance sens) {
	return move_to_vec({x, y}, final_angle, sens);
}

Outcome move_to(Distance x, Distance y, Angle final_angle, SensAdvance sens, Duration timeout) {
	return move_to_vec({x, y}, final_angle, sens, timeout);
}

Outcome move_to_vec(Vector2m pos, Angle final_angle) {
	return move_to_vec(pos, final_angle, SensAdvance::Forward);
}

Outcome move_to_vec(Vector2m pos, Angle final_angle, SensAdvance sens, Duration timeout) {
	logInfo("Move To ", pos);
	set_slow_angular_speed();
	auto res = navigation().move_to(repere::Coordinates(pos, final_angle, get_reference()), sens, timeout);
	if(res != Outcome::SUCCESS && res != Outcome::POSITION_MODIFIED) {
		logError("Failure of Move To ", pos, ": ", res);
	}
	reset_angular_speed();
	return res;
}

Vector2m get_position() {
	return navigation()->get_position().getPos2D(get_reference());
}

Outcome set_position(Distance x, Distance y) {
	navigation()->set_coordinates(repere::Coordinates({x, y}, get_angle(), get_reference()));
	return Outcome::SUCCESS;
}

Angle get_angle() {
	return navigation()->get_orientation().getAngle(get_reference());
}

Outcome set_angle(Angle angle) {
	navigation()->set_coordinates(repere::Coordinates(get_position(), angle, get_reference()));
	return Outcome::SUCCESS;
}

Outcome set_coordinates(Distance x, Distance y, Angle angle) {
	navigation()->set_coordinates(repere::Coordinates({x, y}, angle, get_reference()));
	return Outcome::SUCCESS;
}

Distance get_distance_robot_position(Distance x, Distance y) {
	return (get_position() - repere::Position(x, y, get_reference()).getPos2D(get_reference())).norm();
}

Outcome recaling_top(SensAdvance sens) {
	return navigation().recaling_top(sens,
	                                 GLOBAL_CONSTANTS().get_table_size().y -
	                                     GLOBAL_CONSTANTS()[_manager->get_robot()->name].get_size().y / 2);
}

Outcome recaling_bottom(SensAdvance sens) {
	return navigation().recaling_bottom(sens, GLOBAL_CONSTANTS()[_manager->get_robot()->name].get_size().y / 2);
}

Outcome recaling_right(SensAdvance sens) {
	return navigation().recaling_right(sens,
	                                   GLOBAL_CONSTANTS().get_table_size().x -
	                                       GLOBAL_CONSTANTS()[_manager->get_robot()->name].get_size().y / 2);
}

Outcome recaling_left(SensAdvance sens) {
	return navigation().recaling_left(sens, GLOBAL_CONSTANTS()[_manager->get_robot()->name].get_size().y / 2);
}

Outcome activate_asserv() {
	navigation().activate_asserv();
	return Outcome::SUCCESS;
}

Outcome deactivate_asserv() {
	navigation().deactivate_asserv();
	return Outcome::SUCCESS;
}

Outcome set_fast_linear_speed() {
	try {
		navigation().push_linear_speed(get_linear_speed() * 2);
		return Outcome::SUCCESS;
	} catch(Strategy::Interfacer::NavigationInterfacer::ExceptionStack& e) {
		return Outcome::FAILURE;
	}
}

Outcome set_slow_linear_speed() {
	try {
		navigation().push_linear_speed(get_linear_speed() / 2);
		return Outcome::SUCCESS;
	} catch(Strategy::Interfacer::NavigationInterfacer::ExceptionStack& e) {
		return Outcome::FAILURE;
	}
}

Outcome set_linear_speed(Speed speed) {
	try {
		navigation().push_linear_speed(speed);
		return Outcome::SUCCESS;
	} catch(Strategy::Interfacer::NavigationInterfacer::ExceptionStack& e) {
		return Outcome::FAILURE;
	}
}

Outcome reset_linear_speed() {
	try {
		navigation().pop_linear_speed();
		return Outcome::SUCCESS;
	} catch(Strategy::Interfacer::NavigationInterfacer::ExceptionStack& e) {
		return Outcome::FAILURE;
	}
}

Speed get_linear_speed() {
	return navigation().get_linear_speed();
}

Outcome set_fast_angular_speed() {
	try {
		navigation().push_angular_speed(get_angular_speed() * 2);
		return Outcome::SUCCESS;
	} catch(Strategy::Interfacer::NavigationInterfacer::ExceptionStack& e) {
		return Outcome::FAILURE;
	}
}

Outcome set_slow_angular_speed() {
	try {
		navigation().push_angular_speed(get_angular_speed() / 2);
		return Outcome::SUCCESS;
	} catch(Strategy::Interfacer::NavigationInterfacer::ExceptionStack& e) {
		return Outcome::FAILURE;
	}
}

Outcome set_angular_speed(AngularSpeed speed) {
	try {
		navigation().push_angular_speed(speed);
		return Outcome::SUCCESS;
	} catch(Strategy::Interfacer::NavigationInterfacer::ExceptionStack& e) {
		return Outcome::FAILURE;
	}
}

Outcome reset_angular_speed() {
	try {
		navigation().pop_angular_speed();
		return Outcome::SUCCESS;
	} catch(Strategy::Interfacer::NavigationInterfacer::ExceptionStack& e) {
		return Outcome::FAILURE;
	}
}

AngularSpeed get_angular_speed() {
	return navigation().get_angular_speed();
}
