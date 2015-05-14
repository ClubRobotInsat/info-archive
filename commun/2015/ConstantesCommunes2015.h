// ConstantesCommunes2015.h

#ifndef CONSTANTES_COMMUNES_2015_H
#define CONSTANTES_COMMUNES_2015_H

#include <iostream>
#include <cstdint>
#include "../Commun.h"

namespace Constantes2015 {
	ENUM_CLASS_NS(Constantes2015, CouleurRobot,
				  Jaune,
				  Vert,
				  //Valeur d'initialisation, qui permet de déctecter si la lecture du capteur couleur a déconné.
				  Undef
	);
}

STRUCT_NAMESPACE(Constantes2015Type, Constantes2015, {
	using CouleurRobot = Constantes2015::CouleurRobot;

	static constexpr uint16_t TCPIP_PORT_PAR_DEFAUT = 1234;
	static constexpr uint16_t TCPIP_PORT_SIMU_DEFAUT = 4321;

	static constexpr int deltaPositionServo = 20; // > résolution sur les valeurs de position des servos
	static constexpr int deltaPositionMoteur = 70; // > résolution sur les valeurs de position des moteurs (TODO : mettre la bonne valeur)

	static constexpr Duree TIMEOUT_DEPLACEMENT_DEFAUT = 10_s;
    static constexpr Duree TIMEOUT_BLOCAGE_ADVERSAIRE_DEFAUT = 5_s;
	static constexpr VitesseLineaire VITESSE_LINEAIRE_DEFAUT = 60_cm_s;
	static constexpr VitesseAngulaire VITESSE_ANGULAIRE_DEFAUT = 5_rad_s; // Unité arbitraire décidée en élec.
	static constexpr Distance PRECISION_LINEAIRE_DEFAUT = 5_cm;
	static constexpr Angle PRECISION_ANGULAIRE_DEFAUT = 4_deg;

	static constexpr VitesseLineaire VITESSE_LINEAIRE_EVITEMENT = 20_cm_s;
	static constexpr VitesseAngulaire VITESSE_ANGULAIRE_EVITEMENT = 1_rad_s;

	static constexpr VitesseLineaire VITESSE_LINEAIRE_RECALAGE = 20_cm_s;

	static constexpr int NOMBRE_BLOCAGE_LOGICIEL_MAX = 5;
	static constexpr int NOMBRE_BLOCAGE_PHYSIQUE_MAX = 5;

	static constexpr Duree DUREE_MATCH = 90_s;

	static constexpr Distance LARGEUR_TABLE = 3_m;
	static constexpr Distance HAUTEUR_TABLE = 2_m;

	static constexpr int GRID_SIZE_X = 300;
	static constexpr int GRID_SIZE_Y = 200;
})

inline Constantes2015::CouleurRobot operator!(Constantes2015::CouleurRobot const &c) {
	if(c == Constantes2015::CouleurRobot::Vert)
		return Constantes2015::CouleurRobot::Jaune;
	else if(c == Constantes2015::CouleurRobot::Jaune)
		return Constantes2015::CouleurRobot::Vert;

	return c;
}

#endif //CONSTANTES_COMMUNES_2015_H
