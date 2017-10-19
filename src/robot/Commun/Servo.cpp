//
//  Servo.cpp
//  Club Robot
//
//  Created by Rémi on 12/01/2015.
//

#include "Servo.h"

namespace Commun {

	ServoManager::ServoManager(CarteServo& carteServo, std::function<Angle(uint8_t, uint8_t)> getPositionServo)
	        : _carte(carteServo), _getPositionServo(getPositionServo) {}

	bool ServoManager::estALaPosition(uint8_t servo, Angle position) {
		return lirePosition(servo) == position;
	}

	Angle ServoManager::lirePosition(uint8_t servo) {
		return _carte.lirePosition(servo) - _offset[servo];
	}

	ResultatAction ServoManager::positionnerServoBloquant(uint8_t servo, Angle pos) {
		auto TIMEOUT_SERVO = 1000_ms; // 500_ms;
		auto timeoutDate = TimePoint::now() + TIMEOUT_SERVO;

		// Ajout de l'offset à la position en cours
		pos += _offset[servo];
		Angle anciennePos = lirePosition(servo);
		_carte.reglerPosition(servo, pos);
		if(servo == 0x03)
			for(int i = 0; i < 15; i++)
				_carte.reglerPosition(servo, pos);

		while(true) {
			if(_carte.verifierDeplacementTermine(servo)) {
				return ResultatAction::REUSSI;
			}

			if(TimePoint::now() >= timeoutDate) {
				logWarn("TIMEOUT !");
				return ResultatAction::TIMEOUT;
			}

			if(_carte.verifierServoBloque(servo)) {
				logWarn("Bloqué, je reviens à la position précédente !");
				_carte.reglerPosition(servo, anciennePos);
				return ResultatAction::RATE;
			}
			sleep(50_ms);
		}
	}

	void ServoManager::reglerVitesse(uint8_t servo, uint8_t vitesse) {
		_carte.setServoVitesse(servo, vitesse);
	}

	void ServoManager::setCouleur(uint8_t servo, CarteServo::Couleur couleur) {
		_carte.setCouleur(servo, couleur);
	}

	void ServoManager::reglerModeBlocage(uint8_t servo, CarteServo::ModeBlocage mode) {
		_carte.reglerModeBlocage(servo, mode);
	}
}
