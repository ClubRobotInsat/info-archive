//
// Created by terae on 9/10/18.
//

#include "Motors2019.h"

namespace PhysicalRobot {
	uint8_t Motors2019::get_nbr_controlled() const {
		uint8_t count = 0;
		for(uint8_t i = 0; i < ID_MAX_CONTROLLED_MOTORS; ++i) {
			count += (_controlled[i] != nullptr);
		}
		return count;
	}

	uint8_t Motors2019::get_nbr_uncontrolled() const {
		uint8_t count = 0;
		for(uint8_t i = 0; i < ID_MAX_UNCONTROLLED_MOTORS; ++i) {
			count += (_uncontrolled[i] != nullptr);
		}
		return count;
	}

	uint8_t Motors2019::get_nbr_brushless() const {
		uint8_t count = 0;
		for(uint8_t i = 0; i < ID_MAX_BRUSHLESS; ++i) {
			count += (_brushless[i] != nullptr);
		}
		return count;
	}

	void Motors2019::position_angle(uint8_t id, Angle angle) {
		uint8_t index = test_is_controlled_ok(id);

		lock_variables();
		_controlled[index]->wanted_position = angle;
		_controlled[index]->finished = false;
		unlock_variables();
	}

	void Motors2019::position_turns(uint8_t id, uint8_t nb_turns, RotatingDirection rotation) {
		uint8_t index = test_is_controlled_ok(id);

		lock_variables();
		_controlled[index]->wanted_nb_turns = nb_turns;
		_controlled[index]->rotation = rotation;
		_controlled[index]->finished = false;
		unlock_variables();
	}

	bool Motors2019::is_position_finished(uint8_t id) const {
		uint8_t index = test_is_controlled_ok(id);
		std::lock_guard<std::mutex> lk(_mutex_variables);
		return _controlled[index]->finished;
	}

	void Motors2019::activate_uncontrolled_motor(uint8_t id, RotatingDirection rotation) {
		uint8_t index = test_is_uncontrolled_ok(id);

		lock_variables();
		_uncontrolled[index]->on_off = true;
		_uncontrolled[index]->rotation = rotation;
		unlock_variables();
	}

	void Motors2019::deactivate_uncontrolled_motor(uint8_t id) {
		uint8_t index = test_is_uncontrolled_ok(id);

		lock_variables();
		_uncontrolled[index]->on_off = false;
		unlock_variables();
	}

	void Motors2019::activate_brushless(uint8_t id) {
		uint8_t index = test_is_brushless_ok(id);

		lock_variables();
		_brushless[index]->on_off = true;
		unlock_variables();
	}

	void Motors2019::deactivate_brushless(uint8_t id) {
		uint8_t index = test_is_brushless_ok(id);

		lock_variables();
		_brushless[index]->on_off = false;
		unlock_variables();
	}

	uint8_t Motors2019::test_is_controlled_ok(uint8_t id) const {
		uint8_t index = get_index_of_controlled(id);
		if(index >= ID_MAX_CONTROLLED_MOTORS) {
			throw std::runtime_error("Numéro du moteur asservi demandé invalide : "s + std::to_string(id));
		}

		return index;
	}

	uint8_t Motors2019::test_is_uncontrolled_ok(uint8_t id) const {
		uint8_t index = get_index_of_uncontrolled(id);
		if(index >= ID_MAX_UNCONTROLLED_MOTORS) {
			throw std::runtime_error("Numéro du moteur non-asservi demandé invalide : "s + std::to_string(id));
		}

		return index;
	}

	uint8_t Motors2019::test_is_brushless_ok(uint8_t id) const {
		uint8_t index = get_index_of_brushless(id);
		if(index >= ID_MAX_BRUSHLESS) {
			throw std::runtime_error("Numéro du brushless demandé invalide : "s + std::to_string(id));
		}

		return index;
	}

	uint8_t Motors2019::get_index_of_controlled(uint8_t id) const {
		const uint8_t INDEX_BAD_ID = ID_MAX_CONTROLLED_MOTORS;
		std::lock_guard<std::mutex> lk(_mutex_variables);

		// 'id == 0' veut dire qu'il n'y a pas de moteur dans la représentation C
		if(id == 0)
			return INDEX_BAD_ID;

		for(uint8_t index = 0; index < ID_MAX_CONTROLLED_MOTORS; ++index) {
			if(_controlled[index] && _controlled[index]->id == id)
				return index;
		}

		return INDEX_BAD_ID;
	}

	uint8_t Motors2019::get_index_of_uncontrolled(uint8_t id) const {
		const uint8_t INDEX_BAD_ID = ID_MAX_UNCONTROLLED_MOTORS;
		std::lock_guard<std::mutex> lk(_mutex_variables);

		// 'id == 0' veut dire qu'il n'y a pas de moteur dans la représentation C
		if(id == 0)
			return INDEX_BAD_ID;

		for(uint8_t index = 0; index < ID_MAX_UNCONTROLLED_MOTORS; ++index) {
			if(_uncontrolled[index] && _uncontrolled[index]->id == id)
				return index;
		}

		return INDEX_BAD_ID;
	}

	uint8_t Motors2019::get_index_of_brushless(uint8_t id) const {
		const uint8_t INDEX_BAD_ID = ID_MAX_BRUSHLESS;
		std::lock_guard<std::mutex> lk(_mutex_variables);

		// 'id == 0' veut dire qu'il n'y a pas de moteur dans la représentation C
		if(id == 0)
			return INDEX_BAD_ID;

		for(uint8_t index = 0; index < ID_MAX_BRUSHLESS; ++index) {
			if(_brushless[index] && _brushless[index]->id == id)
				return index;
		}

		return INDEX_BAD_ID;
	}

	uint8_t Motors2019::get_frame_size() const {
		return get_size_motor_frame(get_nbr_controlled(), get_nbr_uncontrolled(), get_nbr_brushless());
	}

	void Motors2019::add_controlled_motor(uint8_t id, RotatingDirection rotation) {
		if(get_nbr_controlled() >= ID_MAX_CONTROLLED_MOTORS) {
			throw std::runtime_error("Trop de moteurs asservis ont été défini !");
		} else if(id == 0) {
			throw std::runtime_error("L'ID 0 des moteurs est réservé !");
		} else if(get_index_of_controlled(id) < ID_MAX_CONTROLLED_MOTORS) {
			throw std::runtime_error("Double assignation du moteur asservi "s + std::to_string(id) + " !");
		}

		for(uint8_t index = 0; index < ID_MAX_CONTROLLED_MOTORS; ++index) {
			if(_controlled[index] == nullptr) {
				_controlled[index] = std::make_unique<ControlledMotor>(id, rotation);
				break;
			}
		}
	}

	void Motors2019::add_uncontrolled_motor(uint8_t id) {
		if(get_nbr_uncontrolled() >= ID_MAX_UNCONTROLLED_MOTORS) {
			throw std::runtime_error("Trop de moteurs non-asservis ont été défini !");
		} else if(id == 0) {
			throw std::runtime_error("L'ID 0 des moteurs est réservé !");
		} else if(get_index_of_uncontrolled(id) < ID_MAX_UNCONTROLLED_MOTORS) {
			throw std::runtime_error("Double assignation du moteur non-asservi "s + std::to_string(id) + " !");
		}

		for(uint8_t index = 0; index < ID_MAX_CONTROLLED_MOTORS; ++index) {
			if(_uncontrolled[index] == nullptr) {
				_uncontrolled[id] = std::make_unique<UncontrolledMotor>(id);
				break;
			}
		}
	}

	void Motors2019::add_brushless(uint8_t id) {
		if(get_nbr_brushless() >= ID_MAX_BRUSHLESS) {
			throw std::runtime_error("Trop de brushless ont été défini !");
		} else if(id == 0) {
			throw std::runtime_error("L'ID 0 des moteurs est réservé !");
		} else if(_brushless[id]) {
			throw std::runtime_error("Double assignation du brushless "s + std::to_string(id) + " !");
		}

		for(uint8_t index = 0; index < ID_MAX_BRUSHLESS; ++index) {
			if(_brushless[index] == nullptr) {
				_brushless[id] = std::make_unique<Brushless>(id);
				break;
			}
		}
	}

	SharedMotors2019 Motors2019::generate_shared() const {
		SharedMotors2019 s = {};
		for(uint8_t i = 0; i < ID_MAX_CONTROLLED_MOTORS; ++i) {
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
		for(uint8_t index = 0; index < ID_MAX_CONTROLLED_MOTORS; ++index) {
			if(_controlled[index]) {
				_controlled[index]->finished = true;
				_controlled[index]->wanted_nb_turns = 0;
				_controlled[index]->wanted_position = 0_deg;
			}
		}
		unlock_variables();

		for(uint8_t index = 0; index < ID_MAX_UNCONTROLLED_MOTORS; ++index) {
			if(_uncontrolled[index]) {
				deactivate_uncontrolled_motor(_uncontrolled[index]->id);
			}
		}

		for(uint8_t index = 0; index < ID_MAX_BRUSHLESS; ++index) {
			if(_brushless[index]) {
				deactivate_brushless(_brushless[index]->id);
			}
		}
	}
} // namespace PhysicalRobot