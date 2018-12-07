//
// Created by terae on 17/09/18.
//

#include "Avoidance.h"
#include <log/Log.h>

namespace PhysicalRobot {
	Avoidance::Avoidance(uint8_t id) : Module(id), _angle_detection_adversary(250_mrad) {}

	Angle Avoidance::get_angle_detection_adversary() const {
		std::lock_guard<std::mutex> lk(_mutex_variables);
		return _angle_detection_adversary;
	}

	void Avoidance::set_angle_detection_adversary(Angle angle) {
		if(abs(angle) <= 0.5_PI) {
			this->_angle_detection_adversary = angle;
			_state_changed.exchange(true);
		} else {
			logWarn("Angle de détection de l'adversaire trop grand ! ", angle);
		}
	}

	void Avoidance::set_position_turret(Vector2m position) {
		lock_variables();
		_position_turret = position;
		_state_changed.exchange(true);
		unlock_variables();
	}

	Vector2m Avoidance::get_position_turret() const {
		std::lock_guard<std::mutex> lk(_mutex_variables);
		return _position_turret;
	}

	bool Avoidance::is_adversary_detected() const {
		return _adversary_detected;
	}

	std::vector<JSON> Avoidance::generate_list_jsons() const {
		JSON j;
		j["angle_detection"] = static_cast<int>(_angle_detection_adversary.load().toMilliRad());
		j["detected"] = false;

		return {j};
	}

	void Avoidance::message_processing(const JSON& j) {
		_angle_detection_adversary.exchange(Angle::makeFromMilliRad(j["angle_detection"]));
		_adversary_detected.exchange(j["detected"]);
	}
} // namespace PhysicalRobot