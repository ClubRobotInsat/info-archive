//
//  ConstantesPrincipal.cpp
//  Club Robot
//
//  Created by Rémi on 28/10/2015.
//

#include "Constantes2015.h"

constexpr angleRad ConstantesPrincipal2015Type::positionPince[enumToInt(ConstantesPrincipal2015Type::PositionPince::NBR)];
constexpr angleRad ConstantesPrincipal2015Type::positionCoude[enumToInt(ConstantesPrincipal2015Type::PositionCoude::NBR)];
constexpr angleRad ConstantesPrincipal2015Type::positionArriere[enumToInt(ConstantesPrincipal2015Type::PositionArriere::NBR)];
constexpr angleRad ConstantesPrincipal2015Type::positionBras[enumToInt(ConstantesPrincipal2015Type::PositionBras::NBR)];
constexpr angleRad ConstantesPrincipal2015Type::positionAscenseur[enumToInt(ConstantesPrincipal2015Type::PositionAscenseur::NBR)];

constexpr dureeS ConstantesPrincipal2015Type::TIMEOUT_DEPLACEMENT_DEFAUT;
constexpr vitesseM_s ConstantesPrincipal2015Type::VITESSE_LINEAIRE_DEFAUT;
constexpr vitesseRad_s ConstantesPrincipal2015Type::VITESSE_ANGULAIRE_DEFAUT;
constexpr distanceM ConstantesPrincipal2015Type::PRECISION_LINEAIRE_DEFAUT;
constexpr angleRad ConstantesPrincipal2015Type::PRECISION_ANGULAIRE_DEFAUT;

constexpr distanceM ConstantesPrincipal2015Type::RAYON_ROTATION_ROBOT;
constexpr Vector3m ConstantesPrincipal2015Type::TAILLE_ROBOT;