/*
 * Feu.cpp
 *
 *  Created on: 13 mars 2014
 *      Author: jerk
 */

#include "Feu.h"

Feu::Feu(int x, int y, SensFeu sens, int level) {
	_x = x;
	_y = y;
	_sens = sens;
	_level = level;
	_couleurRight = Constantes2014::COULEUR_ROBOT_JAUNE;
	_couleurTop = Constantes2014::COULEUR_ROBOT_JAUNE;
	_couleurUp = Constantes2014::COULEUR_ROBOT_JAUNE;
	_collected = false;
}

Feu::~Feu() {
}

Constantes2014::CouleurRobot const Feu::changeColor(Constantes2014::CouleurRobot c) const {
	if (c == Constantes2014::CouleurRobot::COULEUR_ROBOT_JAUNE)
		return Constantes2014::CouleurRobot::COULEUR_ROBOT_ROUGE;
	else
		return Constantes2014::CouleurRobot::COULEUR_ROBOT_JAUNE;
}
