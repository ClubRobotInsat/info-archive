// ConstantesCommunes2015.h

#ifndef CONSTANTES_COMMUNES_2015_H
#define CONSTANTES_COMMUNES_2015_H

#include "Commun.h"
#include "Enum/Enum.h"
#include "IConstantes.h"
#include "MathToolbox/Repere.h"
#include <array>
#include <cstdint>
#include <iostream>
#include <vector>

namespace Constantes {
	ENUM_CLASS_NS(Constantes,
	              RobotColor,
	              Blue,
	              Yellow,
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

	const repere::Repere REFERENCE_ASTAR =
	    repere::Repere({0_m, 0_m}, repere::Multiplicateur::SENS_POSITIF, repere::Multiplicateur::SENS_POSITIF);
}

inline Constantes::RobotColor operator!(Constantes::RobotColor const& c) {
	if(c == Constantes::RobotColor::Blue)
		return Constantes::RobotColor::Yellow;
	else if(c == Constantes::RobotColor::Yellow)
		return Constantes::RobotColor::Blue;

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
namespace positionObjetsTable {

	/**
	 * Objets STATIQUES
	 */
	// Murs
	const Vector3m WALLS_DIMENSIONS = {3_m, 2_m, 7_cm};
	const Length WALLS_SIZE = 2.2_cm; // épaisseur des murs

	// Positionnement des objets sur la table
	const Angle ANGLE_DELIMITER = 31.78833062_deg;

	const std::vector<repere::Coordonnees> LIST_DELIMITERS = {
	    {Vector3m(55_cm, 2_m - 54_cm, 0_m), 90_deg}, // rond en haut à gauche
	    {Vector3m(604.5_mm, 2_m - 448_mm, 0_m), ANGLE_DELIMITER},
	    {Vector3m(693.5_mm, 2_m - 448_mm, 0_m), -ANGLE_DELIMITER},
	    {Vector3m(75_cm, 2_m - 54_cm, 0_m), 90_deg},
	    {Vector3m(601_mm, 2_m - 634_mm, 0_m), -ANGLE_DELIMITER},
	    {Vector3m(693_mm, 2_m - 634_mm, 0_m), ANGLE_DELIMITER},

	    {Vector3m(225_cm, 2_m - 54_cm, 0_m), 90_deg}, // rond en haut à droite
	    {Vector3m(2304.5_mm, 2_m - 448_mm, 0_m), ANGLE_DELIMITER},
	    {Vector3m(2393.5_mm, 2_m - 448_mm, 0_m), -ANGLE_DELIMITER},
	    {Vector3m(245_cm, 2_m - 54_cm, 0_m), 90_deg},
	    {Vector3m(2301_mm, 2_m - 634_mm, 0_m), -ANGLE_DELIMITER},
	    {Vector3m(2393_mm, 2_m - 634_mm, 0_m), ANGLE_DELIMITER},

	    {Vector3m(970_mm, 2_m - 1870_mm, 0_m), 90_deg}, // rond en bas à gauche
	    {Vector3m(1024.5_mm, 2_m - 1778_mm, 0_m), ANGLE_DELIMITER},
	    {Vector3m(1113.5_mm, 2_m - 1778_mm, 0_m), -ANGLE_DELIMITER},
	    {Vector3m(1170_mm, 2_m - 1870_mm, 0_m), 90_deg},
	    {Vector3m(1021_mm, 2_m - 1964_mm, 0_m), -ANGLE_DELIMITER},
	    {Vector3m(1113_mm, 2_m - 1964_mm, 0_m), ANGLE_DELIMITER},

	    {Vector3m(1830_mm, 2_m - 1870_mm, 0_m), 90_deg}, // rond en bas à droite
	    {Vector3m(1884.5_mm, 2_m - 1778_mm, 0_m), ANGLE_DELIMITER},
	    {Vector3m(1973.5_mm, 2_m - 1778_mm, 0_m), -ANGLE_DELIMITER},
	    {Vector3m(2030_mm, 2_m - 1870_mm, 0_m), 90_deg},
	    {Vector3m(1881_mm, 2_m - 1964_mm, 0_m), -ANGLE_DELIMITER},
	    {Vector3m(1973_mm, 2_m - 1964_mm, 0_m), ANGLE_DELIMITER},


	    {Vector3m(51.5_mm - 5_cm, 2_m - 1477.5_mm, 0_m), -5.625_deg}, // arc de cercle en bas à gauche
	    {Vector3m(152.4_mm - 5_cm, 2_m - 1497.6_mm, 0_m), -16.875_deg},
	    {Vector3m(247.5_mm - 5_cm, 2_m - 1537_mm, 0_m), -28.125_deg},
	    {Vector3m(333.1_mm - 5_cm, 2_m - 1594.2_mm, 0_m), -39.375_deg},
	    {Vector3m(405.8_mm - 5_cm, 2_m - 1667_mm, 0_m), -50.625_deg},
	    {Vector3m(463_mm - 5_cm, 2_m - 1752.5_mm, 0_m), -61.875_deg},
	    {Vector3m(502.4_mm - 5_cm, 2_m - 1847_mm, 0_m), -73.125_deg},
	    {Vector3m(522.5_mm - 5_cm, 2_m - 1948.5_mm, 0_m), -84.375_deg},

	    {Vector3m(3_m - 51.5_mm - 5_cm, 2_m - 1477.5_mm, 0_m), 5.625_deg}, // arc de cercle en bas à droite
	    {Vector3m(3_m - 152.4_mm - 5_cm, 2_m - 1497.6_mm, 0_m), 16.875_deg},
	    {Vector3m(3_m - 247.5_mm - 5_cm, 2_m - 1537_mm, 0_m), 28.125_deg},
	    {Vector3m(3_m - 333.1_mm - 5_cm, 2_m - 1594.2_mm, 0_m), 39.375_deg},
	    {Vector3m(3_m - 405.8_mm - 5_cm, 2_m - 1667_mm, 0_m), 50.625_deg},
	    {Vector3m(3_m - 463_mm - 5_cm, 2_m - 1752.5_mm, 0_m), 61.875_deg},
	    {Vector3m(3_m - 502.4_mm - 5_cm, 2_m - 1847_mm, 0_m), 73.125_deg},
	    {Vector3m(3_m - 522.5_mm - 5_cm, 2_m - 1948.5_mm, 0_m), 84.375_deg}};

	// FIXME : il faut calculer les bonnes valeurs (pour le moment, elles sont approximatives)
	const std::vector<repere::Coordonnees> LIST_CENTRAL = {{Vector3m(1108.3_mm, 2_m - 1694.5_mm, 1.5_cm), 45_deg},
	                                                       {Vector3m(1184.6_mm, 2_m - 1598.4_mm, 1.5_cm), 45_deg},
	                                                       {Vector3m(1.446_m, 2_m - 1.5_m, 1.5_cm), 0_deg},
	                                                       {Vector3m(1.554_m, 2_m - 1.5_m, 1.5_cm), 0_deg},
	                                                       {Vector3m(1815.4_mm, 2_m - 1598.4_mm, 1.5_cm), -45_deg},
	                                                       {Vector3m(1891.7_mm, 2_m - 1694.5_mm, 1.5_cm), -45_deg}};

	const std::vector<repere::Coordonnees> LIST_LATERAL_H = {{Vector3m(0_m, 1.3_m, 0_m)},
	                                                         {Vector3m(0_m, 828_mm, 0_m)},
	                                                         {Vector3m(2.92_m, 1.3_m, 0_m)},
	                                                         {Vector3m(2.92_m, 828_mm, 0_m)}};

	const std::vector<repere::Coordonnees> LIST_LATERAL_V = {{Vector3m(8_cm, 85_cm, 0_cm)}, {Vector3m(2.89_m, 85_cm, 0_cm)}};

	/**
	 * Objets DYNAMIQUES
	 */
	enum ColorLunarModule {
		CYLINDER_BLUE,
		CYLINDER_YELLOW,
		CYLINDER_MULTICOLOR,
	};

	struct LunarModule {
		repere::Coordonnees coords;
		ColorLunarModule color;
	};

	const std::vector<LunarModule> LIST_LUNAR_MODULES = {
	    {{Vector3m(1.15_m, 2_m - 64_mm, 5_cm)}, CYLINDER_BLUE},
	    {{Vector3m(1.15_m, 2_m - 64_mm, 15.1_cm)}, CYLINDER_BLUE},
	    {{Vector3m(1.15_m, 2_m - 64_mm, 25.2_cm)}, CYLINDER_BLUE},
	    {{Vector3m(1.15_m, 2_m - 64_mm, 35.3_cm)}, CYLINDER_BLUE},
	    {{Vector3m(1.85_m, 2_m - 64_mm, 5_cm)}, CYLINDER_YELLOW},
	    {{Vector3m(1.85_m, 2_m - 64_mm, 15.1_cm)}, CYLINDER_YELLOW},
	    {{Vector3m(1.85_m, 2_m - 64_mm, 25.2_cm)}, CYLINDER_YELLOW},
	    {{Vector3m(1.85_m, 2_m - 64_mm, 35.3_cm)}, CYLINDER_YELLOW},
	    {{Vector3m(20_cm, 2_m - 60_cm, 5_cm)}, CYLINDER_BLUE},
	    {{Vector3m(1_m, 2_m - 60_cm, 5_cm)}, CYLINDER_MULTICOLOR},
	    {{Vector3m(2_m, 2_m - 60_cm, 5_cm)}, CYLINDER_MULTICOLOR},
	    {{Vector3m(2.8_m, 2_m - 60_cm, 5_cm)}, CYLINDER_YELLOW},
	    {{Vector3m(50_cm, 2_m - 1.1_m, 5_cm)}, CYLINDER_MULTICOLOR},
	    {{Vector3m(2.5_m, 2_m - 1.1_m, 5_cm)}, CYLINDER_MULTICOLOR},
	    {{Vector3m(64_mm, 2_m - 1.35_m, 5_cm)}, CYLINDER_MULTICOLOR},
	    {{Vector3m(64_mm, 2_m - 1.35_m, 15.1_cm)}, CYLINDER_MULTICOLOR},
	    {{Vector3m(64_mm, 2_m - 1.35_m, 25.2_cm)}, CYLINDER_MULTICOLOR},
	    {{Vector3m(64_mm, 2_m - 1.35_m, 35.3_cm)}, CYLINDER_MULTICOLOR},
	    {{Vector3m(2936_mm, 2_m - 1.35_m, 5_cm)}, CYLINDER_MULTICOLOR},
	    {{Vector3m(2936_mm, 2_m - 1.35_m, 15.1_cm)}, CYLINDER_MULTICOLOR},
	    {{Vector3m(2936_mm, 2_m - 1.35_m, 25.2_cm)}, CYLINDER_MULTICOLOR},
	    {{Vector3m(2936_mm, 2_m - 1.35_m, 35.3_cm)}, CYLINDER_MULTICOLOR},
	    {{Vector3m(90_cm, 2_m - 1.4_m, 5_cm)}, CYLINDER_MULTICOLOR},
	    {{Vector3m(2.1_m, 2_m - 1.4_m, 5_cm)}, CYLINDER_MULTICOLOR},
	    {{Vector3m(80_cm, 2_m - 1.85_m, 5_cm)}, CYLINDER_BLUE},
	    {{Vector3m(2.2_m, 2_m - 1.85_m, 5_cm)}, CYLINDER_YELLOW}};


	/// Construction de l'environnement :
}

#endif // CONSTANTES_COMMUNES_2015_H
