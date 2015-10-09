// ConstantesCommunes2015.h

#ifndef CONSTANTES_COMMUNES_2015_H
#define CONSTANTES_COMMUNES_2015_H

#include <iostream>
#include <cstdint>
#include "../Commun.h"
#include "../Constantes.h"
#include "../Enum/Enum.h"

namespace Constantes2015 {
	ENUM_CLASS_NS(Constantes2015, CouleurRobot,
				  Jaune,
				  Vert,
				  //Valeur d'initialisation, qui permet de déctecter si la lecture du capteur couleur a déconné.
				  Undef
	);
}

namespace Constantes2015 {
	using CouleurRobot = Constantes2015::CouleurRobot;

	extern uint16_t const TCPIP_PORT_SIMU_DEFAUT;

	extern int const deltaPositionServo; // > résolution sur les valeurs de position des servos
	extern int const deltaPositionMoteur; // > résolution sur les valeurs de position des moteurs (TODO : mettre la bonne valeur)

	extern Duree const TIMEOUT_DEPLACEMENT_DEFAUT;
    extern Duree const TIMEOUT_BLOCAGE_ADVERSAIRE_DEFAUT;
	extern VitesseLineaire const VITESSE_LINEAIRE_DEFAUT;
	extern VitesseAngulaire const VITESSE_ANGULAIRE_DEFAUT; // Unité arbitraire décidée en élec.
	extern Distance const PRECISION_LINEAIRE_DEFAUT;
	extern Angle const PRECISION_ANGULAIRE_DEFAUT;

	extern VitesseLineaire const VITESSE_LINEAIRE_EVITEMENT;
	extern VitesseAngulaire const VITESSE_ANGULAIRE_EVITEMENT;

	extern VitesseLineaire const VITESSE_LINEAIRE_RECALAGE;

	extern int const NOMBRE_BLOCAGE_LOGICIEL_MAX;
	extern int const NOMBRE_BLOCAGE_PHYSIQUE_MAX;

	extern Duree const DUREE_MATCH;

	extern Vector2m const TAILLE_TABLE;
	extern Vector2u16 const TAILLE_GRILLE;
}

inline Constantes2015::CouleurRobot operator!(Constantes2015::CouleurRobot const &c) {
	if(c == Constantes2015::CouleurRobot::Vert)
		return Constantes2015::CouleurRobot::Jaune;
	else if(c == Constantes2015::CouleurRobot::Jaune)
		return Constantes2015::CouleurRobot::Vert;

	return c;
}

struct ConstantesCommunes2015 : ConstantesCommunes {
	virtual int getNombreBlocageLogicielMax() const override;
	virtual int getNombreBlocagePhysiqueMax() const override;

	virtual Duree getDureeMatch() const override;

	virtual Vector2m getTailleTable() const override;
	virtual Vector2u16 getTailleGrille() const override;
};

#endif //CONSTANTES_COMMUNES_2015_H
