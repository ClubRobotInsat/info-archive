// Constantes2015.h

#ifndef CONSTANTES_TOUMAI_2015_H
#define CONSTANTES_TOUMAI_2015_H


#ifdef TARGET_SIMULATEUR

#include "../../../simulateur/simulateur/Robot/Carte.h"
#include "../../../simulateur/cartes/CarteDeplacement2009.h"
#include "../../../simulateur/cartes/CarteContacteurs2007.h"
#include "../../../simulateur/cartes/CarteServosNova2009.h"
#include "../../../simulateur/cartes/CarteAsservissement2009.h"
#include "../../../simulateur/cartes/CarteDebugDessin.h"
#include "../../../simulateur/cartes/CartePneumatique2013.h"
#include "../../../simulateur/cartes/CarteDetectAdv2009.h"
#include "../../../simulateur/cartes/CartePneumatique2014.h"
#include "../../../simulateur/cartes/CarteIO2014.h"
#include "../../../simulateur/cartes/CarteCAN_USB.h"


#else

#include "../../../robot/Cartes/Carte.h"
#include "../../../robot/Cartes/CarteDeplacement2009.h"
#include "../../../robot/Cartes/CarteContacteurs2007.h"
#include "../../../robot/Cartes/CarteServosNova2009.h"
#include "../../../robot/Cartes/CarteAsservissement2009.h"
#include "../../../robot/Cartes/CarteDebugDessin.h"
#include "../../../robot/Cartes/CartePneumatique.h"
#include "../../../robot/Cartes/CarteDetectAdv2009.h"
#include "../../../robot/Cartes/CartePneumatique2014.h"
#include "../../../robot/Cartes/CarteIO2014.h"
#include "../../../robot/Cartes/CarteCAN_USB.h"

#endif

#include "../../Enum.h"
#include "../../MathToolbox/MathUtilities.h"

namespace IDCartesToumai2015 {
	ENUM_NO_IMPL(IDCartes,
		CAN_USB 						/*= 0*/,
		DEPLACEMENT 					/*= 1*/,
		ASSERVISSEMENT_ASCENSEUR		/*= 2*/,
		PLACEHOLDER3,
		SERVOS_1 						/*= 4*/,
		SERVOS_2	 					/*= 5*/,
		IO 								/*= 6*/,
		PLACEHOLDER7,
		EVITEMENT 						/*= 8*/,
		PNEUMATIQUE 					/*= 9*/,
		DEBUG_DESSIN 					/*= 10*/
		//ID_CARTE_DEBUG = 11
	);
}

ENUM_IMPL(IDCartes, IDCartesToumai2015);

STRUCT_NAMESPACE(IDCartesToumai2015Type, IDCartesToumai2015,
				 {
					 using IDCartes = IDCartesToumai2015::IDCartes;
				 }
				 )

namespace ConstantesToumai2015 {
	/////////////////// CONTACTEURS ///////////////////
	// Numeros des contacteurs et les roles associes
	enum NumContacteur {
		CONTACTEUR_CHOIX_EQUIPE 	= 2,
		CONTACTEUR_TIRETTE 			= 6,
		//CONTACTEUR_SERPE 			= 3,
		CONTACTEUR_TRIANGLE_SOL 	= 4,
		CONTACTEUR_SHARP_HAUT		= 7,
		CONTACTEUR_SHARP_BAS		= 9,
		//CONTACTEUR_RETOURNEMENT 	= 1,
		//CONTACTEUR_SAISIE 			= 5,
		// Contact à l'inverse ==> si 'true' alors pas de feu
		CONTACTEUR_VENTOUSE 		= 10,
		CONTACTEUR_RESERVOIR_FEU	= 11
	};

	/////////////////// SERVOS ///////////////////
	// Indice des servos
	enum ServosCarte1 {
		CARTE1_GRIFFE_FRUIT 		= 1,
		CARTE1_PANIER_FRUIT 		= 2,
		CARTE1_PINCE_SAISIE 		= 3,
		CARTE1_PINCE_RETOURNEMENT 	= 0,
		CARTE1_NBR 					= 4
	};

	enum ServosCarte2 {
		CARTE2_T_VENTOUSE	= 2,
		CARTE2_SERPE_BAS 	= 1,
		CARTE2_VIDE_FEU		= 0,
		CARTE2_NBR 			= 3
	};

	// Position des servos
	enum PositionGriffeFruit {
		POSITION_GRIFFE_INTERMEDIAIRE = 0,
		POSITION_GRIFFE_FERME,
		POSITION_GRIFFE_OUVERT,
		POSITION_GRIFFE_NBR
	};

	enum PositionPanierFruit {
		POSITION_PANIER_HORIZ = 0,
		POSITION_PANIER_VERTICALE,
		POSITION_PANIER_DECHARGEMENT,
		POSITION_PANIER_TRANSPORT,
		POSITION_PANIER_NBR
	};

	enum PositionPinceSaisie {
		POSITION_PINCE_S_OUVERTE = 0,
		POSITION_PINCE_S_FERMEE,
		POSITION_PINCE_S_NBR
	};

	enum PositionPinceRetournement {
		POSITION_PINCE_R_RECTO = 0,
		POSITION_PINCE_R_VERSO,
		POSITION_PINCE_R_NBR
	};

	enum PositionTVentouse{
		POSITION_T_VENTOUSE_IN = 0,
		POSITION_T_VENTOUSE_MID,
		POSITION_T_VENTOUSE_OUT,
		POSITION_T_VENTOUSE_NBR
	};

	enum PositionSerpeBas {
		POSITION_SERPE_BAS_RENTRE = 0,
		POSITION_SERPE_BAS_TRIANGLE,
		POSITION_SERPE_BAS_FOYER,
		POSITION_SERPE_BAS_NBR
	};

	enum PositionVideFeu {
		POSITION_VIDE_FEU_RENTRE = 0,
		POSITION_VIDE_FEU_SORTIE,
		POSITION_VIDE_FEU_NBR
	};

	enum PositionsMaxAscenseur {
		ASC_MAX_HAUT = 0,
		ASC_MAX_BAS
	};
	/////////////////// MOTEURS ///////////////////
	// Position des moteurs
	enum PositionsAsc {
		ASC_FOYER_1 = 0,
		ASC_FOYER_2,
		ASC_FOYER_3,
		ASC_FEU_SOL,
		ASC_BASCULE_FEU,
		ASC_TIRER_TRIANGLE,
		// position utile pour le passage de feu en amenuisant les frottements
		ASC_PINCE_MIN,
		ASC_PINCE,
		NBR_POSITIONS_ASC
	};
}

STRUCT_NAMESPACE(ConstantesToumai2015Type, ConstantesToumai2015,
				 {
					 using NumContacteur = ConstantesToumai2015::NumContacteur;
					 using ServosCarte1 = ConstantesToumai2015::ServosCarte1;
					 using ServosCarte2 = ConstantesToumai2015::ServosCarte2;
					 using PositionGriffeFruit = ConstantesToumai2015::PositionGriffeFruit;
					 using PositionPanierFruit = ConstantesToumai2015::PositionPanierFruit;
					 using PositionPinceSaisie = ConstantesToumai2015::PositionPinceSaisie;
					 using PositionPinceRetournement = ConstantesToumai2015::PositionPinceRetournement;
					 using PositionTVentouse = ConstantesToumai2015::PositionTVentouse;
					 using PositionSerpeBas = ConstantesToumai2015::PositionSerpeBas;
					 using PositionVideFeu = ConstantesToumai2015::PositionVideFeu;
					 using PositionsMaxAscenseur = ConstantesToumai2015::PositionsMaxAscenseur;
					 using PositionsAsc = ConstantesToumai2015::PositionsAsc;

					 // Déclaration des positions
					 static constexpr int positionGriffeFruit[PositionGriffeFruit::POSITION_GRIFFE_NBR] = {950, 800, 1220};
					 static constexpr int positionPanierFruit[PositionPanierFruit::POSITION_PANIER_NBR] = {1570, 2400, 1350, 1700};
					 static constexpr int positionPinceSaisie[PositionPinceSaisie::POSITION_PINCE_S_NBR] = {1300, 1790};
					 static constexpr int positionPinceRetournement[PositionPinceRetournement::POSITION_PINCE_R_NBR] = {575, 2490};
					 static constexpr int positionTVentouse[PositionTVentouse::POSITION_T_VENTOUSE_NBR] = {790, 1430, 2310};
					 static constexpr int positionSerpeBas[PositionSerpeBas::POSITION_SERPE_BAS_NBR] = {1950, 1730, 580};
					 static constexpr int positionVideFeu[PositionVideFeu::POSITION_VIDE_FEU_NBR] = {1980, 1340};

					 // Déclaration des positions
					 static constexpr angleRad tableauPositionsAsc[PositionsAsc::NBR_POSITIONS_ASC] = {/*14200*/15_rad, 20_rad, 24.5_rad, /*30671*/31_rad, 21.5_rad, /*17000*/17.5_rad, 2_rad, 0_rad};

					 /////////////////// CARTES ///////////////////

					 template<int ID_CARTE>
					 struct CarteInfo {};

					 //using namespace Constantes;
				 };
				 )

EXPLICIT_INSTANCIATION(ConstantesToumai2015Type, ConstantesToumai2015,
					   CarteInfo<IDCartesToumai2015::CAN_USB> {
						   typedef CarteCAN_USB type;
					   };
					   )
EXPLICIT_INSTANCIATION(ConstantesToumai2015Type, ConstantesToumai2015,
					   CarteInfo<IDCartesToumai2015::DEPLACEMENT> {
						   typedef CarteDeplacement2009 type;
					   };
					   )
EXPLICIT_INSTANCIATION(ConstantesToumai2015Type, ConstantesToumai2015,
					   CarteInfo<IDCartesToumai2015::ASSERVISSEMENT_ASCENSEUR> {
						   typedef CarteAsservissement2009 type;
					   };
					   )
EXPLICIT_INSTANCIATION(ConstantesToumai2015Type, ConstantesToumai2015,
					   CarteInfo<IDCartesToumai2015::EVITEMENT> {
						   typedef CarteDetectAdv2009 type;
					   };
					   )
EXPLICIT_INSTANCIATION(ConstantesToumai2015Type, ConstantesToumai2015,
					   CarteInfo<IDCartesToumai2015::IO> {
						   typedef CarteIO2014 type;
					   };
					   )
EXPLICIT_INSTANCIATION(ConstantesToumai2015Type, ConstantesToumai2015,
					   CarteInfo<IDCartesToumai2015::SERVOS_1> {
						   typedef CarteServosNova2009 type;
					   };
					   )
EXPLICIT_INSTANCIATION(ConstantesToumai2015Type, ConstantesToumai2015,
					   CarteInfo<IDCartesToumai2015::SERVOS_2> {
						   typedef CarteServosNova2009 type;
					   };
					   )
EXPLICIT_INSTANCIATION(ConstantesToumai2015Type, ConstantesToumai2015,
					   CarteInfo<IDCartesToumai2015::PNEUMATIQUE> {
						   typedef CartePneumatique2014 type;
					   };
					   )
EXPLICIT_INSTANCIATION(ConstantesToumai2015Type, ConstantesToumai2015,
					   CarteInfo<IDCartesToumai2015::DEBUG_DESSIN> {
						   typedef CarteDebugDessin type;
					   };
					   )


#endif

