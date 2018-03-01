//
// Created by benjamin on 17/04/16.
//

#include "ModulePneumatique.h"
#include "../Robot2018.h"


ModulePneumatique::ModulePneumatique(Robot2018& robot2018) : _robot(robot2018) {}

int ModulePneumatique::infoToElecID(unsigned char cardID) {
	switch(cardID) {
		case IDCartesPrincipal::ID_CARTE_PNEUMATIQUE_BAS:
			return 0;
		case IDCartesPrincipal::ID_CARTE_PNEUMATIQUE_HAUTE:
			return 1;
		default:
			logError("Erreur de mappage des ID elecs sur les ID infos");
			throw std::runtime_error("Erreur de mappage des ID elecs sur les ID infos");
	}
}

void ModulePneumatique::setActivation(uint8_t idCarte) {
	switch(idCarte) {
		case IDCartesPrincipal::ID_CARTE_PNEUMATIQUE_BAS:
			_robot.setActivation((uint8_t)infoToElecID(idCarte), true);
			Simulateur::getInstance().sendTextMessage("Le robot a activé la pompe BAS");
			break;
		case IDCartesPrincipal::ID_CARTE_PNEUMATIQUE_HAUTE:
			_robot.setActivation((uint8_t)infoToElecID(idCarte), true);
			Simulateur::getInstance().sendTextMessage("Le robot a activé la pompe HAUT");
			break;
		default:
			logError("carte pneumatique inexistante : ID carte : ", (short)idCarte);
			Simulateur::getInstance().sendTextMessage("Le robot a donné une mauvaise ID de pompe à l'activation. " +
			                                          std::to_string(idCarte));
	}
}

void ModulePneumatique::setDesactivation(uint8_t idCarte) {
	switch(idCarte) {
		case IDCartesPrincipal::ID_CARTE_PNEUMATIQUE_BAS:
			_robot.setActivation((uint8_t)infoToElecID(idCarte), false);
			Simulateur::getInstance().sendTextMessage("Le robot a dessactivé la pompe BAS");
			break;
		case IDCartesPrincipal::ID_CARTE_PNEUMATIQUE_HAUTE:
			_robot.setActivation((uint8_t)infoToElecID(idCarte), false);
			Simulateur::getInstance().sendTextMessage("Le robot a desactivé la pompe HAUT");
			break;
		default:
			logError("carte pneumatique inexistante : ID carte : ", (short)idCarte);
			Simulateur::getInstance().sendTextMessage("Le robot a donné une mauvaise ID de pompe à l'activation. " +
			                                          std::to_string(idCarte));
	}
}

bool ModulePneumatique::isActivated(uint8_t idCarte) {
	switch(idCarte) {
		case IDCartesPrincipal::ID_CARTE_PNEUMATIQUE_BAS:
			Simulateur::getInstance().sendTextMessage("Le robot a accédé à la pompe BAS");
			return _robot.isActivated((uint8_t)infoToElecID(idCarte));
		case IDCartesPrincipal::ID_CARTE_PNEUMATIQUE_HAUTE:
			Simulateur::getInstance().sendTextMessage("Le robot a accédé à la pompe HAUT");
			return _robot.isActivated((uint8_t)infoToElecID(idCarte));
		default:
			logError("carte pneumatique inexistante : ID carte : ", (short)idCarte);
			Simulateur::getInstance().sendTextMessage("Le robot a donné une mauvaise ID de pompe à l'activation. " +
			                                          std::to_string(idCarte));
			throw std::runtime_error("Error bad card ID");
	}
}