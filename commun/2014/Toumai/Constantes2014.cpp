// Constantes2014.cpp

#include "Constantes2014.h"

namespace ConstantesToumai2014 {
	// Servos
	const int positionGriffeFruit[POSITION_GRIFFE_NBR] 			= {950, 800, 1220};
	// TODO : tester la position de transport
	const int positionPanierFruit[POSITION_PANIER_NBR] 			= {1570, 2400, 1350, 1700};

	const int positionPinceSaisie[POSITION_PINCE_S_NBR] 		= {1300, 1790};
	const int positionPinceRetournement[POSITION_PINCE_R_NBR] 	= {575, 2490};
	const int positionTVentouse[POSITION_T_VENTOUSE_NBR] 		= {790, 1430, 2310};
	const int positionSerpeBas[POSITION_SERPE_BAS_NBR] 			= {1950, 1730, 580};
	const int positionVideFeu[POSITION_VIDE_FEU_NBR] 			= {1980, 1340};

	// Moteurs
	const int tableauPositionsAsc[NBR_POSITIONS_ASC] =
			{/*14200*/15000, 20000, 24500, /*30671*/31000, 21500, /*17000*/17500, 2000, 0};
}

