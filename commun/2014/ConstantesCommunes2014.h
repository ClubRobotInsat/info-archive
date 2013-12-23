// ConstantesCommunes2014.h

#ifndef CONSTANTES_COMMUNES_2014_H
#define CONSTANTES_COMMUNES_2014_H

#include <iostream>

namespace Constantes2014 {
	
	enum CouleurRobot {
		COULEUR_ROBOT_ROUGE = 1,
		COULEUR_ROBOT_JAUNE = 0
	};
	
	// Pour permettre l'affichage d'une Couleur :
	inline std::ostream& operator<<(std::ostream& os, const CouleurRobot& couleur) {
		switch(couleur) {
			case COULEUR_ROBOT_ROUGE:
			os << "rouge";
			break;
			case COULEUR_ROBOT_JAUNE:
			os << "bleu";
			break;
		}
		
		return os;
	}
	
	extern const int deltaPositionServo; // > resolution sur les valeurs de position des servos
	extern const int deltaPositionMoteur; // > resolution sur les valeurs de position des moteurs (TODO : mettre la bonne valeur)
	
}
#endif //CONSTANTES_2013_H
