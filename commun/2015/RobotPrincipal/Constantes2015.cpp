//
//  ConstantesPrincipal.cpp
//  Club Robot
//
//  Created by Rémi on 28/10/2015.
//

#include "Constantes2015.h"

namespace ConstantesPrincipal2015 {
	Angle const positionPince[enumToInt(PositionPince::NBR)] = {50_deg, 67.5_deg, 72.5_deg, 67_deg, 90_deg, 150_deg};
	Angle const positionCoude[enumToInt(PositionCoude::NBR)] = {170_deg, 40_deg, 7_deg};
	Angle const positionArriere[enumToInt(PositionArriere::NBR)] = {0_deg, 80_deg, 110_deg};
	Angle const positionBras[enumToInt(PositionBras::NBR)] = {150_deg, 57.5_deg, 30_deg};
	Angle const positionAscenseur[enumToInt(PositionAscenseur::NBR)] = {0_deg, 410_deg, 750_deg, 0_deg, 0_deg, 1000_deg};

	Distance const ENTRAXE_PINCES = 12_cm;
	Distance const DISTANCE_CENTRE_PINCES = 30_cm;
}

constexpr Duree ConstantesPrincipal2015Type::TIMEOUT_DEPLACEMENT_DEFAUT;
constexpr Duree ConstantesPrincipal2015Type::TIMEOUT_BLOCAGE_ADVERSAIRE_DEFAUT;
constexpr VitesseLineaire ConstantesPrincipal2015Type::VITESSE_LINEAIRE_DEFAUT;
constexpr VitesseAngulaire ConstantesPrincipal2015Type::VITESSE_ANGULAIRE_DEFAUT;
constexpr Distance ConstantesPrincipal2015Type::PRECISION_LINEAIRE_DEFAUT;
constexpr Angle ConstantesPrincipal2015Type::PRECISION_ANGULAIRE_DEFAUT;

constexpr Distance ConstantesPrincipal2015Type::RAYON_ROTATION_ROBOT;
constexpr Vector3m ConstantesPrincipal2015Type::TAILLE_ROBOT;
constexpr uint16_t ConstantesPrincipal2015Type::TCPIP_PORT_SIMU_DEFAUT;
constexpr Angle ConstantesPrincipal2015Type::OFFSET_ANGLE_ADV;
