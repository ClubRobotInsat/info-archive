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

// Numeros des positions des servos et moteur asservi et les roles associes
enum PositionGriffeNum {POSITION_GRIFFE_OUVERTE = 0, POSITION_GRIFFE_BLOCAGE_PION, POSITION_GRIFFE_FERMEE, POSITION_GRIFFE_NBR};
enum PositionBrasNum {POSITION_BRAS_OUVERT = 0, POSITION_BRAS_FERME, POSITION_BRAS_NBR};

// angles des positions des servos et moteur asservi (en valeur interne du servo)
static const int positionGriffeGauche[POSITION_GRIFFE_NBR] = {700, 1240, 1530};
static const int positionGriffeDroite[POSITION_GRIFFE_NBR] = {1530, 1240, 700};

static const int positionBrasGauche[POSITION_BRAS_NBR]= {1000, 0};
static const int positionBrasDroit[POSITION_BRAS_NBR]={1000, 0};

static const int deltaPositionMoteur = 5; // > resolution sur les valeurs de position des moteurs (TODO : mettre la bonne valeur)

#endif //CONSTANTES_2011_H
