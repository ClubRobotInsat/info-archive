//
// Created by paul on 19/03/16.
//

#ifndef ROOT_SIMULATEURCONSTANTES_H
#define ROOT_SIMULATEURCONSTANTES_H

#include <iostream>

#include <Units.h>

#include <Color.h>
#include <Constants.h>
#include <MathToolbox/MathToolbox.h>
#include <MathToolbox/Repere.h>

using namespace Units;
using namespace UnitsLiterals;

namespace mass {
	const Mass LIGHT = 100_g;
	const Mass MEDIUM = 1_kg;
	const Mass HEAVY = 10000_kg;
} // namespace mass


/*****************************
 * Couleurs pour l'affichage *
 *****************************/

const Color3f PINK = {1.0, 0.20, 0.82};
const Color3f PURPLE = {0.6, 0.0, 0.6};
const Color3f LIGHT_BLUE = {0.06, 0.73, 0.9};
const Color3f AQUA = {0.0, 1.0, 1.0};
const Color3f BLUE = {0.07, 0.55, 0.9};
const Color3f DARK_BLUE = {0.14, 0.27, 1.0};
const Color3f DARK_GREEN = {0.19, 0.38, 0.0};
const Color3f GREEN = {0.26, 0.9, 0.06};
const Color3f LIGHT_GREEN = {0.5, 1.0, 0.0};
const Color3f YELLOW = {1.0, 1.0, 0.0};
const Color3f ORANGE = {1.0, 0.4, 0.0};
const Color3f RED = {1.0, 0.2, 0.0};

const Color3f BLACK = {0.0, 0.0, 0.0};
const Color3f GREY = {0.53, 0.53, 0.53};
const Color3f WHITE = {1.0, 1.0, 1.0};

namespace ConstantesSimu2017 {
	const Color3f COLOR_HOLD = BLACK;
	const Color3f COLOR_LUNAR_BUILDINGS = WHITE;
	const Color3f COLOR_SOL = GREY;
	const Color3f COLOR_TEAM_BLUE = BLUE;
	const Color3f COLOR_TEAM_YELLOW = YELLOW;
	const Color3f COLOR_WALL = GREY;
	const Color3f COLOR_CYLINDER_BLUE = BLUE;
	const Color3f COLOR_CYLINDER_YELLOW = YELLOW;
	const Color3f COLOR_CYLINDER_MULTICOLOR = GREY;
} // namespace ConstantesSimu2017

// Repère de la simulation
const repere::Repere REFERENCE_SIMULATOR =
    repere::Repere({0_m, 0_m}, repere::Multiplier::SENS_POSITIVE, repere::Multiplier::SENS_POSITIVE);

// Tags d'objets
const std::string TAG_ROBOT = "robot";

#endif