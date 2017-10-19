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
/*
ResultatAction bougerBrasDroit(Angle angle) {
    return meca().bougerBrasBloquant(ConstantesPrincipal::Pince::DROIT, angle);
}*/


ResultatAction ascenseurStockageHaut() {
	return meca().ascenseurStockageHaut();
}

ResultatAction ascenseurStockageBas() {
	return meca().ascenseurStockageBas();
}

ResultatAction ascenseurCylindreSol() {
	return meca().ascenseurCylindreSol();
}

ResultatAction ascenseurAjustagePourSaisieCylindreBas() {
	return meca().ascenseurAjustagePourSaisieCylindreBas();
}

ResultatAction ascenseurPoserRails() {
	return meca().ascenseurPoserRails();
}

ResultatAction ouvrirPince() {
	return meca().ouvrirPince();
}

ResultatAction fermerPince() {
	return meca().fermerPince();
}

ResultatAction attraperCylindre() {
	return meca().attraperCylindre();
}

ResultatAction orienterPinceAttraper() {
	return meca().orienterPinceAttraper();
}

ResultatAction orienterPinceStocker() {
	return meca().orienterPinceStocker();
}

ResultatAction pencherPinceBas() {
	return meca().pencherPinceBas();
}

ResultatAction pencherPinceDevant() {
	return meca().pencherPinceDevant();
}

ResultatAction stockerCylindre() {
	return meca().stockerCylindre();
}

ResultatAction destockerCylindre() {
	return meca().destockerCylindre();
}

int getNbrCylindres() {
	return meca().getNbrCylindres();
}

ResultatAction printCylindres() {
	return meca().printCylindres();
}

ResultatAction ouvrirFesseGauche() {
	return meca().ouvrirFesseGauche();
}

ResultatAction ouvrirFesseDroite() {
	return meca().ouvrirFesseDroite();
}

ResultatAction fermerFesseGauche() {
	return meca().fermerFesseGauche();
}

ResultatAction fermerFesseDroite() {
	return meca().fermerFesseDroite();
}