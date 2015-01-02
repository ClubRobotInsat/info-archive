//
//  ConstantesToumai.cpp
//  Club Robot
//
//  Created by Rémi on 28/10/2015.
//

#include "Constantes2015.h"

constexpr angleRad ConstantesToumai2015Type::positionPince[enumToInt(ConstantesToumai2015Type::PositionPince::NBR)];
constexpr angleRad ConstantesToumai2015Type::positionCoude[enumToInt(ConstantesToumai2015Type::PositionCoude::NBR)];
constexpr angleRad ConstantesToumai2015Type::positionArriere[enumToInt(ConstantesToumai2015Type::PositionArriere::NBR)];
constexpr angleRad ConstantesToumai2015Type::positionBras[enumToInt(ConstantesToumai2015Type::PositionBras::NBR)];
constexpr angleRad ConstantesToumai2015Type::positionAscenseur[enumToInt(ConstantesToumai2015Type::PositionAscenseur::NBR)];

constexpr dureeS ConstantesToumai2015Type::TIMEOUT_DEPLACEMENT_DEFAUT;
constexpr vitesseM_s ConstantesToumai2015Type::VITESSE_LINEAIRE_DEFAUT;
constexpr vitesseRad_s ConstantesToumai2015Type::VITESSE_ANGULAIRE_DEFAUT;
constexpr distanceM ConstantesToumai2015Type::PRECISION_LINEAIRE_DEFAUT;
constexpr angleRad ConstantesToumai2015Type::PRECISION_ANGULAIRE_DEFAUT;

constexpr distanceM ConstantesToumai2015Type::RAYON_ROTATION_ROBOT;
constexpr Vector3m ConstantesToumai2015Type::TAILLE_ROBOT;