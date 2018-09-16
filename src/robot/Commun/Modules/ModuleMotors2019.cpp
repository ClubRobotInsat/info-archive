//
// Created by terae on 9/10/18.
//

#include "ModuleMotors2019.h"

namespace Commun {
	uint8_t ModuleMotors2019::get_nbr_controlled() const {
		uint8_t count = 0;
		for(uint8_t i = 0; i < NB_MAX_CONTROLLED_MOTORS; ++i) {
			count += (_controlled[i] != nullptr);
		}
		return count;
	}

	uint8_t ModuleMotors2019::get_nbr_uncontrolled() const {
		uint8_t count = 0;
		for(uint8_t i = 0; i < NB_MAX_UNCONTROLLED_MOTORS; ++i) {
			count += (_uncontrolled[i] != nullptr);
		}
		return count;
	}

	uint8_t ModuleMotors2019::get_nbr_brushless() const {
		uint8_t count = 0;
		for(uint8_t i = 0; i < NB_MAX_BRUSHLESS; ++i) {
			count += (_brushless[i] != nullptr);
		}
		return count;
	}

	uint8_t ModuleMotors2019::get_frame_size() const {
		return static_cast<uint8_t>(3 + get_nbr_controlled() * 4 + get_nbr_uncontrolled() * 2 + get_nbr_brushless() * 2);
	}

	void ModuleMotors2019::add_controlled_motor(uint8_t id, RotatingDirection rotation) {
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

	void ModuleMotors2019::add_uncontrolled_motor(uint8_t id) {
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

	void ModuleMotors2019::add_brushless(uint8_t id) {
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

	SharedMotors2019 ModuleMotors2019::generate_shared() const {
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

		return s;
	}

	void ModuleMotors2019::message_processing(const SharedMotors2019&) {
		// TODO
	}
} // namespace Commun