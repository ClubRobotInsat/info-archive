//
//  Constantes2015.cpp
//  Club Robot
//
//  Created by Rémi on 28/10/2015.
//

#include "Constantes2015.h"

namespace ConstantesSecondaire2015 {
	Angle const positionContrepoids[enumToInt(PositionServoContrepoids::NBR)] = {1350_mrad, 1500_mrad};
	Angle const positionTapis[enumToInt(PositionServoTapis::NBR)] = {80_deg};
}

Duree const ConstantesSecondaire2015::TIMEOUT_DEPLACEMENT_DEFAUT = Constantes2015::TIMEOUT_DEPLACEMENT_DEFAUT;
Duree const ConstantesSecondaire2015::TIMEOUT_BLOCAGE_ADVERSAIRE_DEFAUT = Constantes2015::TIMEOUT_BLOCAGE_ADVERSAIRE_DEFAUT;
VitesseLineaire const ConstantesSecondaire2015::VITESSE_LINEAIRE_DEFAUT = 1_m_s;
VitesseLineaire const ConstantesSecondaire2015::VITESSE_LINEAIRE_LENTE = 20_cm_s;
VitesseAngulaire const ConstantesSecondaire2015::VITESSE_ANGULAIRE_DEFAUT = 90_deg_s;
Distance const ConstantesSecondaire2015::PRECISION_LINEAIRE_DEFAUT = Constantes2015::PRECISION_LINEAIRE_DEFAUT;
Angle const ConstantesSecondaire2015::PRECISION_ANGULAIRE_DEFAUT = Constantes2015::PRECISION_ANGULAIRE_DEFAUT;

Distance const ConstantesSecondaire2015::RAYON_ROTATION_ROBOT = 230_mm;
Vector3m const ConstantesSecondaire2015::TAILLE_ROBOT = {25_cm, 30_cm, 21_cm};
uint16_t const ConstantesSecondaire2015::TCPIP_PORT_SIMU_DEFAUT = 4321;
Angle const ConstantesSecondaire2015::OFFSET_ANGLE_ADV = 1.9_rad;

int const ConstantesSecondaire2015::IDRobot = 2;


Duree ConstantesRobotSecondaire2015::getTimeoutDeplacementDefaut() const {
	return ConstantesSecondaire2015::TIMEOUT_DEPLACEMENT_DEFAUT;
}

Duree ConstantesRobotSecondaire2015::getTimeoutBlocageAdversaireDefaut() const {
	return ConstantesSecondaire2015::TIMEOUT_BLOCAGE_ADVERSAIRE_DEFAUT;
}

VitesseLineaire ConstantesRobotSecondaire2015::getVitesseLineaireDefaut() const {
	return ConstantesSecondaire2015::VITESSE_LINEAIRE_DEFAUT;
}

VitesseAngulaire ConstantesRobotSecondaire2015::getVitesseAngulaireDefaut() const {
	return ConstantesSecondaire2015::VITESSE_ANGULAIRE_DEFAUT;
}

Distance ConstantesRobotSecondaire2015::getPrecisionLineaireDefaut() const {
	return ConstantesSecondaire2015::PRECISION_LINEAIRE_DEFAUT;
}

Angle ConstantesRobotSecondaire2015::getPrecisionAngulaireDefaut() const {
	return ConstantesSecondaire2015::PRECISION_ANGULAIRE_DEFAUT;
}

Distance ConstantesRobotSecondaire2015::getRayonRotation() const {
	return ConstantesSecondaire2015::RAYON_ROTATION_ROBOT;
}

Vector3m ConstantesRobotSecondaire2015::getTailleRobot() const {
	return ConstantesSecondaire2015::TAILLE_ROBOT;
}

Angle ConstantesRobotSecondaire2015::getOffsetAngleAdv() const {
	return ConstantesSecondaire2015::OFFSET_ANGLE_ADV;
}

uint16_t ConstantesRobotSecondaire2015::getPortTCPIPDefaut() const {
	return 1235;
}

int ConstantesRobotSecondaire2015::getIDRobot() const {
	return ConstantesSecondaire2015::IDRobot;
}
