//
// Created by paul on 14/04/16.
//
#include "ModuleContactor.h"
#include "../../../../../CodeCommun/Informations_cartesElec_2017.h"
#include "../Robot2017.h"
#include <log/Log.h>

ModuleContactor::ModuleContactor(Robot2017& robot2017) : _robot(robot2017) {}

int ModuleContactor::infoToElecID(unsigned char idContact) const {

	if(idContact >= 1 && idContact <= 6) {
		return idContact - 1;
	} else {
		logError("Erreur lors de la conversion des ID des vannes de elec -> info");
		throw std::runtime_error("Erreur lors de la conversion des ID des vannes de elec -> info");
	}
}

bool ModuleContactor::getContactorState(unsigned char numeroContact, unsigned char idCarte) const {
	if(idCarte == ID_CARTE_INYANGA_IO) {
		if(numeroContact >= 1 && numeroContact <= _robot.getNumberofContactor()) {
			Simulateur::getInstance().sendTextMessage("Le robot a accédé au contacteur : " + std::to_string(numeroContact));
			return _robot.getContactor(infoToElecID(numeroContact));
		} else {
			logError("Numero de contacteur inexistant : Numéro demandé :", (short)numeroContact);
			Simulateur::getInstance().sendTextMessage(
			    "Le robot a fourni une mauvaise ID lors de l'accés au contacteur : " + std::to_string(numeroContact));
			throw std::runtime_error("Bad Card ID");
		}
	} else {
		logError("carte contacteur inexistante : ID carte : ", (short)idCarte);
		return false;
	}
}

void ModuleContactor::setContactorState(unsigned char numeroContact, unsigned char idCarte) {
	if(idCarte == ID_CARTE_INYANGA_IO) {
		if(numeroContact >= 1 && numeroContact <= _robot.getNumberofContactor()) {
			_robot.getContactor(infoToElecID(numeroContact)) = not _robot.getContactor(infoToElecID(numeroContact)); // TODO : vérifier que c'est bien le comportement de la carte elec
		} else {
			logError("Numero de contacteur inexistant : Numéro demandé :", (short)numeroContact);
		}
	} else {
		logError("carte contacteur inexistante : ID carte : ", (short)idCarte);
	}
}