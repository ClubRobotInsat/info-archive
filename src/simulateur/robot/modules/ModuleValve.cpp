//
// Created by paul on 19/04/16.
//

#include "ModuleValve.h"
#include "../Robot2017.h"

ModuleValve::ModuleValve(Robot2017& robot) : _robot(robot) {}

int ModuleValve::infoToElecID(unsigned char idVanne) const {
	if(1 <= idVanne && idVanne <= 6) {
		return idVanne - 1;
	} else {
		logError("Erreur lors de la conversion des ID des vannes de elec -> info, id fourni par la carte : ", int(idVanne));
		throw std::runtime_error("Erreur lors de la conversion des ID des vannes de elec -> info");
	}
}

void ModuleValve::setValveOn(unsigned char idVanne) {
	Simulateur::getInstance().sendTextMessage("Le robot a activé la vanne : " + std::to_string(idVanne));
	_robot.setValveState(infoToElecID(idVanne), true);
}

void ModuleValve::setValveOff(unsigned char idVanne) {
	Simulateur::getInstance().sendTextMessage("Le robot a desactivé la vanne : " + std::to_string(idVanne));
	_robot.setValveState(infoToElecID(idVanne), false);
}

void ModuleValve::activateAllValves() {
	Simulateur::getInstance().sendTextMessage("Le robot a activé toutes les vannes");
	_robot.setAllValveState(true);
}

void ModuleValve::desactivateAllValves() {
	Simulateur::getInstance().sendTextMessage("Le robot a desactivé toutes les vannes");
	_robot.setAllValveState(false);
}

bool ModuleValve::getValveStatus(unsigned char idVanne) const {
	Simulateur::getInstance().sendTextMessage("Le robot a accédé à l'état de la vanne : " + std::to_string(idVanne));
	return _robot.getValveState(infoToElecID(idVanne));
}

bool ModuleValve::getAllValveStatus() const {
	bool result = true;
	for(int i = 0; i < 5; i++) {
		result = result && _robot.getValveState(i);
	}
	return result;
};