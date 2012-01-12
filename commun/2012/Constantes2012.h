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
		BRAS_AVANT							= 0,
		BRAS_AVANT_GAUCHE					= 1,
		BRAS_AVANT_DROIT 					= 2,
		BRAS_CALE							= 3,
		BRAS_CALE_DROITE					= 4,
		BRAS_CALE_GAUCHE					= 5,
		BRAS_CARTE							= 6
	};

	// Position des bras (générique)
	enum PositionBras {POSITION_BRAS_OUVERTE = 0, POSITION_BRAS_MOITIE, POSITION_BRAS_FERME, POSITION_BRAS_NBR};
	static const int positionBrasAvant[POSITION_BRAS_NBR] = {0,0,0}; //TBD
	static const int positionBrasCale[POSITION_BRAS_NBR] = {0,0,0}; //TBD
	static const int positionBrasCarte[POSITION_BRAS_NBR] = {0,0,0}; //TBD

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
	static const int positionPinceHaut[POSITION_PINCE_NBR] = {0,0}; //TBD
	static const int positionPinceTotem[POSITION_PINCE_NBR] = {0,0}; //TBD
	static const int positionPinceCarte[POSITION_PINCE_NBR] = {0,0}; //TBD

	// Position longitudinale des pinces pour attraper les lingots
	enum PositionsLongitudinalesPinceTotem	{PINCE_RANGEE = 0, PINCE_SORTIE, PINCE_MI_RENTREE, NBR_POSITIONS_LONGITUDINALES_PINCE};
	static const int tableauPositionsLongitudinalesPinceTotem[NBR_POSITIONS_LONGITUDINALES_PINCE] = {0,0,0}; //ToBeDefined !!!

	// Positions du servo d'ouverture du coffre
	enum ServoCoffre {SERVO_COFFRE = 0};
	enum PositionServoCoffre {POSITION_COFFRE_OUVERT = 0, POSITION_COFFRE_FERME, POSITION_COFFRE_NBR};
	static const int positionServoCoffre[POSITION_COFFRE_NBR] = {0,0};
	// Positions possibles pour la plaque vidant le coffre
	enum PositionsLongitudinalesCoffre {COFFRE_OUVERT = 0, COFFRE_FERME, NBR_POSITIONS_LONGITUDINALES_COFFRE};
	static const int tableauPositionsLongitudinalesCoffre[NBR_POSITIONS_LONGITUDINALES_COFFRE] = {0,0}; // TDB

	// Positions possibles pour la moissoneuse
	enum PositionsMoissoneuse {MOISSONEUSE_ARRET_VERTICAL = 0, MOISSONEUSE_ARRET_HORIZONTAL, NBR_POSITIONS_MOISSONEUSE};
	static const int tableauPositionsMoissoneuse[NBR_POSITIONS_MOISSONEUSE] = {0,0}; // TDB

}

#endif //CONSTANTES_2012_H

