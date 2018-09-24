//
// Created by terae on 9/10/18.
//

#include "Motors2019.h"

namespace Commun {
	uint8_t Motors2019::get_nbr_controlled() const {
		uint8_t count = 0;
		for(uint8_t i = 0; i < NB_MAX_CONTROLLED_MOTORS; ++i) {
			count += (_controlled[i] != nullptr);
		}
		return count;
	}

	uint8_t Motors2019::get_nbr_uncontrolled() const {
		uint8_t count = 0;
		for(uint8_t i = 0; i < NB_MAX_UNCONTROLLED_MOTORS; ++i) {
			count += (_uncontrolled[i] != nullptr);
		}
		return count;
	}

	uint8_t Motors2019::get_nbr_brushless() const {
		uint8_t count = 0;
		for(uint8_t i = 0; i < NB_MAX_BRUSHLESS; ++i) {
			count += (_brushless[i] != nullptr);
		}
		return count;
	}

	void Motors2019::position_angle(uint8_t id, Angle angle) {
		test_is_controlled_ok(id);

		lock_variables();
		_controlled[id]->wanted_position = angle;
		_controlled[id]->finished = false;
		unlock_variables();
	}

	void Motors2019::position_turns(uint8_t id, uint8_t nb_turns, RotatingDirection rotation) {
		test_is_controlled_ok(id);

		lock_variables();
		_controlled[id]->wanted_nb_turns = nb_turns;
		_controlled[id]->rotation = rotation;
		_controlled[id]->finished = false;
		unlock_variables();
	}

	bool Motors2019::is_position_finished(uint8_t id) const {
		test_is_controlled_ok(id);
		std::lock_guard<std::mutex> lk(_mutex_variables);
		return _controlled[id]->finished;
	}

	void Motors2019::activate_uncontrolled_motor(uint8_t id, RotatingDirection rotation) {
		test_is_uncontrolled_ok(id);

		lock_variables();
		_uncontrolled[id]->on_off = true;
		_uncontrolled[id]->rotation = rotation;
		unlock_variables();
	}

	void Motors2019::deactivate_uncontrolled_motor(uint8_t id) {
		test_is_uncontrolled_ok(id);

		lock_variables();
		_uncontrolled[id]->on_off = false;
		unlock_variables();
	}

	void Motors2019::activate_brushless(uint8_t id) {
		test_is_brushless_ok(id);

		lock_variables();
		_brushless[id]->on_off = true;
		unlock_variables();
	}

	void Motors2019::deactivate_brushless(uint8_t id) {
		test_is_brushless_ok(id);

		lock_variables();
		_brushless[id]->on_off = false;
		unlock_variables();
	}

	bool Motors2019::test_is_controlled_ok(uint8_t id) const {
		// 'id == 0' veut dire qu'il n'y a pas de moteur dans la représentation C
		bool result = id > 0 && id < NB_MAX_CONTROLLED_MOTORS && static_cast<bool>(_controlled[id]);
		if(!result) {
			throw std::runtime_error("Numéro du moteur asservi demandé invalide : "s + std::to_string(id));
		}
		return result;
	}

	bool Motors2019::test_is_uncontrolled_ok(uint8_t id) const {
		// 'id == 0' veut dire qu'il n'y a pas de moteur dans la représentation C
		bool result = id > 0 && id < NB_MAX_UNCONTROLLED_MOTORS && static_cast<bool>(_uncontrolled[id]);
		if(!result) {
			throw std::runtime_error("Numéro du moteur non-asservi demandé invalide : "s + std::to_string(id));
		}
		return result;
	}

	bool Motors2019::test_is_brushless_ok(uint8_t id) const {
		// 'id == 0' veut dire qu'il n'y a pas de moteur dans la représentation C
		bool result = id > 0 && id < NB_MAX_BRUSHLESS && static_cast<bool>(_brushless[id]);
		if(!result) {
			throw std::runtime_error("Numéro du brushless demandé invalide : "s + std::to_string(id));
		}
		return result;
	}

	uint8_t Motors2019::get_frame_size() const {
		return static_cast<uint8_t>(3 + get_nbr_controlled() * 4 + get_nbr_uncontrolled() * 2 + get_nbr_brushless() * 2);
	}

	void Motors2019::add_controlled_motor(uint8_t id, RotatingDirection rotation) {
		if(id >= NB_MAX_CONTROLLED_MOTORS) {
			throw std::runtime_error("ID du moteur asservi trop grand ("s + std::to_string(id) + " > " +
			                         std::to_string(NB_MAX_CONTROLLED_MOTORS) + ") !");
		} else if(id == 0) {
			throw std::runtime_error("L'ID 0 des moteurs est réservé !");
		} else if(_controlled[id]) {
			throw std::runtime_error("Double assignation du moteur asservi "s + std::to_string(id) + " !");
		}

		_controlled[id] = std::make_unique<ControlledMotor>(id, rotation);
	}

	void Motors2019::add_uncontrolled_motor(uint8_t id) {
		if(id >= NB_MAX_UNCONTROLLED_MOTORS) {
			throw std::runtime_error("ID du moteur non-asservi trop grand ("s + std::to_string(id) + " > " +
			                         std::to_string(NB_MAX_UNCONTROLLED_MOTORS) + ") !");
		} else if(id == 0) {
			throw std::runtime_error("L'ID 0 des moteurs est réservé !");
		} else if(_uncontrolled[id]) {
			throw std::runtime_error("Double assignation du moteur non-asservi "s + std::to_string(id) + " !");
		}

		_uncontrolled[id] = std::make_unique<UncontrolledMotor>(id);
	}

	void Motors2019::add_brushless(uint8_t id) {
		if(id >= NB_MAX_BRUSHLESS) {
			throw std::runtime_error("ID du brushless trop grand ("s + std::to_string(id) + " > " +
			                         std::to_string(NB_MAX_BRUSHLESS) + ") !");
		} else if(id == 0) {
			throw std::runtime_error("L'ID 0 des moteurs est réservé !");
		} else if(_brushless[id]) {
			throw std::runtime_error("Double assignation du brushless "s + std::to_string(id) + " !");
		}

		_brushless[id] = std::make_unique<Brushless>(id);
	}

	SharedMotors2019 Motors2019::generate_shared() const {
		SharedMotors2019 s = {};
		for(uint8_t i = 0; i < NB_MAX_CONTROLLED_MOTORS; ++i) {
			if(_controlled[i]) {
				s.controlled_motors[i].id = i;

				// TODO

			} else {
				s.controlled_motors[i].id = 0;
			}
		}

		// TODO : uncontrolled & brushless

		s.parsing_failed = 0;
		return s;
	}

	void Motors2019::message_processing(const SharedMotors2019& s) {
		if(s.parsing_failed == 0) {
			// TODO
		}
	}

	void Motors2019::deactivation() {
		lock_variables();
		for(uint8_t id = 0; id < NB_MAX_CONTROLLED_MOTORS; ++id) {
			if(_controlled[id]) {
				_controlled[id]->finished = true;
				_controlled[id]->wanted_nb_turns = 0;
				_controlled[id]->wanted_position = 0_deg;
			}
		}
		unlock_variables();

		for(uint8_t id = 0; id < NB_MAX_UNCONTROLLED_MOTORS; ++id) {
			if(_uncontrolled[id]) {
				deactivate_uncontrolled_motor(id);
			}
		}

		for(uint8_t id = 0; id < NB_MAX_BRUSHLESS; ++id) {
			if(_brushless[id]) {
				deactivate_brushless(id);
			}
		}
	}
} // namespace Commun