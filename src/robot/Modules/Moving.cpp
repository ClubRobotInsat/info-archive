//
// Created by terae on 16/09/18.
//

#include "Moving.h"

namespace PhysicalRobot {
	Moving2019::Moving2019(uint8_t id) : Module(id, moving_read_frame, moving_write_frame) {}

	Speed Moving2019::get_linear_speed() const {
		std::lock_guard<std::mutex> lk(_mutex_variables);
		return _linear_speed;
	}

	AngularSpeed Moving2019::get_angular_speed() const {
		std::lock_guard<std::mutex> lk(_mutex_variables);
		return _angular_speed;
	}

	void Moving2019::set_linear_speed(Speed speed) {
		lock_variables();
		_linear_speed = speed;
		unlock_variables();
	}

	void Moving2019::set_angular_speed(AngularSpeed speed) {
		lock_variables();
		_angular_speed = speed;
		unlock_variables();
	}

	Coordonnees Moving2019::get_coordonnees() const {
		std::lock_guard<std::mutex> lk(_mutex_variables);
		return _coords;
	}

	uint8_t Moving2019::get_frame_size() const {
		return get_size_moving_frame();
	}

	SharedMoving2019 Moving2019::generate_shared() const {
		SharedMoving2019 s = {};
		s.parsing_failed = 1;
		return s;
	}

	void Moving2019::message_processing(const SharedMoving2019&) {}

	void Moving2019::deactivation() {
		// TODO : arrêt d'urgence + arrêter l'asservissement
	}
} // namespace PhysicalRobot