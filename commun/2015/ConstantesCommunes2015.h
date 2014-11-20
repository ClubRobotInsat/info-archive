// ConstantesCommunes2015.h

#ifndef CONSTANTES_COMMUNES_2015_H
#define CONSTANTES_COMMUNES_2015_H

#include <iostream>
#include <cstdint>
#include "../Enum.h"

namespace Constantes2015 {
	ENUM_NO_IMPL(CouleurRobot,
					   COULEUR_ROBOT_JAUNE /*= 0*/,
					   COULEUR_ROBOT_VERT /*= 1*/,
					   //Valeur d'initialisation, qui permet de déctecter si la lecture du capteur couleur a déconné.
					   COULEUR_ROBOT_UNDEF /*= 2*/
	);
}

ENUM_IMPL(CouleurRobot, Constantes2015);

STRUCT_NAMESPACE(Constantes2015Type, Constantes2015, {
	using CouleurRobot = Constantes2015::CouleurRobot;

	static constexpr std::uint16_t TCPIP_PORT_PAR_DEFAUT = 1234;

	static constexpr int deltaPositionServo = 20; // > resolution sur les valeurs de position des servos
	static constexpr int deltaPositionMoteur = 70; // > resolution sur les valeurs de position des moteurs (TODO : mettre la bonne valeur)
})

inline Constantes2015::CouleurRobot operator!(Constantes2015::CouleurRobot const &c) {
	if(c == Constantes2015::CouleurRobot::COULEUR_ROBOT_VERT)
		return Constantes2015::CouleurRobot::COULEUR_ROBOT_JAUNE;
	else if(c == Constantes2015::CouleurRobot::COULEUR_ROBOT_JAUNE)
		return Constantes2015::CouleurRobot::COULEUR_ROBOT_VERT;

	return c;
}

// Pour permettre l'affichage d'une Couleur :
inline std::ostream& operator<<(std::ostream& os, const Constantes2015::CouleurRobot& couleur) {
	switch(couleur) {
		case Constantes2015::COULEUR_ROBOT_VERT:
			os << "VERT";
			break;
		case Constantes2015::COULEUR_ROBOT_JAUNE:
			os << "JAUNE";
			break;
		case Constantes2015::COULEUR_ROBOT_UNDEF:
			os << "non initialise";
			break;
	}

	return os;
}

#endif //CONSTANTES_COMMUNES_2015_H
