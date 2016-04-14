//
//  ConstantesPrincipal.cpp
//  Club Robot
//
//  Created by Rémi on 28/10/.
//

#include "Constantes.h"

namespace ConstantesPrincipal {
	Angle const positionPince[enumToInt(PositionPince::NBR)] = {50_deg, 67.5_deg}; // TODO: Modifier les valeurs pour cette année.

	Distance const ENTRAXE_PINCES = 12_cm;
	Distance const DISTANCE_CENTRE_PINCES = 30_cm;
}

Duration const ConstantesPrincipal::TIMEOUT_DEPLACEMENT_DEFAUT = Constantes::TIMEOUT_DEPLACEMENT_DEFAUT;
Duration const ConstantesPrincipal::TIMEOUT_BLOCAGE_ADVERSAIRE_DEFAUT = Constantes::TIMEOUT_BLOCAGE_ADVERSAIRE_DEFAUT;
Speed const ConstantesPrincipal::VITESSE_LINEAIRE_DEFAUT = Constantes::VITESSE_LINEAIRE_DEFAUT;
AngularSpeed const ConstantesPrincipal::VITESSE_ANGULAIRE_DEFAUT = 3.75_rad_s;
Distance const ConstantesPrincipal::PRECISION_LINEAIRE_DEFAUT = Constantes::PRECISION_LINEAIRE_DEFAUT;
Angle const ConstantesPrincipal::PRECISION_ANGULAIRE_DEFAUT = Constantes::PRECISION_ANGULAIRE_DEFAUT;

Distance const ConstantesPrincipal::RAYON_ROTATION_ROBOT = 230.0_mm;
Vector3m const ConstantesPrincipal::TAILLE_ROBOT = {30_cm, 30_cm, 40_cm};
uint16_t const ConstantesPrincipal::TCPIP_PORT_SIMU_DEFAUT = 4321;
Angle const ConstantesPrincipal::OFFSET_ANGLE_ADV = -0.5_PI;

int const ConstantesPrincipal::IDRobot = 1;

Duration ConstantesRobotPrincipal::getTimeoutDeplacementDefaut() const {
	return ConstantesPrincipal::TIMEOUT_DEPLACEMENT_DEFAUT;
}

Duration ConstantesRobotPrincipal::getTimeoutBlocageAdversaireDefaut() const {
	return ConstantesPrincipal::TIMEOUT_BLOCAGE_ADVERSAIRE_DEFAUT;
}

Speed ConstantesRobotPrincipal::getVitesseLineaireDefaut() const {
	return ConstantesPrincipal::VITESSE_LINEAIRE_DEFAUT;
}
AngularSpeed ConstantesRobotPrincipal::getVitesseAngulaireDefaut() const {
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
