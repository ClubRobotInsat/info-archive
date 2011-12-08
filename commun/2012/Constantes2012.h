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

namespace Constantes2012
{
	// Numeros des contacteurs et les roles associes
	enum NumContacteur
	{
		CONTACTEUR_TIRETTE 					= 0,
		CONTACTEUR_CHOIX_EQUIPE 			= 1,
		CONTACTEUR_USB_EPIA					= 2

	};

	// Numéro des bras du robot
	enum NumBras
	{
		BRAS_AVANT_GAUCHE					= 0,
		BRAS_AVANT_DROIT 					= 1,
		BRAS_CALE_DROITE					= 2,
		BRAS_CALE_GAUCHE					= 3,
		BRAS_CARTE							= 4
	};

	// Numéro des pinces du robot
	enum NumPinces
	{
		PINCE_HAUT							= 0,
		PINCE_HAUT_DROITE					= 1,
		PINCE_HAUT_GAUCHE					= 2,
		PINCE_TOTEM							= 3,
		PINCE_TOTEM_DROITE 					= 4,
		PINCE_TOTEM_GAUCHE					= 5,
		PINCE_CARTE							= 6
	};

	// Position des pinces (générique)
	enum PositionPince {POSITION_PINCE_OUVERTE = 0, POSITION_PINCE_FERMEE, POSITION_PINCE_NBR};

	// Position longitudinale des pinces pour attraper les lingots
	enum PositionsLongitudinalesPinceTotem
	{
		PINCE_RANGEE						= 0,
		PINCE_SORTIE,
		PINCE_MI_RENTREE,
		NBR_POSITIONS_LONGITUDINALES_PINCE
	};
	static const int tableauPositionLongitudinalPinceTotem[NBR_POSITIONS_LONGITUDINALES_PINCE]= {0,0,0}; //ToBeDefined !!!
}

#endif //CONSTANTES_2012_H

