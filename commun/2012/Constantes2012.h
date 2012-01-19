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
	enum NumContacteur	{
		CONTACTEUR_TIRETTE 					= 0,
		CONTACTEUR_CHOIX_EQUIPE 			= 1,
		CONTACTEUR_USB_EPIA					= 2
	};

	///////////////////////////////////// Numéro des servos /////////////////////////////////////////////
	// Indice des différents servo de ramassage dans le tableau de positions
	enum ServosPieceTotem {
		SERVO_HAUT_DROIT 					= 0,
		SERVO_HAUT_GAUCHE					= 1,
		SERVO_BAS_DROIT						= 2,
		SERVO_BAS_GAUCHE					= 3,
		SERVO_PIECE_TOTEM_NBR				= 4
	};

	// Position des servos PieceTotem
	enum PositionServosPieceTotem {POSITION_SERVO_TOTEM_OUVERT = 0, POSITION_SERVO_TOTEM_MOITIE, POSITION_SERVO_TOTEM_FERME, POSITION_SERVO_TOTEM_NBR};
	static const int positionServosHaut[POSITION_SERVO_TOTEM_NBR] = {0,0,0}; //TBD
	static const int positionServosBas[POSITION_SERVO_TOTEM_NBR] = {0,0,0}; //TBD

	///
	// Indice des différents servo de dépose dans le tab de positions
	enum ServosCoffreCale {
		SERVO_BRAS_CALE_DROITE				= 0,
		SERVO_BRAS_CALE_GAUCHE				= 1,
		SERVO_COFFRE						= 2,
		SERVO_COFFRE_CALE_NBR				= 3
	};

	// Position des servos CoffreCale
	enum PositionServosCale {POSITION_SERVO_CALE_OUVERT = 0, POSITION_SERVO_CALE_MOITIE, POSITION_SERVO_CALE_FERME, POSITION_SERVO_CALE_NBR};
	static const int positionServosCale[POSITION_SERVO_CALE_NBR] = {0,0,0}; //TBD
	enum PositionServoCoffre {POSITION_SERVO_COFFRE_OUVERT = 0, POSITION_SERVO_COFFRE_FERME, POSITION_SERVO_COFFRE_NBR};
	static const int positionServoCoffre[POSITION_SERVO_COFFRE_NBR] = {0,0}; //TBD

	///
	// Indice des différents servo de ramassage lingot et carte dans le tab de positions
	enum ServosCarteEtLingotTotem {
		SERVO_PINCE_CARTE					= 0,
		SERVO_BRAS_CARTE 					= 1,
		SERVO_BRAS_PINCE_TOTEM_DROIT		= 2,
		SERVO_BRAS_PINCE_TOTEM_GAUCHE		= 3,
		SERVO_CARTE_ET_LINGOT_TOTEM_NBR		= 4
	};

	// Position des servos CarteEtLingotTotem
	enum PositionServosCarteEtLingotTotem {POSITION_SERVO_CARTE_LINGOT_OUVERT = 0, POSITION_SERVO_CARTE_LINGOT_MILIEU, POSITION_SERVO_CARTE_LINGOT_FERME, POSITION_SERVO_CARTE_LINGOT_NBR};
	static const int positionsServosTotem[POSITION_SERVO_CARTE_LINGOT_NBR] = {0,0,0}; // TBD
	static const int positionsServosPinceCarte[POSITION_SERVO_CARTE_LINGOT_NBR] = {0,0,0};
	static const int positionsServosBrasCarte[POSITION_SERVO_CARTE_LINGOT_NBR] = {0,0,0};

	//////////////////////////////////////////// Moteurs ///////////////////////////////////////////////////////
	// Positions possibles pour la plaque vidant le coffre (moteur)
	enum PositionsLongitudinalesGlissiereCoffre {COFFRE_OUVERT = 0, COFFRE_FERME, NBR_POSITIONS_LONGITUDINALES_COFFRE};
	static const int tableauPositionsLongitudinalesGlissiereCoffre[NBR_POSITIONS_LONGITUDINALES_COFFRE] = {0,0}; // TDB

	// Positions possibles pour la moissoneuse (moteur)
	enum PositionsMoissoneuse {MOISSONEUSE_ARRET_VERTICAL = 0, MOISSONEUSE_ARRET_HORIZONTAL, NBR_POSITIONS_MOISSONEUSE};
	static const int tableauPositionsMoissoneuse[NBR_POSITIONS_MOISSONEUSE] = {0,0}; // TDB

	// Position longitudinale la glissière pour attraper les lingots (moteur)
	enum PositionsLongitudinalesGlissiereLingot	{PINCE_RANGEE = 0, PINCE_SORTIE, PINCE_MI_RENTREE, NBR_POSITIONS_LONGITUDINALES_PINCE};
	static const int tableauPositionsLongitudinalesGlissiereLingot[NBR_POSITIONS_LONGITUDINALES_PINCE] = {0,0,0}; //ToBeDefined !!!

}

#endif //CONSTANTES_2012_H

