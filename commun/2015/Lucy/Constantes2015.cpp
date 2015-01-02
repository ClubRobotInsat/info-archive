//
//  Constantes2015.cpp
//  Club Robot
//
//  Created by Rémi on 28/10/2015.
//

#include "Constantes2015.h"

constexpr angleRad ConstantesLucy2015Type::positionContrepoids[enumToInt(ConstantesLucy2015Type::PositionServo::NBR)];
constexpr angleRad ConstantesLucy2015Type::positionTapis[enumToInt(ConstantesLucy2015Type::PositionServo::NBR)];

constexpr dureeS ConstantesLucy2015Type::TIMEOUT_DEPLACEMENT_DEFAUT;
constexpr vitesseM_s ConstantesLucy2015Type::VITESSE_LINEAIRE_DEFAUT;
constexpr vitesseRad_s ConstantesLucy2015Type::VITESSE_ANGULAIRE_DEFAUT;
constexpr distanceM ConstantesLucy2015Type::PRECISION_LINEAIRE_DEFAUT;
constexpr angleRad ConstantesLucy2015Type::PRECISION_ANGULAIRE_DEFAUT;

constexpr distanceM ConstantesLucy2015Type::RAYON_ROTATION_ROBOT;
constexpr Vector3m ConstantesLucy2015Type::TAILLE_ROBOT;