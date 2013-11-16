// Constantes2014.h

#ifndef CONSTANTES_2014_H
#define CONSTANTES_2014_H

namespace ConstantesChronos2014 {
	
	// Numeros des contacteurs et les roles associes
	enum NumContacteur	{
		CONTACTEUR_TIRETTE 			= 1,
		CONTACTEUR_CHOIX_EQUIPE     = 0
	};
	
	enum NumContacteurAsc{
		CONTACTEUR_GAUCHE_ASCG	= 0,
		CONTACTEUR_DROIT_ASCG	= 1,
		CONTACTEUR_GAUCHE_ASCD 	= 2,
		CONTACTEUR_DROIT_ASCD	= 3
	};
	
	// Indice des différents servo de ramassage dans le tableau de positions
	enum ServosAsc {
		SERVO_G_ASC_G     		= 0,
		SERVO_G_ASC_D        	= 1,
		SERVO_D_ASC_D      		= 2,
		SERVO_D_ASC_G        	= 3,
		SERVO_ASC_NBR         	= 4
	};
	
	// Indice des différents servo de ramassage dans le tableau de positions
	enum ServosBras {
		SERVO_BRAS_G_HAUT       	= 0,
		SERVO_BRAS_D_HAUT     		= 1,
		SERVO_BRAS_G_BAS      		= 2,
		SERVO_BRAS_D_BAS        	= 3,
		SERVO_BRAS_NBR  	       	= 4
	};
	
	// Numeros des positions des servos et moteur asservi et les roles associes
	enum PositionServosPinceAsc {
		POSITION_PINCE_ASC_OUVERTE = 0,
		POSITION_PINCE_ASC_EMPILE,
		POSITION_PINCE_ASC_BLOCAGE,
		POSITION_PINCE_ASC_RANGE,
		POSITION_PINCE_ASC_NBR};
	
	enum PositionServosBras {
		POSITION_BRAS_HAUT = 0,
		POSITION_BRAS_SOUFFLE_BOUGIE,
		POSITION_BRAS_RANGE,
		POSITION_BRAS_NBR};
	
	// angles des positions des servos et moteur asservi (en valeur interne du servo)
	//static const int positionServoGAscD[POSITION_PINCE_ASC_NBR] = {980, 880, 780, 1430};
	extern const int positionServoGAscD[POSITION_PINCE_ASC_NBR];
	//static const int positionServoGAscG[POSITION_PINCE_ASC_NBR] = {1200, 1100, 1000, 600};
	extern const int positionServoGAscG[POSITION_PINCE_ASC_NBR];
	//static const int positionServoDAscD[POSITION_PINCE_ASC_NBR] = {1510, 1610, 1710, 2160};
	extern const int positionServoDAscD[POSITION_PINCE_ASC_NBR];
	//static const int positionServoDAscG[POSITION_PINCE_ASC_NBR] = {1500, 1600, 1750, 980};
	extern const int positionServoDAscG[POSITION_PINCE_ASC_NBR];
	
	// angles des positions des servos et moteur asservi (en valeur interne du servo)
	extern const int positionServoBrasGHaut[POSITION_BRAS_NBR];
	extern const int positionServoBrasDHaut[POSITION_BRAS_NBR];
	extern const int positionServoBrasGBas[POSITION_BRAS_NBR];
	extern const int positionServoBrasDBas[POSITION_BRAS_NBR];
	
	//////////////////////////////////////////// Moteurs ///////////////////////////////////////////////////////
	
	// Positions possibles pour l'ascenseur gauche et droit
	enum PositionsAsc {
		ASC_HAUT = 0,
		ASC_MILIEU,
		ASC_MAGIC,
		ASC_BAS,
		NBR_POSITIONS_ASC
	};
	
	extern const int tableauPositionsAscGauche[NBR_POSITIONS_ASC];
	extern const int tableauPositionsAscDroit[NBR_POSITIONS_ASC];
	
	
}
#endif

