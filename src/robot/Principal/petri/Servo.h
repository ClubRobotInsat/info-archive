//
// Created by benji on 14/10/16.
//

#ifndef ROOT_SERVO_H
#define ROOT_SERVO_H
#pragma once

#include "PetriCommon.h"
#include <ResultatAction.h>

// @Petri@ Servos

ResultatAction ascenseurStockageHaut();
ResultatAction ascenseurStockageBas();
ResultatAction ascenseurCylindreSol();
ResultatAction ascenseurAjustagePourSaisieCylindreBas();
ResultatAction ascenseurPoserRails();

// Actions pour la pince du robot
ResultatAction ouvrirPince();
ResultatAction fermerPince();
ResultatAction attraperCylindre();

ResultatAction orienterPinceAttraper();
ResultatAction orienterPinceStocker();

ResultatAction pencherPinceBas();
ResultatAction pencherPinceDevant();

// Automatisation du stockage
ResultatAction stockerCylindre(); // on considère qu'un cylindre est juste devant la pince
ResultatAction destockerCylindre();
int getNbrCylindres();
ResultatAction printCylindres();

// Gestion des fesses du robot
ResultatAction ouvrirFesseGauche();
ResultatAction ouvrirFesseDroite();
ResultatAction fermerFesseGauche();
ResultatAction fermerFesseDroite();

#endif // ROOT_SERVO_H
