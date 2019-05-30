#include "SimuNavigation.h"

#include <log/Log.h>

#include "../../robot/Modules/NavigationUtility.h"

using PhysicalRobot::NavigationUtility::angle_to_i32;
using PhysicalRobot::NavigationUtility::distance_to_i32;
using PhysicalRobot::NavigationUtility::i32_to_angle;
using PhysicalRobot::NavigationUtility::i32_to_distance;

SimuNavigation::SimuNavigation(uint8_t id, const std::shared_ptr<IRobotController>& robotController)
        : SimuModule(id, "Simulator Navigation"), _robotController(robotController) {}

void SimuNavigation::deactivation() {}

MovingCommand SimuNavigation::get_current_command() {
	return _command;
}

JSON SimuNavigation::getModuleState() {
	lock_variables();
	JSON json;
	repere::Coordinates coords = _robotController->getCoordinates();

	json["position"] = {};
	json["position"]["x"] = coords.getX().toM();
	json["position"]["y"] = coords.getY().toM();
	json["position"]["angle"] = coords.getAngle().toDeg();
	json["state"] = toString(_robotController->getState());
	unlock_variables();
	return json;
}

std::vector<JSON> SimuNavigation::generate_list_jsons() const {
	// Update from RobotController
	repere::Coordinates coords = _robotController->getCoordinates();
	auto state = _robotController->getState();

	JSON frame;

	// Only writeable values are sent (+ counter because Navigation modules needs it)
	frame["x"] = distance_to_i32(coords.getX());
	frame["y"] = distance_to_i32(coords.getY());
	frame["angle"] = angle_to_i32(coords.getAngle());
	frame["left_dist"] = 0;
	frame["right_dist"] = 0;
	frame["blocked"] = state == SimuRobotState::Blocked;
	frame["moving_done"] = state == SimuRobotState::Idle;

	frame["counter"] = _counter;

	return {frame};
}

void SimuNavigation::message_processing(const JSON& frame) {
	// Only readeable values are read
	_leds = frame["led"];
	_asserv_lin = frame["asserv_lin"];
	_asserv_lin = frame["asserv_ang"];
	_reset = frame["reset"];

	// Command parsing
	if(frame["counter"] > _counter) {
		// Stop current command
		if(_robotController->getState() != SimuRobotState::Idle) {
			_robotController->stop();
		}

		try {
			_command = fromString<MovingCommand>(frame["command"]);
		} catch(std::invalid_argument& e) {
			throw std::runtime_error("Navigation command failed to parse");
		}
		_args_cmd[0] = frame["args_cmd1"];
		_args_cmd[1] = frame["args_cmd2"];
		_counter = frame["counter"];

		// Execute next command
		Distance distance;
		Angle angle;

		auto robot_coords = _robotController->getCoordinates();
		int sign;

		switch(_command) {
			case MovingCommand::GoForward:
				distance = i32_to_distance(_args_cmd[0]);
				_robotController->forward(distance);
				logDebug6("Forward ", distance);
				break;

			case MovingCommand::GoBackward:
				distance = -i32_to_distance(_args_cmd[0]);
				_robotController->forward(distance);
				logDebug6("Backward ", -distance);
				break;

			case MovingCommand::TurnRelative:
				sign = _args_cmd[1] == 1 ? -1 : 1;
				angle = sign * i32_to_angle(_args_cmd[0]);
				_robotController->turnRelative(angle);
				logDebug6("Turn relative ", angle.toDeg(), " deg");
				break;

			case MovingCommand::TurnAbsolute:
				angle = (i32_to_angle(_args_cmd[0]) - robot_coords.getAngle()).toMinusPiPi();
				_robotController->turnRelative(angle);
				logDebug6("Turn absolute ", i32_to_angle(_args_cmd[0]).toDeg(), " deg");
				break;

			case MovingCommand::EmergencyStop:
			case MovingCommand::Stop:
				_robotController->stop();
				logDebug8("Stop");
				break;
			case MovingCommand::DoNothing:
				break;
		}
	}

	Module::_state_changed.exchange(true);
}
