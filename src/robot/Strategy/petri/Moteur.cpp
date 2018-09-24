//
// Created by terae on 4/20/18.
//

#include "Moteur.h"
#include "../../../Principal/librobot/libRobot.h"

namespace {
	StrategiePrincipal* _strategie;

	MecaManagerPrincipal& meca() {
		return _strategie->getMecaManager();
	}
} // namespace

void initMoteurPetri(StrategiePrincipal& strategie) {
	_strategie = &strategie;
}

ResultatAction monterAscenseursDe(int nbr_tours) {
	return meca().monterAscenseursDe(nbr_tours);
}

ResultatAction monterAscenseursDe(Angle value) {
	return meca().monterAscenseursDe(value);
}

int cubesLibres() {
	return meca().nbrPlaceAscenseur();
}

ResultatAction activerAvaleurs(SensAvaleurs sens) {
	return meca().activerAvaleurs(sens);
}

ResultatAction desactiverAvaleurs() {
	return meca().desactiverAvaleurs();
}

ResultatAction activerTurbineD() {
	if(_strategie->getCouleurEquipe() == RobotColor::Green) {
		return meca().activerTurbineD();
	} else {
		return meca().activerTurbineG();
	}
}

ResultatAction activerTurbineG() {
	if(_strategie->getCouleurEquipe() == RobotColor::Green) {
		return meca().activerTurbineG();
	} else {
		return meca().activerTurbineD();
	}
}

ResultatAction desactiverTurbineD() {
	if(_strategie->getCouleurEquipe() == RobotColor::Green) {
		return meca().desactiverTurbineD();
	} else {
		return meca().desactiverTurbineG();
	}
}

ResultatAction desactiverTurbineG() {
	if(_strategie->getCouleurEquipe() == RobotColor::Green) {
		return meca().desactiverTurbineG();
	} else {
		return meca().desactiverTurbineD();
	}
}
