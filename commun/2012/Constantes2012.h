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
	enum ServosAvantDroit {
		SERVO_HAUT_DROIT 					= 0,
		SERVO_PINCE_TOTEM_DROIT				= 1,
		SERVO_BAS_DROIT						= 2,
		SERVO_BRAS_CALE_DROIT				= 3,
		SERVO_AD_NBR						= 4
	};

	// Position des servos Avant Droit
	enum PositionServosAvantDroit {POSITION_SERVO_AD_OUVERT = 0, POSITION_SERVO_AD_MOITIE, POSITION_SERVO_AD_FERME, POSITION_SERVO_AD_NBR};
	static const int positionServosHautDroit[POSITION_SERVO_AD_NBR] = {0,200,500}; //TBD
	static const int positionsServosPinceTotemDroit[POSITION_SERVO_AD_NBR] = {0,200,1000}; // TBD
	static const int positionServosBasDroit[POSITION_SERVO_AD_NBR] = {0,200,500}; //TBD
	static const int positionServosCaleDroit[POSITION_SERVO_AD_NBR] = {0,200,500}; //TBD

	///
	// Indice des différents servo de dépose dans le tab de positions
	enum ServosAvantGauche {
		SERVO_HAUT_GAUCHE 					= 0,
		SERVO_PINCE_TOTEM_GAUCHE			= 1,
		SERVO_BAS_GAUCHE					= 2,
		SERVO_BRAS_CALE_GAUCHE				= 3,
		SERVO_AG_NBR						= 4
	};

	// Position des servos Avant Gauche
	enum PositionServosAvantGauche {POSITION_SERVO_AG_OUVERT = 0, POSITION_SERVO_AG_MOITIE, POSITION_SERVO_AG_FERME, POSITION_SERVO_AG_NBR};
	static const int positionServosHautGauche[POSITION_SERVO_AG_NBR] = {0,200,500}; //TBD
	static const int positionsServosPinceTotemGauche[POSITION_SERVO_AG_NBR] = {0,200,1000}; // TBD
	static const int positionServosBasGauche[POSITION_SERVO_AG_NBR] = {0,200,500}; //TBD
	static const int positionServosCaleGauche[POSITION_SERVO_AG_NBR] = {0,200,500}; //TBD


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
	static const int positionsServosTotemGauche[POSITION_SERVO_CARTE_LINGOT_NBR] = {0,200,1000}; // TBD
	static const int positionsServosPinceCarte[POSITION_SERVO_CARTE_LINGOT_NBR] = {0,200,1000};
	static const int positionsServosBrasCarte[POSITION_SERVO_CARTE_LINGOT_NBR] = {0,200,1000};

	//////////////////////////////////////////// Moteurs ///////////////////////////////////////////////////////
	// Positions possibles pour la plaque vidant le coffre (moteur)
	enum PositionsLongitudinalesGlissiereCoffre {COFFRE_OUVERT = 0, COFFRE_FERME, NBR_POSITIONS_LONGITUDINALES_COFFRE};
	static const int tableauPositionsLongitudinalesGlissiereCoffre[NBR_POSITIONS_LONGITUDINALES_COFFRE] = {0,20000}; // TDB

	// Positions possibles pour la moissoneuse (moteur)
	//enum PositionsMoissoneuse {MOISSONEUSE_ARRET_VERTICAL = 0, MOISSONEUSE_ARRET_HORIZONTAL, NBR_POSITIONS_MOISSONEUSE};
	//static const int tableauPositionsMoissoneuse[NBR_POSITIONS_MOISSONEUSE] = {0,20000}; // TDB

	// Position longitudinale la glissière pour attraper les lingots (moteur)
	enum PositionsLongitudinalesGlissiereLingot	{GLISSIERE_RANGEE = 0, GLISSIERE_SORTIE, GLISSIERE_MI_RANGEE, NBR_POSITIONS_LONGITUDINALES_GLISSIERE};
	static const int tableauPositionsLongitudinalesGlissiereLingot[NBR_POSITIONS_LONGITUDINALES_GLISSIERE] = {0,20000,30000}; //ToBeDefined !!!

  	static const int deltaPositionServo = 20; // > resolution sur les valeurs de position des servos
  	static const int deltaPositionMoteur = 70; // > resolution sur les valeurs de position des moteurs (TODO : mettre la bonne valeur)
  	//static const int vitesseRotationMoissoneuse = 4000; // > Vitesse de rotation de la moissoneuse (TODO : TDB !)
}

#endif //CONSTANTES_2012_H

