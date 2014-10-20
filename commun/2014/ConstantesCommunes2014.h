// ConstantesCommunes2014.h

#ifndef CONSTANTES_COMMUNES_2014_H
#define CONSTANTES_COMMUNES_2014_H

#include <iostream>

namespace Constantes2014 {
	enum {
		TCPIP_PORT_PAR_DEFAUT = 1234
	};

	enum CouleurRobot {
		//Valeur d'initialisation, qui permet de déctecter si la lecture du capteur couleur a déconné.
		COULEUR_ROBOT_UNDEF = 2,
		COULEUR_ROBOT_ROUGE = 1,
		COULEUR_ROBOT_JAUNE = 0
	};

	inline CouleurRobot operator!(CouleurRobot const &c) {
		if(c == CouleurRobot::COULEUR_ROBOT_ROUGE)
			return CouleurRobot::COULEUR_ROBOT_JAUNE;
		else if(c == CouleurRobot::COULEUR_ROBOT_JAUNE)
			return CouleurRobot::COULEUR_ROBOT_ROUGE;
		
		return c;
	}
	
	// Pour permettre l'affichage d'une Couleur :
	inline std::ostream& operator<<(std::ostream& os, const CouleurRobot& couleur) {
		switch(couleur) {
			case COULEUR_ROBOT_ROUGE:
				os << "ROUGE";
				break;
			case COULEUR_ROBOT_JAUNE:
				os << "JAUNE";
				break;
			case COULEUR_ROBOT_UNDEF:
				os << "non initialise";
				break;
		}
		
		return os;
	}
	
	extern const int deltaPositionServo; // > resolution sur les valeurs de position des servos
	extern const int deltaPositionMoteur; // > resolution sur les valeurs de position des moteurs (TODO : mettre la bonne valeur)
	
}
#endif //CONSTANTES_COMMUNES_2014_H
