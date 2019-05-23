//
// Created by terae on 16/09/18.
//

#include "Navigation.h"

#include "NavigationUtility.h"

namespace PhysicalRobot {

	using NavigationUtility::angle_to_i32;
	using NavigationUtility::distance_to_i32;
	using NavigationUtility::i32_to_angle;
	using NavigationUtility::i32_to_distance;

	void Navigation::forward(Distance distance, SensAdvance sens) {
		lock_variables();
		if(sens == SensAdvance::Forward) {
			set_command(MovingCommand::GoForward);
		} else {
			set_command(MovingCommand::GoBackward);
		}
		_args_cmd[0] = uint16_t(distance_to_i32(distance));
		_state_changed.exchange(true);
		unlock_variables();
	}

	void Navigation::turn_absolute(Angle angle, SensRotation sens) {
		lock_variables();
		set_command(MovingCommand::TurnAbsolute);
		_args_cmd[0] = uint16_t(angle_to_i32(angle));

		if(sens == SensRotation::Trigo) {
			_args_cmd[1] = 0;
		} else {
			_args_cmd[1] = 1;
		}

		_state_changed.exchange(true);
		unlock_variables();
	}

	void Navigation::turn_relative(Angle angle) {
		lock_variables();
		set_command(MovingCommand::TurnRelative);

		if(angle > 0_deg) {
			_args_cmd[0] = uint16_t(angle_to_i32(angle));
			_args_cmd[1] = 0;
		} else {
			_args_cmd[0] = uint16_t(angle_to_i32(-angle));
			_args_cmd[1] = 1;
		}
		_state_changed.exchange(true);
		unlock_variables();
	}

	void Navigation::stop() {
		lock_variables();
		set_command(MovingCommand::Stop);
		_state_changed.exchange(true);
		unlock_variables();
	}

	void Navigation::emergency_stop() {
		lock_variables();
		set_command(MovingCommand::EmergencyStop);
		_state_changed.exchange(true);
		unlock_variables();
	}

	bool Navigation::is_physically_blocked() const {
		std::lock_guard<std::mutex> lk(_mutex_variables);
		return _blocked;
	}

	repere::Coordinates Navigation::get_coordinates() const {
		std::lock_guard<std::mutex> lk(_mutex_variables);
		return _coords;
	}

	repere::Position Navigation::get_position() const {
		return get_coordinates().getPosition();
	}

	repere::Orientation Navigation::get_orientation() const {
		return get_coordinates().getOrientation();
	}

	Distance Navigation::get_left_wheel_distance() const {
		std::lock_guard<std::mutex> lk(_mutex_variables);
		return _left_dist;
	}

	Distance Navigation::get_right_wheel_distance() const {
		std::lock_guard<std::mutex> lk(_mutex_variables);
		return _right_dist;
	}

	const repere::Repere& Navigation::get_reference() const {
		return Navigation::REFERENCE;
	}

	void Navigation::set_coordinates(const repere::Coordinates& coords) {
		lock_variables();
		_reset = true;
		_coords = coords;
		_state_changed.exchange(true);
		unlock_variables();
	}

	void Navigation::update_linear_speed(Speed speed) {
		lock_variables();
		_linear_speed = speed;
		_state_changed.exchange(true);
		unlock_variables();
	}

	void Navigation::update_angular_speed(AngularSpeed angular_speed) {
		lock_variables();
		_angular_speed = angular_speed;
		_state_changed.exchange(true);
		unlock_variables();
	}

	void Navigation::update_linear_accuracy(Distance accuracy) {
		lock_variables();
		_linear_accuracy = accuracy;
		_state_changed.exchange(true);
		unlock_variables();
	}

	void Navigation::update_angular_accuracy(Angle accuracy) {
		lock_variables();
		_angular_accuracy = accuracy;
		_state_changed.exchange(true);
		unlock_variables();
	}

	bool Navigation::is_moving_done() const {
		std::lock_guard<std::mutex> lk(_mutex_variables);
		return _moving_done;
	}

	bool Navigation::is_precision_reached() const {
		std::lock_guard<std::mutex> lk(_mutex_variables);
		// Pas de distinction pour l'instant.
		return _moving_done;
	}

	void Navigation::set_linear_asserv_enabled(bool activated) {
		lock_variables();
		_asserv_lin = activated;
		_state_changed.exchange(true);
		unlock_variables();
	}

	void Navigation::set_angular_asserv_enabled(bool activated) {
		lock_variables();
		_asserv_ang = activated;
		_state_changed.exchange(true);
		unlock_variables();
	}

	void Navigation::set_asserv_enabled(bool activated) {
		lock_variables();
		_asserv_lin = activated;
		_asserv_ang = activated;
		_state_changed.exchange(true);
		unlock_variables();
	}

	std::vector<JSON> Navigation::generate_list_jsons() const {
		JSON moving;
		moving["x"] = distance_to_i32(_coords.getX());
		moving["y"] = distance_to_i32(_coords.getY());
		moving["angle"] = angle_to_i32(_coords.getAngle());
		moving["left_dist"] = distance_to_i32(_left_dist);
		moving["right_dist"] = distance_to_i32(_right_dist);
		moving["blocked"] = _blocked.load();
		moving["moving_done"] = _moving_done.load();

		moving["asserv_lin"] = _asserv_lin.load();
		moving["asserv_ang"] = _asserv_ang.load();
		moving["led"] = _leds.load();
		moving["reset"] = _reset.load();

		moving["command"] = toString(_command);
		moving["args_cmd1"] = _args_cmd[0];
		moving["args_cmd2"] = _args_cmd[1];
		moving["counter"] = _counter;

		moving["max_lin_speed"] = NavigationUtility::speed_to_u16(_linear_speed);
		moving["max_ang_speed"] = NavigationUtility::angular_speed_to_u16(_angular_speed);
		moving["lin_accuracy"] = uint16_t(NavigationUtility::distance_to_i32(_linear_accuracy));
		moving["ang_accuracy"] = uint16_t(NavigationUtility::angle_to_i32(_angular_accuracy));
		return {moving};
	}

	void Navigation::message_processing(const JSON& json) {
		if(json_has_fields(json, {"x", "y", "angle", "left_dist", "right_dist", "blocked", "counter", "moving_done"})) {
			_coords = repere::Coordinates({i32_to_distance(json["x"]), i32_to_distance(json["y"])}, i32_to_angle(json["angle"]));
			_left_dist = i32_to_distance(json["left_dist"]);
			_right_dist = i32_to_distance(json["right_dist"]);

			_blocked = json["blocked"];

			// moving_done est valide seulement si ils parlent
			// de la commande actuelle.
			if(json["counter"] == _counter) {
				_moving_done = json["moving_done"];
			}
		}
	}

	void Navigation::deactivation() {
		lock_variables();
		set_command(MovingCommand::EmergencyStop);
		_state_changed.exchange(true);
		// TODO : arrêt d'urgence + arrêter l'asservissement
		unlock_variables();
	}

	void Navigation::set_command(MovingCommand command) {
		_command = command;
		++_counter;
		_moving_done = false;
	}

} // namespace PhysicalRobot
