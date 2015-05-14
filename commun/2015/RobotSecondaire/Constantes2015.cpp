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

constexpr Duree ConstantesSecondaire2015Type::TIMEOUT_DEPLACEMENT_DEFAUT;
constexpr Duree ConstantesSecondaire2015Type::TIMEOUT_BLOCAGE_ADVERSAIRE_DEFAUT;
constexpr VitesseLineaire ConstantesSecondaire2015Type::VITESSE_LINEAIRE_DEFAUT;
constexpr VitesseLineaire ConstantesSecondaire2015Type::VITESSE_LINEAIRE_LENTE;
constexpr VitesseAngulaire ConstantesSecondaire2015Type::VITESSE_ANGULAIRE_DEFAUT;
constexpr Distance ConstantesSecondaire2015Type::PRECISION_LINEAIRE_DEFAUT;
constexpr Angle ConstantesSecondaire2015Type::PRECISION_ANGULAIRE_DEFAUT;

constexpr Distance ConstantesSecondaire2015Type::RAYON_ROTATION_ROBOT;
constexpr Vector3m ConstantesSecondaire2015Type::TAILLE_ROBOT;
constexpr uint16_t ConstantesSecondaire2015Type::TCPIP_PORT_SIMU_DEFAUT;
constexpr Angle ConstantesSecondaire2015Type::OFFSET_ANGLE_ADV;
