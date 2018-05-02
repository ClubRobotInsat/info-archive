//
// Created by terae on 4/20/18.
//

#ifndef ROOT_MOTEUR_H
#define ROOT_MOTEUR_H
#pragma once

#include "../librobot/libRobot.h"
#include "PetriCommon.h"
#include <ResultatAction.h>

// @PetriLab@ Moteurs
ResultatAction monterAscenseursDe(int nbr_tours);
ResultatAction monterAscenseursDe(Angle value);

ResultatAction activerAvaleurs(SensAvaleurs = AVALER);
ResultatAction desactiverAvaleurs();

ResultatAction activerTurbineD();
ResultatAction activerTurbineG();

ResultatAction desactiverTurbineD();
ResultatAction desactiverTurbineG();

#endif // ROOT_MOTEUR_H
