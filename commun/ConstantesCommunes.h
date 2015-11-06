// ConstantesCommunes2015.h

#ifndef CONSTANTES_COMMUNES_2015_H
#define CONSTANTES_COMMUNES_2015_H

#include <iostream>
#include <cstdint>
#include "Commun.h"
#include "IConstantes.h"
#include "Enum/Enum.h"

namespace Constantes {
	ENUM_CLASS_NS(Constantes,
				  CouleurRobot,
				  Jaune,
				  Vert,
				  // Valeur d'initialisation, qui permet de déctecter si la lecture du capteur couleur a déconné.
				  Undef);
}

namespace Constantes {
	using CouleurRobot = Constantes::CouleurRobot;

	extern uint16_t const TCPIP_PORT_SIMU_DEFAUT;

	extern int const deltaPositionServo;  // > résolution sur les valeurs de position des servos
	extern int const deltaPositionMoteur; // > résolution sur les valeurs de position des moteurs (TODO : mettre la
										  // bonne valeur)

	extern Duration const TIMEOUT_DEPLACEMENT_DEFAUT;
	extern Duration const TIMEOUT_BLOCAGE_ADVERSAIRE_DEFAUT;
	extern Speed const VITESSE_LINEAIRE_DEFAUT;
	extern AngularSpeed const VITESSE_ANGULAIRE_DEFAUT; // Unité arbitraire décidée en élec.
	extern Distance const PRECISION_LINEAIRE_DEFAUT;
	extern Angle const PRECISION_ANGULAIRE_DEFAUT;

	extern Speed const VITESSE_LINEAIRE_EVITEMENT;
	extern AngularSpeed const VITESSE_ANGULAIRE_EVITEMENT;

	extern Speed const VITESSE_LINEAIRE_RECALAGE;

	extern int const NOMBRE_BLOCAGE_LOGICIEL_MAX;
	extern int const NOMBRE_BLOCAGE_PHYSIQUE_MAX;

	extern Duration const DUREE_MATCH;

	extern Vector2m const TAILLE_TABLE;
	extern Vector2u16 const TAILLE_GRILLE;
}

inline Constantes::CouleurRobot operator!(Constantes::CouleurRobot const &c) {
	if(c == Constantes::CouleurRobot::Vert)
		return Constantes::CouleurRobot::Jaune;
	else if(c == Constantes::CouleurRobot::Jaune)
		return Constantes::CouleurRobot::Vert;

	return c;
}

struct ConstantesCommunes : Commun::ConstantesCommunes {
	virtual int getNombreBlocageLogicielMax() const override;
	virtual int getNombreBlocagePhysiqueMax() const override;

	virtual Duration getDureeMatch() const override;

	virtual Vector2m getTailleTable() const override;
	virtual Vector2u16 getTailleGrille() const override;
};

#endif // CONSTANTES_COMMUNES_2015_H
