//
// Created by benji on 14/10/16.
//

#ifndef ROOT_SERVO_H
#define ROOT_SERVO_H
#pragma once

#include "PetriCommon.h"
#include <ResultatAction.h>

// @PetriLab@ Servos

// Actions pour l'ascenseur
ResultatAction ouvrirPorteCube();
ResultatAction fermerPorteCube();

// Actions pour les soutes
ResultatAction ouvrirSouteD();
ResultatAction fermerSouteD();

ResultatAction ouvrirSouteG();
ResultatAction fermerSouteG();

// Actions pour l'abeille
ResultatAction ouvrirAbeilleD();
ResultatAction fermerAbeilleD();

ResultatAction ouvrirAbeilleG();
ResultatAction fermerAbeilleG();

#endif // ROOT_SERVO_H
