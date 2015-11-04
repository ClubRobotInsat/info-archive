//
//  ConstantesPrincipal.cpp
//  Club Robot
//
//  Created by Rémi on 28/10/.
//

#include "Constantes.h"

namespace ConstantesPrincipal {
	Angle const positionPince[enumToInt(PositionPince::NBR)] = {50_deg, 67.5_deg, 72.5_deg, 67_deg, 95_deg, 150_deg};
	Angle const positionCoude[enumToInt(PositionCoude::NBR)] = {170_deg, 40_deg, 7_deg};
	Angle const positionArriere[enumToInt(PositionArriere::NBR)] = {0_deg, 80_deg, 110_deg};
	Angle const positionBras[enumToInt(PositionBras::NBR)] = {150_deg, 57.5_deg, 30_deg};
	Angle const positionAscenseur[enumToInt(PositionAscenseur::NBR)] = {0_deg, 410_deg, 800_deg, 500_deg, 50_deg, 1000_deg};

	Distance const ENTRAXE_PINCES = 12_cm;
	Distance const DISTANCE_CENTRE_PINCES = 30_cm;
}

Duree const ConstantesPrincipal::TIMEOUT_DEPLACEMENT_DEFAUT = Constantes::TIMEOUT_DEPLACEMENT_DEFAUT;
Duree const ConstantesPrincipal::TIMEOUT_BLOCAGE_ADVERSAIRE_DEFAUT = Constantes::TIMEOUT_BLOCAGE_ADVERSAIRE_DEFAUT;
VitesseLineaire const ConstantesPrincipal::VITESSE_LINEAIRE_DEFAUT = Constantes::VITESSE_LINEAIRE_DEFAUT;
VitesseAngulaire const ConstantesPrincipal::VITESSE_ANGULAIRE_DEFAUT = 3.75_rad_s;
Distance const ConstantesPrincipal::PRECISION_LINEAIRE_DEFAUT = Constantes::PRECISION_LINEAIRE_DEFAUT;
Angle const ConstantesPrincipal::PRECISION_ANGULAIRE_DEFAUT = Constantes::PRECISION_ANGULAIRE_DEFAUT;

Distance const ConstantesPrincipal::RAYON_ROTATION_ROBOT = 230.0_mm;
Vector3m const ConstantesPrincipal::TAILLE_ROBOT = {30_cm, 30_cm, 40_cm};
uint16_t const ConstantesPrincipal::TCPIP_PORT_SIMU_DEFAUT = 4321;
Angle const ConstantesPrincipal::OFFSET_ANGLE_ADV = -0.5_PI;

int const ConstantesPrincipal::IDRobot = 1;

Duree ConstantesRobotPrincipal::getTimeoutDeplacementDefaut() const {
	return ConstantesPrincipal::TIMEOUT_DEPLACEMENT_DEFAUT;
}

Duree ConstantesRobotPrincipal::getTimeoutBlocageAdversaireDefaut() const {
	return ConstantesPrincipal::TIMEOUT_BLOCAGE_ADVERSAIRE_DEFAUT;
}

VitesseLineaire ConstantesRobotPrincipal::getVitesseLineaireDefaut() const {
	return ConstantesPrincipal::VITESSE_LINEAIRE_DEFAUT;
}

VitesseAngulaire ConstantesRobotPrincipal::getVitesseAngulaireDefaut() const {
	return ConstantesPrincipal::VITESSE_ANGULAIRE_DEFAUT;
}

Distance ConstantesRobotPrincipal::getPrecisionLineaireDefaut() const {
	return ConstantesPrincipal::PRECISION_LINEAIRE_DEFAUT;
}

Angle ConstantesRobotPrincipal::getPrecisionAngulaireDefaut() const {
	return ConstantesPrincipal::PRECISION_ANGULAIRE_DEFAUT;
}

Distance ConstantesRobotPrincipal::getRayonRotation() const {
	return ConstantesPrincipal::RAYON_ROTATION_ROBOT;
}

Vector3m ConstantesRobotPrincipal::getTailleRobot() const {
	return ConstantesPrincipal::TAILLE_ROBOT;
}

Angle ConstantesRobotPrincipal::getOffsetAngleAdv() const {
	return ConstantesPrincipal::OFFSET_ANGLE_ADV;
}


uint16_t ConstantesRobotPrincipal::getPortTCPIPDefaut() const {
	return 1234;
}

int ConstantesRobotPrincipal::getIDRobot() const {
	return ConstantesPrincipal::IDRobot;
}


