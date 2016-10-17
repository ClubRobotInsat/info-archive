// ConstantesCommunes2015.h

#ifndef CONSTANTES_COMMUNES_2015_H
#define CONSTANTES_COMMUNES_2015_H

#include "Commun.h"
#include "Enum/Enum.h"
#include "IConstantes.h"
#include <array>
#include <vector>
#include <cstdint>
#include <iostream>

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

// Murs
    const Vector3m WALLS_DIMENSIONS = {3_m, 2_m, 7_cm};
    const Length WALLS_SIZE = 2.2_cm; // épaisseur des murs

// Robot
    const Length ROBOT_Z = 51_cm;
    const Vector3m START_POSITION_ROBOT_BLUE = {1_m, 20_cm, 0.15_m};
    const Vector3m START_POSITION_ROBOT_YELLOW = {2_m, 20_cm, 0.15_m};
    const Angle START_ANGLE_ROBOT_BLUE = 90_deg;
    const Angle START_ANGLE_ROBOT_YELLOW = 90_deg;

// Position et angle des bâtons
    struct Building {
        Vector3m position;
        Angle angle;
    };

    const Angle ANGLE_DELIMITER = 31.78833062_deg;

    const std::vector<Building> LIST_DELIMITERS = {{Vector3m(55_cm, 54_cm, 0_m),       90_deg}, // rond en haut à gauche
                                                   {Vector3m(604.5_mm, 448_mm, 0_m),   -ANGLE_DELIMITER},
                                                   {Vector3m(693.5_mm, 448_mm, 0_m),   ANGLE_DELIMITER},
                                                   {Vector3m(75_cm, 54_cm, 0_m),       90_deg},
                                                   {Vector3m(601_mm, 634_mm, 0_m),     ANGLE_DELIMITER},
                                                   {Vector3m(693_mm, 634_mm, 0_m),     -ANGLE_DELIMITER},

                                                   {Vector3m(225_cm, 54_cm, 0_m),      90_deg}, // rond en haut à droite
                                                   {Vector3m(2304.5_mm, 448_mm, 0_m),  -ANGLE_DELIMITER},
                                                   {Vector3m(2393.5_mm, 448_mm, 0_m),  ANGLE_DELIMITER},
                                                   {Vector3m(245_cm, 54_cm, 0_m),      90_deg},
                                                   {Vector3m(2301_mm, 634_mm, 0_m),    ANGLE_DELIMITER},
                                                   {Vector3m(2393_mm, 634_mm, 0_m),    -ANGLE_DELIMITER},

                                                   {Vector3m(970_mm, 1870_mm, 0_m),    90_deg}, //rond en bas à gauche
                                                   {Vector3m(1024.5_mm, 1778_mm, 0_m), -ANGLE_DELIMITER},
                                                   {Vector3m(1113.5_mm, 1778_mm, 0_m), ANGLE_DELIMITER},
                                                   {Vector3m(1170_mm, 1870_mm, 0_m),   90_deg},
                                                   {Vector3m(1021_mm, 1964_mm, 0_m),   ANGLE_DELIMITER},
                                                   {Vector3m(1113_mm, 1964_mm, 0_m),   -ANGLE_DELIMITER},

                                                   {Vector3m(1830_mm, 1870_mm, 0_m),   90_deg}, // rond en bas à droite
                                                   {Vector3m(1884.5_mm, 1778_mm, 0_m), -ANGLE_DELIMITER},
                                                   {Vector3m(1973.5_mm, 1778_mm, 0_m), ANGLE_DELIMITER},
                                                   {Vector3m(2030_mm, 1870_mm, 0_m),   90_deg},
                                                   {Vector3m(1881_mm, 1964_mm, 0_m),   ANGLE_DELIMITER},
                                                   {Vector3m(1973_mm, 1964_mm, 0_m),   -ANGLE_DELIMITER}};

    // FIXME : il faut calculer les bonnes valeurs (pour le moment, elles sont approximatives)
    const std::vector<Building> LIST_CENTRAL = {{Vector3m(1.05_m, 1.38_m, 3_cm),                    -45_deg},
                                                {Vector3m(1.15_m, 1.3_m, 3_cm),                     -45_deg},
                                                {Vector3m(1.43_m, WALLS_DIMENSIONS.y - 80_cm, 0_m), 0_deg},
                                                {Vector3m(1.54_m, WALLS_DIMENSIONS.y - 80_cm, 0_m), 0_deg},
                                                {Vector3m(1.83_m, 1.3_m, 3_cm),                     45_deg},
                                                {Vector3m(1.93_m, 1.38_m, 3_cm),                    45_deg}};

    const std::vector<Vector3m> LIST_LATERAL_H = {Vector3m(0_m, 678_mm, 0_m),
                                                  Vector3m(0_m, 1.15_m, 0_m),
                                                  Vector3m(2.92_m, 678_mm, 0_m),
                                                  Vector3m(2.92_m, 1.15_m, 0_m)};

    const std::vector<Vector3m> LIST_LATERAL_V = {Vector3m(8_cm, 70_cm, 0_cm),
                                                  Vector3m(2.89_m, 70_cm, 0_cm)};
}


#endif // CONSTANTES_COMMUNES_2015_H
