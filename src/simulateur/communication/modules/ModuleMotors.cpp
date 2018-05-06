//
// Created by terae on 5/3/18.
//

#include "ModuleMotors.h"
#include "../Robot2018.h"

#include "../../core/Simulateur.h"

std::string generateMessage(uint8_t num_motor, std::string cmd) {
	return "Le robot a envoyé un ordre au moteur : " + std::to_string(num_motor) + " | Consigne : " + cmd;
}

void ModuleMotors::setEnginePositionAngle(uint8_t idCarte, uint8_t num_motor, Angle angle) {
	if(idCarte == ID_ELEC_CARD_MOTORS) {
		if(num_motor < (short)ConstantesPrincipal::Moteurs::NBR) {
			Simulateur::getInstance().sendTextMessage(
			    generateMessage(num_motor, "[asservi] position angle à " + std::to_string(angle)));
		} else {
			logError("Numéro de moteur inexistant : Numéro demandé : ", (short)num_motor);
		}
		logError("carte moteur inexistante : ID carte : ", (short)idCarte);
	}
}

void ModuleMotors::setEnginePositionTurn(uint8_t idCarte, uint8_t num_motor, std::size_t nb_turns) {
	if(idCarte == ID_ELEC_CARD_MOTORS) {
		if(num_motor < (short)ConstantesPrincipal::Moteurs::NBR) {
			Simulateur::getInstance().sendTextMessage(
			    generateMessage(num_motor, "[asservi] position en " + std::to_string(nb_turns) + " tours"));
		} else {
			logError("Numéro de moteur inexistant : Numéro demandé : ", (short)num_motor);
		}
		logError("carte moteur inexistante : ID carte : ", (short)idCarte);
	}
}

bool ModuleMotors::isAtPosition(uint8_t idCarte, uint8_t num_motor) const {
	if(idCarte == ID_ELEC_CARD_MOTORS) {
		if(num_motor < (short)ConstantesPrincipal::Moteurs::NBR) {
			Simulateur::getInstance().sendTextMessage(generateMessage(num_motor, "[asservi] demande de position"));
		} else {
			logError("Numéro de moteur inexistant : Numéro demandé : ", (short)num_motor);
		}
		logError("carte moteur inexistante : ID carte : ", (short)idCarte);
	}
	return true;
}

void ModuleMotors::setEngineOn(uint8_t idCarte, uint8_t num_motor, SensRotation sens) {
	if(idCarte == ID_ELEC_CARD_MOTORS) {
		if(num_motor < (short)ConstantesPrincipal::Moteurs::NBR) {
			Simulateur::getInstance().sendTextMessage(
			    generateMessage(num_motor,
			                    "[non asservi] activation du moteur dans le sens " +
			                        (sens == SensRotation::Trigo ? "trigo"s : "horaire"s)));
		} else {
			logError("Numéro de moteur inexistant : Numéro demandé : ", (short)num_motor);
		}
		logError("carte moteur inexistante : ID carte : ", (short)idCarte);
	}
}

void ModuleMotors::setEngineOff(uint8_t idCarte, uint8_t num_motor) {
	if(idCarte == ID_ELEC_CARD_MOTORS) {
		if(num_motor < (short)ConstantesPrincipal::Moteurs::NBR) {
			Simulateur::getInstance().sendTextMessage(
			    generateMessage(num_motor, "[non asservi] désactivation du moteur"));
		} else {
			logError("Numéro de moteur inexistant : Numéro demandé : ", (short)num_motor);
		}
		logError("carte moteur inexistante : ID carte : ", (short)idCarte);
	}
}

void ModuleMotors::setBrushlessOn(uint8_t idCarte, uint8_t num_motor) {
	if(idCarte == ID_ELEC_CARD_MOTORS) {
		if(num_motor < (short)ConstantesPrincipal::Moteurs::NBR) {
			Simulateur::getInstance().sendTextMessage(generateMessage(num_motor, "[brushless] activation du moteur"));
		} else {
			logError("Numéro de moteur inexistant : Numéro demandé : ", (short)num_motor);
		}
		logError("carte moteur inexistante : ID carte : ", (short)idCarte);
	}
}

void ModuleMotors::setBrushlessOff(uint8_t idCarte, uint8_t num_motor) {
	if(idCarte == ID_ELEC_CARD_MOTORS) {
		if(num_motor < (short)ConstantesPrincipal::Moteurs::NBR) {
			Simulateur::getInstance().sendTextMessage(
			    generateMessage(num_motor, "[brushless] désactivation du moteur"));
		} else {
			logError("Numéro de moteur inexistant : Numéro demandé : ", (short)num_motor);
		}
		logError("carte moteur inexistante : ID carte : ", (short)idCarte);
	}
}
