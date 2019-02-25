#include "SimuNavigation.h"

#include "../../robot/Modules/NavigationUtility.h"

using PhysicalRobot::NavigationUtility::angle_to_u16;
using PhysicalRobot::NavigationUtility::distance_to_u16;
using PhysicalRobot::NavigationUtility::u16_to_angle;
using PhysicalRobot::NavigationUtility::u16_to_distance;

SimuNavigation::SimuNavigation(uint8_t id) : Module(id, "Simulator Navigation") {}

void SimuNavigation::deactivation() {}

MovingCommand SimuNavigation::get_current_command() {
	return _command;
}

std::vector<JSON> SimuNavigation::generate_list_jsons() const {
	JSON frame;

	// Only writeable values are sent
	frame["x"] = distance_to_u16(_coords.getX());
	frame["y"] = distance_to_u16(_coords.getY());
	frame["angle"] = angle_to_u16(_coords.getAngle());
	frame["blocked"] = _blocked.load();
	frame["moving_done"] = _moving_done.load();

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
		if(!_moving_done) {
			_robotController->stop();
		}

		_command = frame["command"];
		_args_cmd[0] = frame["args_cmd1"];
		_args_cmd[1] = frame["args_cmd2"];
		_counter = frame["counter"];

		// Execute next command
		Distance distance = 0_m;
		Angle angle = 0_deg;

		switch(_command) {
			case MovingCommand::GoForward:
				distance = u16_to_distance(_args_cmd[0]);
			case MovingCommand::GoBackward:
				distance = -u16_to_distance(_args_cmd[0]);
				_robotController->forward(distance);
				break;

			case MovingCommand::TurnRelative:
				// angle = _robotController->getAngle() + u16_to_angle(_args_cmd[0]);
			case MovingCommand::TurnAbsolute:
				angle = u16_to_angle(_args_cmd[0]);
				_robotController->turn(angle);
				break;

			case MovingCommand::EmergencyStop:
			case MovingCommand::Stop:
				_robotController->stop();
				break;
			case MovingCommand::DoNothing:
				break;
		}
	}
}
