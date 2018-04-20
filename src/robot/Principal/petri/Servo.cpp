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
	return meca().ouvrirSouteD();
}

ResultatAction fermerSouteD() {
	return meca().fermerSouteD();
}

ResultatAction ouvrirSouteG() {
	return meca().ouvrirSouteG();
}

ResultatAction fermerSouteG() {
	return meca().fermerSouteG();
}

ResultatAction ouvrirAbeilleD() {
	return meca().ouvrirAbeilleD();
}

ResultatAction fermerAbeilleD() {
	return meca().fermerAbeilleD();
}

ResultatAction ouvrirAbeilleG() {
	return meca().ouvrirAbeilleG();
}

ResultatAction fermerAbeilleG() {
	return meca().fermerAbeilleG();
}
