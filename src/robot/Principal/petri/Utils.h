#pragma once

#include "PetriCommon.h"
#include <Units/Length.h>

using namespace Units;
using namespace Units::UnitsLiterals;

/**
 * Renvoie la distance réelle dx en fonction de l'angle du robot en considérant un dx pris pour la table vers le haut
 * Ex: dx=10_cm si on met le robot tourné vers +PI/2
 *     renvoie 10_cm*sin(angle_robot) en fonction de l'angle du robot pour avoir le vrai angle dx car robot de base
 * tourné en pi/2
 *
 * ATTENTION ne pas utiliser pour un AllerA en position de destination
 * sauf si l'angle de destination est le même que celui de départ
 */
Distance robotDx(Distance dx);
/**
 * Cf robot dx mais renvoie 10_cm*cos(angle_robot)
 *
 * ATTENTION ne pas utiliser pour un AllerA en position de destination
 * sauf si l'angle de destination est le même que celui de départ
 */
Distance robotDy(Distance dy);

/**
 * Transforme un int en Distance_mm
 */
Distance mm(int64_t d);

/**
 * Retourne true si la couleur de l'équipe est violet, false si elle est verte.
 */
bool couleurBleue();

int getSeashellMap();

bool cubeDetecte(CONTACTEUR contacteur);