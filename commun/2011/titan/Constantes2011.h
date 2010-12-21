// Constantes2011.h

#ifndef CONSTANTES_2011_H
#define CONSTANTES_2011_H

#include <iostream>

enum CouleurRobot {
	COULEUR_ROBOT_ROUGE = 0,
	COULEUR_ROBOT_BLEU = 1
};

// Pour permettre l'affichage d'une Couleur :
inline std::ostream& operator<<(std::ostream& os, const CouleurRobot& couleur) 
{
	switch(couleur)
	{
	case COULEUR_ROBOT_ROUGE:
		os << "rouge";
		break;
	case COULEUR_ROBOT_BLEU:
		os << "bleu";
		break;
	}

	return os;
}

enum Bras{
	BRAS_GAUCHE = 0,
	BRAS_DROIT = 1
};

// Numeros des contacteurs et les roles associes
enum NumContacteur{
	CONTACTEUR_TIRETTE = 0,
	CONTACTEUR_CHOIX_EQUIPE = 1
};

// Numeros des servos
enum ServosNovaAscenseur{
	GRIFFE_GAUCHE = 0,
	GRIFFE_DROITE,
	PINCE_GAUCHE,
	PINCE_DROITE,
	SERVO_ASC_NBR
};

#endif //CONSTANTES_2011_H
