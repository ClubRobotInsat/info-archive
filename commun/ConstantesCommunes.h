// ConstantesCommunes2015.h

#ifndef CONSTANTES_COMMUNES_2015_H
#define CONSTANTES_COMMUNES_2015_H

#include "Commun.h"
#include "Enum/Enum.h"
#include "IConstantes.h"
#include <array>
#include <cstdint>
#include <iostream>

namespace Constantes {
	ENUM_CLASS_NS(Constantes,
	              RobotColor,
	              Purple,
	              Green,
	              // Valeur d'initialisation, qui permet de déctecter si la lecture du capteur couleur a déconné.
	              Undef);
}

namespace Constantes {
	using RobotColor = Constantes::RobotColor;

	extern uint16_t const TCPIP_PORT_SIMU_DEFAULT;

	extern int const deltaServoPosition;  // > résolution sur les valeurs de position des servos
	extern int const deltaEnginePosition; // > résolution sur les valeurs de position des moteurs (TODO : mettre la
	                                      // bonne valeur)

	extern Duration const TIMEOUT_MOVE_DEFAULT;
	extern Duration const TIMEOUT_OPPONENT_BLOCKING_DEFAULT;
	extern Speed const LINEAR_SPEED_DEFAULT;
	extern AngularSpeed const ANGULAR_SPEED_DEFAULT; // Unité arbitraire décidée en élec.
	extern Distance const LINEAR_PRECISION_DEFAULT;
	extern Angle const ANGULAR_PRECISION_DEFAULT;

	extern Speed const VITESSE_LINEAIRE_EVITEMENT;
	extern AngularSpeed const VITESSE_ANGULAIRE_EVITEMENT;

	extern Speed const VITESSE_LINEAIRE_RECALAGE;

	extern int const NOMBRE_BLOCAGE_LOGICIEL_MAX;
	extern int const NOMBRE_BLOCAGE_PHYSIQUE_MAX;

	extern Duration const MATCH_DURATION;

	extern Vector2m const TABLE_SIZE;
	extern Vector2u16 const TAILLE_GRILLE;
}

inline Constantes::RobotColor operator!(Constantes::RobotColor const& c) {
	if(c == Constantes::RobotColor::Green)
		return Constantes::RobotColor::Purple;
	else if(c == Constantes::RobotColor::Purple)
		return Constantes::RobotColor::Green;

	return c;
}

struct ConstantesCommunes : Commun::ConstantesCommunes {
	virtual int getNombreBlocageLogicielMax() const override;
	virtual int getNombreBlocagePhysiqueMax() const override;

	virtual Duration getMatchDuration() const override;

	virtual Vector2m getTableSize() const override;
	virtual Vector2u16 getTailleGrille() const override;
};


/*
 * Constantes des objets de la table
 */

// Murs
const Vector3m WALLS_DIMENSIONS = {3_m, 2_m, 7_cm};
const Length WALLS_SIZE = 2_cm; // épaisseur des murs

// Paravent
const Vector2m WINDBREAK_SIDE_DIMENSIONS = Vector2m(57.6_cm, 22_mm);
const Vector2m WINDBREAK_SIDE_POSITION[2] = {Vector2m(90_cm, 75_cm), Vector2m(1.524_m, 75_cm)};
const Vector2m WINDBREAK_MID_DIMENSIONS = Vector2m(48_mm, 60_cm);
const Vector2m WINDBREAK_MID_POSITION = Vector2m(1.476_m, 75_cm);

// Barrettes
const Vector2m TOP_STICK_DIMENSIONS = Vector2m(22_mm, 20_cm);
const Vector2m TOP_STICK_POSITION[2] = {Vector2m(80_cm, 0_m), Vector2m(2.178_m, 0_m)};

// Cabines de plage
const Vector3m BEACHCABIN_DIMENSIONS = {10_cm, 7.5_cm, 26.4_cm};
const Vector3m LIST_BEACHCABIN_POSITION[4] = {{30_cm, BEACHCABIN_DIMENSIONS.y * -1 / 2, BEACHCABIN_DIMENSIONS.z * 1 / 2},
                                              {60_cm, BEACHCABIN_DIMENSIONS.y * -1 / 2, BEACHCABIN_DIMENSIONS.z * 1 / 2},
                                              {2.4_m, BEACHCABIN_DIMENSIONS.y * -1 / 2, BEACHCABIN_DIMENSIONS.z * 1 / 2},
                                              {2.7_m, BEACHCABIN_DIMENSIONS.y * -1 / 2, BEACHCABIN_DIMENSIONS.z * 1 / 2}};

// Rochers
const Length ROCK_RADIUS = 25_cm;
const Vector2m ROCK_POSITION[2] = {Vector2m(0_m, 2_m), Vector2m(0_m, 3_m)};

// Cubes
const Vector3m CUBE_DIMENSIONS = {58_mm, 58_mm, 58_mm};
const int NUMBER_CUBES = 28;
const Vector3m LIST_CUBE_POSITIONS[NUMBER_CUBES] = {
    {65_cm - CUBE_DIMENSIONS.x, 90_cm - CUBE_DIMENSIONS.y, 0_m}, // bloc de 4 cubes en bas à gauche
    {65_cm - CUBE_DIMENSIONS.x, 90_cm, 0_m},
    {65_cm, 90_cm - CUBE_DIMENSIONS.y, 0_m},
    {65_cm, 90_cm, 0_m},
    {822_mm, 0_m, 0_m}, // bloc de 4 cubes en haut à gauche
    {822_mm, CUBE_DIMENSIONS.y, 0_m},
    {822_mm + CUBE_DIMENSIONS.x, CUBE_DIMENSIONS.y, 0_m},
    {822_mm + CUBE_DIMENSIONS.x, 0_m, 0_m},
    {1.5_m - 4.5 * CUBE_DIMENSIONS.x, 0_m, 0_m}, // bloc de 9+3 cubes en haut au milieu
    {1.5_m - 3.5 * CUBE_DIMENSIONS.x, 0_m, 0_m},
    {1.5_m - 2.5 * CUBE_DIMENSIONS.x, 0_m, 0_m},
    {1.5_m - 1.5 * CUBE_DIMENSIONS.x, 0_m, 0_m},
    {1.5_m - 0.5 * CUBE_DIMENSIONS.x, 0_m, 0_m},
    {1.5_m + 0.5 * CUBE_DIMENSIONS.x, 0_m, 0_m},
    {1.5_m + 1.5 * CUBE_DIMENSIONS.x, 0_m, 0_m},
    {1.5_m + 2.5 * CUBE_DIMENSIONS.x, 0_m, 0_m},
    {1.5_m + 3.5 * CUBE_DIMENSIONS.x, 0_m, 0_m},
    {1.5_m - 1.5 * CUBE_DIMENSIONS.x, CUBE_DIMENSIONS.y, 0_m},
    {1.5_m - 0.5 * CUBE_DIMENSIONS.x, CUBE_DIMENSIONS.y, 0_m},
    {1.5_m + 0.5 * CUBE_DIMENSIONS.x, CUBE_DIMENSIONS.y, 0_m},
    {2.178_m - CUBE_DIMENSIONS.x, 0_m, 0_m}, // bloc de 4 cubes en haut à droite
    {2.178_m - 2 * CUBE_DIMENSIONS.x, CUBE_DIMENSIONS.y, 0_m},
    {2.178_m - CUBE_DIMENSIONS.x, CUBE_DIMENSIONS.y, 0_m},
    {2.178_m - 2 * CUBE_DIMENSIONS.x, 0_m, 0_m},
    {2.35_m - CUBE_DIMENSIONS.x, 90_cm - CUBE_DIMENSIONS.y, 0_m}, // bloc de 4 cubes en bas à droite
    {2.35_m - CUBE_DIMENSIONS.x, 90_cm, 0_m},
    {2.35_m, 90_cm - CUBE_DIMENSIONS.y, 0_m},
    {2.35_m, 90_cm, 0_m}};

// Cylindres
const Length CYLINDER_RADIUS = 58_mm;
const Length CYLINDER_Z = 58_mm;
const int NUMBER_CYLINDERS = 1;
const Vector3m LIST_CYLINDER_POSITIONS[NUMBER_CYLINDERS] = {{1.5_m, 2 * CUBE_DIMENSIONS.y + CYLINDER_RADIUS, 0_m}};


// Coquillages
const int NUMBER_SEASHELLS = 16;
struct Seashell {
	enum Color { PURPLE, GREEN, WHITE };
	static constexpr Length RADIUS = 76.2_mm;
	static constexpr Length Z = 2.5_cm;
	Color _color;
	Vector3m _position;
};

struct SeashellMap {
	Seashell _seashells[NUMBER_SEASHELLS];
};

const std::array<SeashellMap, 5> LIST_SEASHELL_MAP = {{// Carte 1
                                                       SeashellMap{{Seashell{Seashell::PURPLE, {7.5_cm, 20_cm, 4.4_cm}},
                                                                    Seashell{Seashell::PURPLE, {7.5_cm, 7.5_cm, 6.6_cm}},
                                                                    Seashell{Seashell::PURPLE, {20_cm, 7.5_cm, 4.4_cm}},
                                                                    Seashell{Seashell::WHITE, {20_cm, 1.55_m, 0_m}},
                                                                    Seashell{Seashell::WHITE, {20_cm, 1.25_m, 0_m}},
                                                                    Seashell{Seashell::PURPLE, {90_cm, 1.45_m, 0_m}},
                                                                    Seashell{Seashell::GREEN, {1.2_m, 1.65_m, 0_m}},
                                                                    Seashell{Seashell::WHITE, {1.5_m, 1.55_m, 0_m}},
                                                                    Seashell{Seashell::WHITE, {1.5_m, 1.85_m, 0_m}},
                                                                    Seashell{Seashell::PURPLE, {1.8_m, 1.65_m, 0_m}},
                                                                    Seashell{Seashell::GREEN, {2.1_m, 1.45_m, 0_m}},
                                                                    Seashell{Seashell::WHITE, {2.8_m, 1.55_m, 0_m}},
                                                                    Seashell{Seashell::WHITE, {2.8_m, 1.25_m, 0_m}},
                                                                    Seashell{Seashell::GREEN, {2.825_m, 20_cm, 4.4_cm}},
                                                                    Seashell{Seashell::GREEN, {2.825_m, 7.5_cm, 6.6_cm}},
                                                                    Seashell{Seashell::GREEN, {2.8_m, 7.5_cm, 4.4_cm}}}},
                                                       // Carte 2
                                                       SeashellMap{{Seashell{Seashell::PURPLE, {7.5_cm, 20_cm, 4.4_cm}},
                                                                    Seashell{Seashell::WHITE, {7.5_cm, 7.5_cm, 6.6_cm}},
                                                                    Seashell{Seashell::PURPLE, {20_cm, 7.5_cm, 4.4_cm}},
                                                                    Seashell{Seashell::WHITE, {20_cm, 1.55_m, 0_m}},
                                                                    Seashell{Seashell::PURPLE, {20_cm, 1.25_m, 0_m}},
                                                                    Seashell{Seashell::PURPLE, {90_cm, 1.45_m, 0_m}},
                                                                    Seashell{Seashell::PURPLE, {1.2_m, 1.65_m, 0_m}},
                                                                    Seashell{Seashell::WHITE, {1.5_m, 1.55_m, 0_m}},
                                                                    Seashell{Seashell::WHITE, {1.5_m, 1.85_m, 0_m}},
                                                                    Seashell{Seashell::GREEN, {1.8_m, 1.65_m, 0_m}},
                                                                    Seashell{Seashell::GREEN, {2.1_m, 1.45_m, 0_m}},
                                                                    Seashell{Seashell::WHITE, {2.8_m, 1.55_m, 0_m}},
                                                                    Seashell{Seashell::GREEN, {2.8_m, 1.25_m, 0_m}},
                                                                    Seashell{Seashell::GREEN, {2.825_m, 20_cm, 4.4_cm}},
                                                                    Seashell{Seashell::WHITE, {2.825_m, 7.5_cm, 6.6_cm}},
                                                                    Seashell{Seashell::GREEN, {2.8_m, 7.5_cm, 4.4_cm}}}},
                                                       // Carte 3
                                                       SeashellMap{{Seashell{Seashell::PURPLE, {7.5_cm, 20_cm, 4.4_cm}},
                                                                    Seashell{Seashell::WHITE, {7.5_cm, 7.5_cm, 6.6_cm}},
                                                                    Seashell{Seashell::PURPLE, {20_cm, 7.5_cm, 4.4_cm}},
                                                                    Seashell{Seashell::WHITE, {20_cm, 1.55_m, 0_m}},
                                                                    Seashell{Seashell::PURPLE, {20_cm, 1.25_m, 0_m}},
                                                                    Seashell{Seashell::WHITE, {70_cm, 1.55_m, 0_m}},
                                                                    Seashell{Seashell::PURPLE, {70_cm, 1.25_m, 0_m}},
                                                                    Seashell{Seashell::PURPLE, {1.2_m, 1.65_m, 0_m}},
                                                                    Seashell{Seashell::GREEN, {1.8_m, 1.65_m, 0_m}},
                                                                    Seashell{Seashell::WHITE, {2.3_m, 1.55_m, 0_m}},
                                                                    Seashell{Seashell::GREEN, {2.3_m, 1.25_m, 0_m}},
                                                                    Seashell{Seashell::WHITE, {2.8_m, 1.55_m, 0_m}},
                                                                    Seashell{Seashell::GREEN, {2.8_m, 1.25_m, 0_m}},
                                                                    Seashell{Seashell::GREEN, {2.825_m, 20_cm, 4.4_cm}},
                                                                    Seashell{Seashell::WHITE, {2.825_m, 7.5_cm, 6.6_cm}},
                                                                    Seashell{Seashell::GREEN, {2.8_m, 7.5_cm, 4.4_cm}}}},
                                                       // Carte 4
                                                       SeashellMap{{Seashell{Seashell::PURPLE, {7.5_cm, 20_cm, 4.4_cm}},
                                                                    Seashell{Seashell::WHITE, {7.5_cm, 7.5_cm, 6.6_cm}},
                                                                    Seashell{Seashell::WHITE, {20_cm, 7.5_cm, 4.4_cm}},
                                                                    Seashell{Seashell::PURPLE, {20_cm, 1.55_m, 0_m}},
                                                                    Seashell{Seashell::PURPLE, {20_cm, 1.25_m, 0_m}},
                                                                    Seashell{Seashell::WHITE, {70_cm, 1.85_m, 0_m}},
                                                                    Seashell{Seashell::GREEN, {70_cm, 1.55_m, 0_m}},
                                                                    Seashell{Seashell::PURPLE, {70_cm, 1.25_m, 0_m}},
                                                                    Seashell{Seashell::WHITE, {2.3_m, 1.85_m, 0_m}},
                                                                    Seashell{Seashell::PURPLE, {2.3_m, 1.55_m, 0_m}},
                                                                    Seashell{Seashell::GREEN, {2.3_m, 1.25_m, 0_m}},
                                                                    Seashell{Seashell::GREEN, {2.8_m, 1.55_m, 0_m}},
                                                                    Seashell{Seashell::GREEN, {2.8_m, 1.25_m, 0_m}},
                                                                    Seashell{Seashell::GREEN, {2.825_m, 20_cm, 4.4_cm}},
                                                                    Seashell{Seashell::WHITE, {2.825_m, 7.5_cm, 6.6_cm}},
                                                                    Seashell{Seashell::WHITE, {2.8_m, 7.5_cm, 4.4_cm}}}},
                                                       // Carte 5
                                                       SeashellMap{{Seashell{Seashell::GREEN, {7.5_cm, 20_cm, 4.4_cm}},
                                                                    Seashell{Seashell::WHITE, {7.5_cm, 7.5_cm, 6.6_cm}},
                                                                    Seashell{Seashell::GREEN, {20_cm, 7.5_cm, 4.4_cm}},
                                                                    Seashell{Seashell::PURPLE, {20_cm, 1.55_m, 0_m}},
                                                                    Seashell{Seashell::PURPLE, {20_cm, 1.25_m, 0_m}},
                                                                    Seashell{Seashell::WHITE, {70_cm, 1.55_m, 0_m}},
                                                                    Seashell{Seashell::PURPLE, {70_cm, 1.25_m, 0_m}},
                                                                    Seashell{Seashell::WHITE, {1.2_m, 1.65_m, 0_m}},
                                                                    Seashell{Seashell::WHITE, {1.8_m, 1.65_m, 0_m}},
                                                                    Seashell{Seashell::WHITE, {2.3_m, 1.55_m, 0_m}},
                                                                    Seashell{Seashell::GREEN, {2.3_m, 1.25_m, 0_m}},
                                                                    Seashell{Seashell::GREEN, {2.8_m, 1.55_m, 0_m}},
                                                                    Seashell{Seashell::GREEN, {2.8_m, 1.25_m, 0_m}},
                                                                    Seashell{Seashell::PURPLE, {2.825_m, 20_cm, 4.4_cm}},
                                                                    Seashell{Seashell::WHITE, {2.825_m, 7.5_cm, 6.6_cm}},
                                                                    Seashell{Seashell::PURPLE, {2.8_m, 7.5_cm, 4.4_cm}}}}}};
std::vector<Seashell> getListSeashells(Constantes::RobotColor color, int map);

// Robot
const Length ROBOT_Z = 51_cm;
const Vector3m START_POSITION_ROBOT_PURPLE = {21_cm,
                                              0.751_m,
                                              0.15_m}; // => Vector2m(robotSize.x / 2 + 2_cm, 60.1_cm + robotSize.y / 2
const Vector3m START_POSITION_ROBOT_GREEN = {2.79_m, 0.751_m, 0.15_m}; // TODO : avoir la meme formule que dans
                                                                       // info/robot/Principal/IA/IAPrincipal.cpp -
                                                                       // initialisation (au setRepere())
const Angle START_ANGLE_ROBOT_PURPLE = 0_deg;
const Angle START_ANGLE_ROBOT_GREEN = 180_deg;

#endif // CONSTANTES_COMMUNES_2015_H
