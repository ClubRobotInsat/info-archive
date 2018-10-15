//
// Created by terae on 17/09/18.
//

#include "Avoidance2019.h"
#include <log/Log.h>

namespace PhysicalRobot {
	Avoidance2019::Avoidance2019(uint8_t id)
	        : Module(id, avoidance_read_frame, avoidance_write_frame), _angle_detection_adversary(250_mrad) {}

	Angle Avoidance2019::get_angle_detection_adversary() const {
		std::lock_guard<std::mutex> lk(_mutex_variables);
		return _angle_detection_adversary;
	}

	void Avoidance2019::set_angle_detection_adversary(Angle angle) {
		if(abs(angle) <= 0.5_PI) {
			this->_angle_detection_adversary = angle;
		} else {
			logWarn("Angle de détection de l'adversaire trop grand ! ", angle);
		}
	}

	void Avoidance2019::set_position_turret(Vector2m position) {
		lock_variables();
		_position_turret = position;
		unlock_variables();
	}

	Vector2m Avoidance2019::get_position_turret() const {
		std::lock_guard<std::mutex> lk(_mutex_variables);
		return _position_turret;
	}

	bool Avoidance2019::is_adversary_detected() const {
		return _adversary_detected;
	}

	uint8_t Avoidance2019::get_frame_size() const {
		return get_size_avoidance_frame();
	}

	SharedAvoidance2019 Avoidance2019::generate_shared() const {
		Angle unwrapped = _angle_detection_adversary;
		SharedAvoidance2019 s = {static_cast<int>(unwrapped.toMilliRad()), static_cast<uint8_t>(_adversary_detected), 0};

		return s;
	}

	void Avoidance2019::message_processing(const SharedAvoidance2019& s) {
		if(s.parsing_failed == 0) {
			_angle_detection_adversary.exchange(Angle::makeFromMilliRad(s.angle_detection_adversary));
			_adversary_detected.exchange(static_cast<bool>(s.adversary_detected));
		}
	}
} // namespace PhysicalRobot