// Constantes2012.h

#ifndef CONSTANTES_2012_H
#define CONSTANTES_2012_H

#include <iostream>

enum CouleurRobot {
	COULEUR_ROBOT_ROUGE	 = 0,
	COULEUR_ROBOT_VIOLET = 1
};

// Pour permettre l'affichage d'une Couleur :
inline std::ostream& operator<<(std::ostream& os, const CouleurRobot& couleur)
{
	switch(couleur)
	{
	case COULEUR_ROBOT_ROUGE:
		os << "Rouge";
		break;
	case COULEUR_ROBOT_VIOLET:
		os << "Violet";
		break;
	}

	return os;
}

// Numeros des contacteurs et les roles associes
enum NumContacteur
{
	CONTACTEUR_TIRETTE 					= 0,
	CONTACTEUR_CHOIX_EQUIPE 			= 1,
	CONTACTEUR_USB_EPIA					= 2

};

#endif //CONSTANTES_2012_H

