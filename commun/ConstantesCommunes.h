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

// Murs
const Vector3m WALLS_DIMENSIONS = {3_m, 2_m, 7_cm};
const Length WALLS_SIZE = 2_cm; // épaisseur des murs

// Robot
const Length ROBOT_Z = 51_cm;
const Vector3m START_POSITION_ROBOT_BLUE = {60_cm, 0.751_m, 0.15_m};
const Vector3m START_POSITION_ROBOT_YELLOW = {2.4_m, 0.751_m, 0.15_m};
const Angle START_ANGLE_ROBOT_BLUE = 90_deg;
const Angle START_ANGLE_ROBOT_YELLOW = 90_deg;

#endif // CONSTANTES_COMMUNES_2015_H
