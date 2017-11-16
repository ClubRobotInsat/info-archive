//
// Created by paul on 19/03/16.
//

#ifndef ROOT_SIMULATEURCONSTANTES_H
#define ROOT_SIMULATEURCONSTANTES_H

#include "MathToolbox/Repere.h"
#include "Units/Units.h"
#include <ConstantesCommunes.h>
#include <MathToolbox/MathToolbox.h>
#include <iostream>

using namespace Units;
using namespace UnitsLiterals;

/*****************************
 * Couleurs pour l'affichage *
 *****************************/

const std::string PINK = "0xFF32D2";
const std::string PURPLE = "0x990099";
const std::string LIGHT_BLUE = "0x10BAE8";
const std::string AQUA = "0x00FFFF";
const std::string BLUE = "0x318CE7";
const std::string DARK_BLUE = "0x2346FF";
const std::string DARK_GREEN = "0x316100";
const std::string GREEN = "0x43E810";
const std::string LIGHT_GREEN = "0x82FF00";
const std::string YELLOW = "0xFEFF00";
const std::string ORANGE = "0xFF6600";
const std::string RED = "0xFF3300";

const std::string BLACK = "0x000000";
const std::string GREY = "0x888888";
const std::string WHITE = "0xFFFFFF";

namespace ConstantesSimu2017 {
	const std::string COLOR_HOLD = BLACK;
	const std::string COLOR_LUNAR_BUILDINGS = WHITE;
	const std::string COLOR_SOL = GREY;
	const std::string COLOR_TEAM_BLUE = BLUE;
	const std::string COLOR_TEAM_YELLOW = YELLOW;
	const std::string COLOR_WALL = "0x444444";
	const std::string COLOR_CYLINDER_BLUE = BLUE;
	const std::string COLOR_CYLINDER_YELLOW = YELLOW;
	const std::string COLOR_CYLINDER_MULTICOLOR = "0xDDDDDD";
}

// Repère de la simulation
const repere::Repere REFERENCE_SIMULATOR =
    repere::Repere({0_m, 0_m}, repere::Multiplicateur::SENS_POSITIF, repere::Multiplicateur::SENS_POSITIF);

#endif