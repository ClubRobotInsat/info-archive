// ConstantesCommunes2013.h

#ifndef CONSTANTES_COMMUNES_2013_H
#define CONSTANTES_COMMUNES_2013_H

#include <iostream>


namespace Constantes2013
{
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

static const int deltaPositionServo = 20; // > resolution sur les valeurs de position des servos
static const int deltaPositionMoteur = 70; // > resolution sur les valeurs de position des moteurs (TODO : mettre la bonne valeur)

}
#endif //CONSTANTES_2013_H
