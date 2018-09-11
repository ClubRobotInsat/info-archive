//
// Created by terae on 8/21/18.
//

#include "ModuleServos2019.h"
#include <log/Log.h>

namespace Commun {
	void ModuleServos2019::add_servo(uint8_t id, Angle start_position, BlockingMode mode) {
		if(id >= NB_MAX_SERVOS) {
			throw std::runtime_error("ID du servo trop grand ("s + std::to_string(id) + " > " + std::to_string(NB_MAX_SERVOS) + ") !");
		} else if(id == 0) {
			throw std::runtime_error("L'ID 0 des servos est réservé !");
		} else if(_servos[id]) {
			throw std::runtime_error("Double assignation du servo "s + std::to_string(id) + " !");
		}

		_servos[id] = std::make_unique<Servo>(id, start_position, mode);
	}

	uint8_t ModuleServos2019::get_nbr_servos() const {
		uint8_t count = 0;
		for(uint8_t i = 0; i < NB_MAX_SERVOS; ++i) {
			count += (_servos[i] != nullptr);
		}
		return count;
	}

	bool ModuleServos2019::is_servo_ok(uint8_t id) const {
		// 'id == 0' veut dire qu'il n'y a pas de servo-moteur dans la représentation C
		return id > 0 && id < NB_MAX_SERVOS && static_cast<bool>(_servos[id]);
	}

	uint8_t ModuleServos2019::get_frame_size() const {
		return static_cast<uint8_t>(1 + get_nbr_servos() * 7);
	}

	void ModuleServos2019::set_position(uint8_t id, Angle angle) {
		if(!is_servo_ok(id)) {
			throw std::runtime_error("Numéro du servo demandé invalide : "s + std::to_string(id));
		}

		lockVariables();
		_servos[id]->blocked = false;
		_servos[id]->wanted_position = angle;
		unlockVariables();
	}

	void ModuleServos2019::set_speed(uint8_t id, uint8_t speed) {
		if(!is_servo_ok(id)) {
			throw std::runtime_error("Numéro du servo demandé invalide : "s + std::to_string(id));
		}

		lockVariables();
		_servos[id]->speed = speed;
		unlockVariables();
	}

	Angle ModuleServos2019::read_position(uint8_t id) const {
		if(!is_servo_ok(id)) {
			throw std::runtime_error("Numéro du servo demandé invalide : "s + std::to_string(id));
		}

		std::lock_guard<std::mutex> lk(_mutexVariables);
		return _servos[id]->position;
	}

	void ModuleServos2019::set_color(uint8_t id, Color color) {
		if(!is_servo_ok(id)) {
			throw std::runtime_error("Numéro du servo demandé invalide : "s + std::to_string(id));
		}

		lockVariables();
		_servos[id]->color = color;
		unlockVariables();
	}

	bool ModuleServos2019::is_blocking(uint8_t id) const {
		if(!is_servo_ok(id)) {
			throw std::runtime_error("Numéro du servo demandé invalide : "s + std::to_string(id));
		}

		std::lock_guard<std::mutex> lk(_mutexVariables);
		return _servos[id]->blocked;
	}

	bool ModuleServos2019::is_moving_done(uint8_t id) const {
		if(!is_servo_ok(id)) {
			throw std::runtime_error("Numéro du servo demandé invalide : "s + std::to_string(id));
		}

		std::lock_guard<std::mutex> lk(_mutexVariables);
		// TODO : voir s'il faut mettre un seuil pour considérer le mouvement terminé, ou si les élecs nous le disent
		return _servos[id]->position == _servos[id]->wanted_position;
	}

	SharedServos2019 ModuleServos2019::generate_shared() const {
		SharedServos2019 s = {};
		for(uint8_t i = 0; i < NB_MAX_SERVOS; ++i) {
			if(_servos[i]) {
				s.servos[i].id = i;

				auto angle_to_uint16t = [i](Angle angle) -> uint16_t {
					uint16_t pos = static_cast<uint16_t>((angle.toMinusPiPi().toDeg() + 166.7) * 1023 / 333.4);
					if(pos < 21 || pos > 1002) {
						logWarn("Angle demandé en-dehors de l'intervalle [-159.8°; 159.8°] pour le servo n°", (int)i);
					}
					pos = static_cast<uint16_t>(pos < 21 ? 21 : (pos > 1023 ? 1023 : pos));
					return pos;
				};

				s.servos[i].position = angle_to_uint16t(_servos[i]->position);
				s.servos[i].wanted_position = angle_to_uint16t(_servos[i]->wanted_position);
				s.servos[i].blocked = _servos[i]->blocked;
				s.servos[i].speed = _servos[i]->speed;
				s.servos[i].color = _servos[i]->color;
				s.servos[i].blocking_mode = _servos[i]->blocking_mode;

			} else {
				s.servos[i].id = 0;
			}
		}

		return s;
	}

	void ModuleServos2019::message_processing(const SharedServos2019& s) {
		for(uint8_t i = 0; i < NB_MAX_SERVOS; ++i) {
			if(_servos[i] && s.servos[i].id != 0) {
				auto uint16t_to_angle = [](uint16_t pos) -> Angle {
					Angle angle = 0_deg;
					return angle;
				};
				auto uint8t_to_color = [](uint8_t val) -> Color {
					return (val >= Color::NBR ? RED : static_cast<Color>(val));
				};

				_servos[i]->position = uint16t_to_angle(s.servos[i].position);
				_servos[i]->blocked.exchange(s.servos[i].blocked);
				_servos[i]->speed.exchange(s.servos[i].speed);
				_servos[i]->color.exchange(uint8t_to_color(s.servos[i].color));
			}
		}
	}
}
