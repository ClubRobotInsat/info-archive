//
//  ConstantesPrincipal.cpp
//  Club Robot
//
//  Created by Rémi on 28/10/2015.
//

#include "Constantes2015.h"

namespace ConstantesPrincipal2015 {
	Angle const positionPince[enumToInt(PositionPince::NBR)] = {50_deg, 67.5_deg, 72.5_deg, 67_deg, 95_deg, 150_deg};
	Angle const positionCoude[enumToInt(PositionCoude::NBR)] = {170_deg, 40_deg, 7_deg};
	Angle const positionArriere[enumToInt(PositionArriere::NBR)] = {0_deg, 80_deg, 110_deg};
	Angle const positionBras[enumToInt(PositionBras::NBR)] = {150_deg, 57.5_deg, 30_deg};
	Angle const positionAscenseur[enumToInt(PositionAscenseur::NBR)] = {0_deg, 410_deg, 800_deg, 500_deg, 50_deg, 1000_deg};

	Distance const ENTRAXE_PINCES = 12_cm;
	Distance const DISTANCE_CENTRE_PINCES = 30_cm;
}

Duree const ConstantesPrincipal2015::TIMEOUT_DEPLACEMENT_DEFAUT = Constantes2015::TIMEOUT_DEPLACEMENT_DEFAUT;
Duree const ConstantesPrincipal2015::TIMEOUT_BLOCAGE_ADVERSAIRE_DEFAUT = Constantes2015::TIMEOUT_BLOCAGE_ADVERSAIRE_DEFAUT;
VitesseLineaire const ConstantesPrincipal2015::VITESSE_LINEAIRE_DEFAUT = Constantes2015::VITESSE_LINEAIRE_DEFAUT;
VitesseAngulaire const ConstantesPrincipal2015::VITESSE_ANGULAIRE_DEFAUT = 3.75_rad_s;
Distance const ConstantesPrincipal2015::PRECISION_LINEAIRE_DEFAUT = Constantes2015::PRECISION_LINEAIRE_DEFAUT;
Angle const ConstantesPrincipal2015::PRECISION_ANGULAIRE_DEFAUT = Constantes2015::PRECISION_ANGULAIRE_DEFAUT;

Distance const ConstantesPrincipal2015::RAYON_ROTATION_ROBOT = 230.0_mm;
Vector3m const ConstantesPrincipal2015::TAILLE_ROBOT = {30_cm, 30_cm, 40_cm};
uint16_t const ConstantesPrincipal2015::TCPIP_PORT_SIMU_DEFAUT = 4321;
Angle const ConstantesPrincipal2015::OFFSET_ANGLE_ADV = -0.5_PI;

int const ConstantesPrincipal2015::IDRobot = 1;

Duree ConstantesRobotPrincipal2015::getTimeoutDeplacementDefaut() const {
	return ConstantesPrincipal2015::TIMEOUT_DEPLACEMENT_DEFAUT;
}

Duree ConstantesRobotPrincipal2015::getTimeoutBlocageAdversaireDefaut() const {
	return ConstantesPrincipal2015::TIMEOUT_BLOCAGE_ADVERSAIRE_DEFAUT;
}

VitesseLineaire ConstantesRobotPrincipal2015::getVitesseLineaireDefaut() const {
	return ConstantesPrincipal2015::VITESSE_LINEAIRE_DEFAUT;
}

VitesseAngulaire ConstantesRobotPrincipal2015::getVitesseAngulaireDefaut() const {
	return ConstantesPrincipal2015::VITESSE_ANGULAIRE_DEFAUT;
}

Distance ConstantesRobotPrincipal2015::getPrecisionLineaireDefaut() const {
	return ConstantesPrincipal2015::PRECISION_LINEAIRE_DEFAUT;
}

Angle ConstantesRobotPrincipal2015::getPrecisionAngulaireDefaut() const {
	return ConstantesPrincipal2015::PRECISION_ANGULAIRE_DEFAUT;
}

Distance ConstantesRobotPrincipal2015::getRayonRotation() const {
	return ConstantesPrincipal2015::RAYON_ROTATION_ROBOT;
}

Vector3m ConstantesRobotPrincipal2015::getTailleRobot() const {
	return ConstantesPrincipal2015::TAILLE_ROBOT;
}

Angle ConstantesRobotPrincipal2015::getOffsetAngleAdv() const {
	return ConstantesPrincipal2015::OFFSET_ANGLE_ADV;
}


uint16_t ConstantesRobotPrincipal2015::getPortTCPIPDefaut() const {
	return 1234;
}

int ConstantesRobotPrincipal2015::getIDRobot() const {
	return ConstantesPrincipal2015::IDRobot;
}


