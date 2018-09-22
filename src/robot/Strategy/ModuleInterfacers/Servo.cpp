//
//  Servo.cpp
//  Club Robot
//
//  Created by Rémi on 12/01/2015.
//

#include "Servo.h"

namespace Commun {

	ServoManager::ServoManager(Servos2019& module_servo, std::function<Angle(uint8_t, uint8_t)> getPositionServo)
	        : _module(module_servo), _getPositionServo(std::move(getPositionServo)) {}

	bool ServoManager::estALaPosition(uint8_t servo, Angle position) {
		return lirePosition(servo) == position;
	}

	Angle ServoManager::lirePosition(uint8_t servo) {
		return _module.read_position(servo) - _offset[servo];
	}

	ResultatAction ServoManager::positionnerServoBloquant(uint8_t servo, Angle pos) {
		auto TIMEOUT_SERVO = 1000_ms; // 500_ms;
		auto timeoutDate = TimePoint::now() + TIMEOUT_SERVO;

		// Ajout de l'offset à la position en cours
		pos += _offset[servo];
		Angle anciennePos = lirePosition(servo);
		_module.set_position(servo, pos);

		while(true) {
			if(_module.is_moving_done(servo)) {
				return ResultatAction::REUSSI;
			}

			if(TimePoint::now() >= timeoutDate) {
				logWarn("TIMEOUT !");
				return ResultatAction::TIMEOUT;
			}

			if(_module.is_blocking(servo)) {
				logWarn("Bloqué, je reviens à la position précédente !");
				_module.set_position(servo, anciennePos);
				return ResultatAction::RATE;
			}
			sleep(50_ms);
		}
	}

	void ServoManager::reglerVitesse(uint8_t servo, uint8_t vitesse) {
		_module.set_speed(servo, vitesse);
	}

	void ServoManager::setCouleur(uint8_t servo, Servos2019::Color color) {
		_module.set_color(servo, color);
	}

	void ServoManager::reglerModeBlocage(uint8_t servo, Servos2019::BlockingMode mode) {
		_module.set_blocking_mode(servo, mode);
	}
} // namespace Commun
