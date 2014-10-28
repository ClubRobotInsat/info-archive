// ConstantesCommunes2014.h

#ifndef CONSTANTES_COMMUNES_2014_H
#define CONSTANTES_COMMUNES_2014_H

#include <iostream>
#include <cstdint>
#include "../Enum.h"

namespace Constantes2014 {
	ENUM_NO_IMPL(CouleurRobot,
					   COULEUR_ROBOT_JAUNE /*= 0*/,
					   COULEUR_ROBOT_ROUGE /*= 1*/,
					   //Valeur d'initialisation, qui permet de déctecter si la lecture du capteur couleur a déconné.
					   COULEUR_ROBOT_UNDEF /*= 2*/
	);
}

ENUM_IMPL(CouleurRobot, Constantes2014);

STRUCT_NAMESPACE(Constantes2014Type, Constantes2014, {
	using CouleurRobot = Constantes2014::CouleurRobot;

	static constexpr std::uint16_t TCPIP_PORT_PAR_DEFAUT = 1234;

	static constexpr int deltaPositionServo = 20; // > resolution sur les valeurs de position des servos
	static constexpr int deltaPositionMoteur = 70; // > resolution sur les valeurs de position des moteurs (TODO : mettre la bonne valeur)
})

inline Constantes2014::CouleurRobot operator!(Constantes2014::CouleurRobot const &c) {
	if(c == Constantes2014::CouleurRobot::COULEUR_ROBOT_ROUGE)
		return Constantes2014::CouleurRobot::COULEUR_ROBOT_JAUNE;
	else if(c == Constantes2014::CouleurRobot::COULEUR_ROBOT_JAUNE)
		return Constantes2014::CouleurRobot::COULEUR_ROBOT_ROUGE;

	return c;
}

// Pour permettre l'affichage d'une Couleur :
inline std::ostream& operator<<(std::ostream& os, const Constantes2014::CouleurRobot& couleur) {
	switch(couleur) {
		case Constantes2014::COULEUR_ROBOT_ROUGE:
			os << "ROUGE";
			break;
		case Constantes2014::COULEUR_ROBOT_JAUNE:
			os << "JAUNE";
			break;
		case Constantes2014::COULEUR_ROBOT_UNDEF:
			os << "non initialise";
			break;
	}

	return os;
}

#endif //CONSTANTES_COMMUNES_2014_H
