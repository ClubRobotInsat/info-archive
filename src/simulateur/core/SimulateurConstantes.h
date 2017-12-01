//
// Created by paul on 19/03/16.
//

#ifndef ROOT_SIMULATEURCONSTANTES_H
#define ROOT_SIMULATEURCONSTANTES_H

#include <iostream>

#include <Units.h>

#include <ConstantesCommunes.h>
#include <MathToolbox/MathToolbox.h>
#include <MathToolbox/Repere.h>

using namespace Units;
using namespace UnitsLiterals;

namespace mass {
	const Mass LIGHT = 100_g;
	const Mass MEDIUM = 1_kg;
	const Mass HEAVY = 10000_kg;
}


/*****************************
 * Couleurs pour l'affichage *
 *****************************/

// TODO [Important] Convertir tout ça en objets couleurs (Vector3f ou, mieux, Color3f)
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
	const Vector3f COLOR_HOLD = {0, 0, 0};
	const Vector3f COLOR_LUNAR_BUILDINGS = {1, 1, 1};
	const Vector3f COLOR_SOL = {0.5, 0.5, 0.5};
	const Vector3f COLOR_TEAM_BLUE = {0, 0, 1};
	const Vector3f COLOR_TEAM_YELLOW = {0.9, 1, 0};
	const Vector3f COLOR_WALL = {0.5, 0.5, 0.5};
	const Vector3f COLOR_CYLINDER_BLUE = {0, 0, 1};
	const Vector3f COLOR_CYLINDER_YELLOW = {0.9, 1, 0};
	const Vector3f COLOR_CYLINDER_MULTICOLOR = {0.5, 0.5, 0.5};
}

// Repère de la simulation
const repere::Repere REFERENCE_SIMULATOR =
    repere::Repere({0_m, 0_m}, repere::Multiplicateur::SENS_POSITIF, repere::Multiplicateur::SENS_POSITIF);

#endif