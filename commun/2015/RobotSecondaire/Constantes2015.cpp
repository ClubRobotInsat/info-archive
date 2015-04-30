//
//  Constantes2015.cpp
//  Club Robot
//
//  Created by Rémi on 28/10/2015.
//

#include "Constantes2015.h"

constexpr Angle ConstantesSecondaire2015Type::positionContrepoids[enumToInt(ConstantesSecondaire2015Type::PositionServo::NBR)];
constexpr Angle ConstantesSecondaire2015Type::positionTapis[enumToInt(ConstantesSecondaire2015Type::PositionServo::NBR)];

constexpr Duree ConstantesSecondaire2015Type::TIMEOUT_DEPLACEMENT_DEFAUT;
constexpr VitesseLineaire ConstantesSecondaire2015Type::VITESSE_LINEAIRE_DEFAUT;
constexpr VitesseAngulaire ConstantesSecondaire2015Type::VITESSE_ANGULAIRE_DEFAUT;
constexpr Distance ConstantesSecondaire2015Type::PRECISION_LINEAIRE_DEFAUT;
constexpr Angle ConstantesSecondaire2015Type::PRECISION_ANGULAIRE_DEFAUT;

constexpr Distance ConstantesSecondaire2015Type::RAYON_ROTATION_ROBOT;
constexpr Vector3m ConstantesSecondaire2015Type::TAILLE_ROBOT;
constexpr uint16_t ConstantesSecondaire2015Type::TCPIP_PORT_SIMU_DEFAUT;
