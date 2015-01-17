//
//  Constantes2015.cpp
//  Club Robot
//
//  Created by Rémi on 28/10/2015.
//

#include "Constantes2015.h"

constexpr angleRad ConstantesSecondaire2015Type::positionContrepoids[enumToInt(ConstantesSecondaire2015Type::PositionServo::NBR)];
constexpr angleRad ConstantesSecondaire2015Type::positionTapis[enumToInt(ConstantesSecondaire2015Type::PositionServo::NBR)];

constexpr dureeS ConstantesSecondaire2015Type::TIMEOUT_DEPLACEMENT_DEFAUT;
constexpr vitesseM_s ConstantesSecondaire2015Type::VITESSE_LINEAIRE_DEFAUT;
constexpr vitesseRad_s ConstantesSecondaire2015Type::VITESSE_ANGULAIRE_DEFAUT;
constexpr distanceM ConstantesSecondaire2015Type::PRECISION_LINEAIRE_DEFAUT;
constexpr angleRad ConstantesSecondaire2015Type::PRECISION_ANGULAIRE_DEFAUT;

constexpr distanceM ConstantesSecondaire2015Type::RAYON_ROTATION_ROBOT;
constexpr Vector3m ConstantesSecondaire2015Type::TAILLE_ROBOT;