//
// Created by benji on 14/10/16.
//

#include "Servo.h"
#include "../librobot/libRobot.h"

namespace {
	StrategiePrincipal* _strategie;

	MecaManagerPrincipal& meca() {
		return _strategie->getMecaManager();
	}
}

void initServoPetri(StrategiePrincipal& strategie) {
	_strategie = &strategie;
}

// liste des actions petri :
ResultatAction ouvrirPorteCube() {
	return meca().ouvrirPorteCube();
}

ResultatAction fermerPorteCube() {
	return meca().fermerPorteCube();
}

ResultatAction ouvrirSouteD() {
	if(_strategie->getCouleurEquipe() == RobotColor::Green) {
		return meca().ouvrirSouteD();
	} else {
		return meca().ouvrirSouteG();
	}
}

ResultatAction fermerSouteD() {
	if(_strategie->getCouleurEquipe() == RobotColor::Green) {
		return meca().fermerSouteD();
	} else {
		return meca().fermerSouteG();
	}
}

ResultatAction ouvrirSouteG() {
	if(_strategie->getCouleurEquipe() == RobotColor::Green) {
		return meca().ouvrirSouteG();
	} else {
		return meca().ouvrirSouteD();
	}
}

ResultatAction fermerSouteG() {
	if(_strategie->getCouleurEquipe() == RobotColor::Green) {
		return meca().fermerSouteG();
	} else {
		return meca().fermerAbeilleD();
	}
}

ResultatAction ouvrirAbeilleD() {
	if(_strategie->getCouleurEquipe() == RobotColor::Green) {
		return meca().ouvrirAbeilleD();
	} else {
		return meca().ouvrirAbeilleG();
	}
}

ResultatAction fermerAbeilleD() {
	if(_strategie->getCouleurEquipe() == RobotColor::Green) {
		return meca().fermerAbeilleD();
	} else {
		return meca().fermerAbeilleG();
	}
}

ResultatAction ouvrirAbeilleG() {
	if(_strategie->getCouleurEquipe() == RobotColor::Green) {
		return meca().ouvrirAbeilleG();
	} else {
		return meca().ouvrirAbeilleD();
	}
}

ResultatAction fermerAbeilleG() {
	if(_strategie->getCouleurEquipe() == RobotColor::Green) {
		return meca().fermerAbeilleG();
	} else {
		return meca().fermerAbeilleD();
	}
}
