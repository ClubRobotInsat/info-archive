//
// Created by benjamin on 17/04/16.
//

#include "ModuleServos.h"
#include "../Robot2017.h"

std::string generateMessage(uint8_t numServo, Angle angle) {
	return "Le robot a envoyé un ordre d'angle au servo :" + std::to_string(numServo) + " | Consigne : " +
	       std::to_string(angle.toDeg());
}

ModuleServos::ModuleServos(Robot2017& robot2017) : _robot(robot2017){};

void ModuleServos::setAngle(uint8_t idCarte, uint8_t numServo, Angle angle) {
	if(idCarte == ID_CARTE_INYANGA_SERVOS) {
		if(numServo < (short)ConstantesPrincipal::Servo::NBR) {
			Simulateur::getInstance().sendTextMessage(generateMessage(numServo, angle));
			//_robot.setAngle(idCarte, numServo, angle);
		} else {
			logError("Numero de servo inexistant : Numéro demandé :", (short)numServo);
		}
	}
	logError("carte servo inexistante : ID carte : ", (short)idCarte);
}

Angle ModuleServos::getAngle(uint8_t idCarte, uint8_t numServo) {
	if(idCarte == ID_CARTE_INYANGA_SERVOS) {
		if(numServo < (short)ConstantesPrincipal::Servo::NBR) {
			// return _robot.getAngle(idCarte, numServo);
			return 0_deg;
		} else {
			logError("Numero de servo inexistant : Numéro demandé :", (short)numServo);
		}
	}
	logError("carte servo inexistante : ID carte : ", (short)idCarte);

	return 0_rad;
}

bool ModuleServos::isMovementCompleted(uint8_t idCarte, uint8_t numServo) {
	switch(idCarte) {
		case ID_CARTE_INYANGA_SERVOS:
			if(numServo < (short)ConstantesPrincipal::Servo::NBR) {
				// return _robot.isMovementCompleted(idCarte, numServo);
				return true;
			} else {
				logError("Numero de servo inexistant : Numéro demandé :", (short)numServo);
			}
			break;

		default:
			logError("carte servo inexistante : ID carte : ", (short)idCarte);
			break;
	}
	throw std::runtime_error("Impossible de déterminer si le mouvement du servo est terminé");
}

void ModuleServos::setSpeed(uint8_t idCarte, uint8_t numServo, unsigned char /*speed*/) {
	switch(idCarte) {
		case ID_CARTE_INYANGA_SERVOS:
			if(numServo < (short)ConstantesPrincipal::Servo::NBR) {
				; //_robot.setSpeed(idCarte, numServo, speed);
			} else {
				logError("Numero de servo inexistant : Numéro demandé :", (short)numServo);
			}
			break;

		default:
			logError("carte servo inexistante : ID carte : ", (short)idCarte);
			break;
	}
	throw std::runtime_error("Impossible de donner une vitesse au servo");
}

bool ModuleServos::isBlocked(uint8_t idCarte, uint8_t numServo) const {
	switch(idCarte) {
		case ID_CARTE_INYANGA_SERVOS:
			if(numServo < (short)ConstantesPrincipal::Servo::NBR) {
				// return _robot.isBlocked(idCarte, numServo);
				return false;
			} else {
				logError("Numero de servo inexistant : Numéro demandé :", (short)numServo);
			}
			break;

		default:
			logError("carte servo inexistante : ID carte : ", (short)idCarte);
			break;
	}
	throw std::runtime_error("Impossible de déterminer si le servo est bloqué ou non");
}