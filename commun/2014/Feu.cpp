/*
 * Feu.cpp
 *
 *  Created on: 13 mars 2014
 *      Author: jerk
 */

#include "Feu.h"

Feu::Feu(distanceMm x, distanceMm y, SensFeu sens, int level) : _position(x, y) {
	_sens = sens;
	_level = level;
	_couleurRight = Constantes2014::COULEUR_ROBOT_JAUNE;
	_couleurTop = Constantes2014::COULEUR_ROBOT_JAUNE;
	_couleurUp = Constantes2014::COULEUR_ROBOT_JAUNE;
	_collected = false;
}

Feu::~Feu() {
}
