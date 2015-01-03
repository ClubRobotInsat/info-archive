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

	static constexpr std::uint16_t TCPIP_PORT_PAR_DEFAUT = 1234;

	static constexpr int deltaPositionServo = 20; // > résolution sur les valeurs de position des servos
	static constexpr int deltaPositionMoteur = 70; // > résolution sur les valeurs de position des moteurs (TODO : mettre la bonne valeur)

	static constexpr dureeS TIMEOUT_DEPLACEMENT_DEFAUT = 10_s;
	static constexpr vitesseM_s VITESSE_LINEAIRE_DEFAUT = 80_cm_s;
	static constexpr vitesseRad_s VITESSE_ANGULAIRE_DEFAUT = 3.25_rad_s;
	static constexpr distanceM PRECISION_LINEAIRE_DEFAUT = 5_cm;
	static constexpr angleRad PRECISION_ANGULAIRE_DEFAUT = 0.075_rad;

	static constexpr vitesseM_s VITESSE_LINEAIRE_EVITEMENT = 20_cm_s;
	static constexpr vitesseRad_s VITESSE_ANGULAIRE_EVITEMENT = 1_rad_s;

	static constexpr vitesseM_s VITESSE_LINEAIRE_RECALAGE = 20_cm_s;

	static constexpr int NOMBRE_BLOCAGE_LOGICIEL_MAX = 5;
	static constexpr int NOMBRE_BLOCAGE_PHYSIQUE_MAX = 5;

	static constexpr dureeS DUREE_MATCH = 90_s;

	static constexpr distanceM LARGEUR_TABLE = 3_m;
	static constexpr distanceM HAUTEUR_TABLE = 2_m;
})

inline Constantes2015::CouleurRobot operator!(Constantes2015::CouleurRobot const &c) {
	if(c == Constantes2015::CouleurRobot::Vert)
		return Constantes2015::CouleurRobot::Jaune;
	else if(c == Constantes2015::CouleurRobot::Jaune)
		return Constantes2015::CouleurRobot::Vert;

	return c;
}

#endif //CONSTANTES_COMMUNES_2015_H
