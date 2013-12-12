// Constantes2014.cpp

#include "Constantes2014.h"

namespace ConstantesToumai2014 {
	// angles des positions des servos et moteur asservi (en valeur interne du servo)
	const int positionServoGAscD[POSITION_PINCE_ASC_NBR] = {1050, 900, 715, 1430};
	const int positionServoGAscG[POSITION_PINCE_ASC_NBR] = {900, 1050, 1200, 1800};
	const int positionServoDAscD[POSITION_PINCE_ASC_NBR] = {1400, 1550, 1700, 2160};
	const int positionServoDAscG[POSITION_PINCE_ASC_NBR] = {1500, 1650, 1800, 980};
	
	// angles des positions des servos et moteur asservi (en valeur interne du servo)
	const int positionServoBrasGHaut[POSITION_BRAS_NBR] = {1075, 1195, 2000};
	const int positionServoBrasDHaut[POSITION_BRAS_NBR] = {848, 1020,  1793};
	const int positionServoBrasGBas[POSITION_BRAS_NBR] = {1407, 1657,  2355};
	const int positionServoBrasDBas[POSITION_BRAS_NBR] = {912, 1182,  1942};
	
	//////////////////////////////////////////// Moteurs ///////////////////////////////////////////////////////
	const int tableauPositionsAscGauche[NBR_POSITIONS_ASC] = {0, -3000, -15500, -19000};
	const int tableauPositionsAscDroit[NBR_POSITIONS_ASC] = {0, -3000, -15500, -19000};
	
	
}

