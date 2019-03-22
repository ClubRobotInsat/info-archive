#include "SimuNavigation.h"

#include <log/Log.h>

#include "../../robot/Modules/NavigationUtility.h"

using PhysicalRobot::NavigationUtility::angle_to_u16;
using PhysicalRobot::NavigationUtility::distance_to_u16;
using PhysicalRobot::NavigationUtility::u16_to_angle;
using PhysicalRobot::NavigationUtility::u16_to_distance;

SimuNavigation::SimuNavigation(uint8_t id, const std::shared_ptr<IRobotController>& robotController)
        : Module(id, "Simulator Navigation"), _robotController(robotController) {}

void SimuNavigation::deactivation() {}

MovingCommand SimuNavigation::get_current_command() {
	return _command;
}

std::vector<JSON> SimuNavigation::generate_list_jsons() const {
	// Update from RobotController
	repere::Coordinates _coords = _robotController->getCoordinates();
	auto state = _robotController->getState();

	JSON frame;

	// Only writeable values are sent (+ counter because Navigation modules needs it)
	frame["x"] = distance_to_u16(_coords.getX());
	frame["y"] = distance_to_u16(_coords.getY());
	frame["angle"] = angle_to_u16(_coords.getAngle());
	frame["blocked"] = state == SimuRobotState::Blocked;
	frame["moving_done"] = state == SimuRobotState::Idle;

	frame["counter"] = _counter;

	return {frame};
}

void SimuNavigation::message_processing(const JSON& frame) {
	// Only readeable values are read
	_leds = frame["led"];
	_asserv_on_off = frame["asserv_on_off"];
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

		switch(_command) {
			case MovingCommand::GoForward:
				distance = u16_to_distance(_args_cmd[0]);
				_robotController->forward(distance);
				logDebug6("Forward ", distance);
				break;

			case MovingCommand::GoBackward:
				distance = -u16_to_distance(_args_cmd[0]);
				_robotController->forward(distance);
				logDebug6("Backward ", -distance);
				break;

			case MovingCommand::TurnRelative:
				angle = robot_coords.getAngle() + u16_to_angle(_args_cmd[0]);
				_robotController->turn(angle);
				logDebug6("Turn relative ", u16_to_angle(_args_cmd[0]));
				break;

			case MovingCommand::TurnAbsolute:
				angle = u16_to_angle(_args_cmd[0]);
				_robotController->turn(angle);
				logDebug6("Turn absolute ", angle);
				break;

			case MovingCommand::EmergencyStop:
			case MovingCommand::Stop:
				_robotController->stop();
				logDebug6("Stop");
				break;
			case MovingCommand::DoNothing:
				break;
		}
	}

	Module::_state_changed.exchange(true);
}
