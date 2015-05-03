//
//  ConstantesPrincipal.cpp
//  Club Robot
//
//  Created by Rémi on 28/10/2015.
//

#include "Constantes2015.h"

constexpr Angle ConstantesPrincipal2015Type::positionPince[enumToInt(ConstantesPrincipal2015Type::PositionPince::NBR)];
constexpr Angle ConstantesPrincipal2015Type::positionCoude[enumToInt(ConstantesPrincipal2015Type::PositionCoude::NBR)];
constexpr Angle ConstantesPrincipal2015Type::positionArriere[enumToInt(ConstantesPrincipal2015Type::PositionArriere::NBR)];
constexpr Angle ConstantesPrincipal2015Type::positionBras[enumToInt(ConstantesPrincipal2015Type::PositionBras::NBR)];
constexpr Angle ConstantesPrincipal2015Type::positionAscenseur[enumToInt(ConstantesPrincipal2015Type::PositionAscenseur::NBR)];

constexpr Duree ConstantesPrincipal2015Type::TIMEOUT_DEPLACEMENT_DEFAUT;
constexpr Duree ConstantesPrincipal2015Type::TIMEOUT_BLOCAGE_ADVERSAIRE_DEFAUT;
constexpr VitesseLineaire ConstantesPrincipal2015Type::VITESSE_LINEAIRE_DEFAUT;
constexpr VitesseAngulaire ConstantesPrincipal2015Type::VITESSE_ANGULAIRE_DEFAUT;
constexpr Distance ConstantesPrincipal2015Type::PRECISION_LINEAIRE_DEFAUT;
constexpr Angle ConstantesPrincipal2015Type::PRECISION_ANGULAIRE_DEFAUT;

constexpr Distance ConstantesPrincipal2015Type::RAYON_ROTATION_ROBOT;
constexpr Vector3m ConstantesPrincipal2015Type::TAILLE_ROBOT;
constexpr uint16_t ConstantesPrincipal2015Type::TCPIP_PORT_SIMU_DEFAUT;
