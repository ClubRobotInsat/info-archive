// Constantes2014.cpp

#include "Constantes2014.h"

namespace ConstantesToumai2014 {
	// Servos
	const int positionGriffeFruit[POSITION_GRIFFE_NBR] 			= {950, 800, 1220};
	// TODO : tester la position de transport
	const int positionPanierFruit[POSITION_PANIER_NBR] 			= {1570, 2400, 1350, 1700};

	const int positionPinceSaisie[POSITION_PINCE_S_NBR] 		= {1300, /*1770*/1790};
	const int positionPinceRetournement[POSITION_PINCE_R_NBR] 	= {600, 2490};
	const int positionTVentouse[POSITION_T_VENTOUSE_NBR] 		= {800, 1430/*1460*/, /*2350*/2310};
	const int positionSerpeBas[POSITION_SERPE_BAS_NBR] 			= {1950, 1730, /*600*/580};
	const int positionVideFeu[POSITION_VIDE_FEU_NBR] 			= {1990, 1340};

	// Moteurs
	const int tableauPositionsAsc[NBR_POSITIONS_ASC] =
			{/*13889*/14200, /*19078*/22000, /*24661*/27000, 30671, 24000, 17000, 2000, 0};
}

