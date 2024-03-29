//
// Created by benji on 14/10/16.
//

#include "Servo.h"
#include "../../../Principal/librobot/libRobot.h"

namespace {
	StrategiePrincipal* _strategie;

	MecaManagerPrincipal& meca() {
		return _strategie->getMecaManager();
	}
} // namespace

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
		return meca().fermerSouteD();
	}
}

ResultatAction ouvrirAbeille() {
	return meca().ouvrirAbeille();
}

ResultatAction fermerAbeille() {
	return meca().fermerAbeille();
}

ResultatAction monterLoquet() {
	return meca().monterLoquet();
}

ResultatAction descendreLoquet() {
	return meca().descendreLoquet();
}
