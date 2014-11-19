/*
 * Feu.cpp
 *
 *  Created on: 13 mars 2015
 *      Author: jerk
 */

#include "Feu.h"

Feu::Feu(distanceM x, distanceM y, SensFeu sens, int level) : _position(x, y) {
	_sens = sens;
	_level = level;
	_couleurRight = Constantes2015::COULEUR_ROBOT_JAUNE;
	_couleurTop = Constantes2015::COULEUR_ROBOT_JAUNE;
	_couleurUp = Constantes2015::COULEUR_ROBOT_JAUNE;
	_collected = false;
}

Feu::~Feu() {
}
