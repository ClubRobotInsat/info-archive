// Constantes2014.h

#ifndef CONSTANTES_TOUMAI_2014_H
#define CONSTANTES_TOUMAI_2014_H


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


#else

#include "../../../robot/Robot/Carte.h"
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

namespace ConstantesToumai2014 {
	
	/////////////////// CONTACTEURS ///////////////////
	// Numeros des contacteurs et les roles associes
	enum NumContacteur {
		CONTACTEUR_CHOIX_EQUIPE 	= 2,
		CONTACTEUR_TIRETTE 			= 6,
		CONTACTEUR_SERPE 			= 3,
		CONTACTEUR_TRIANGLE_SOL 	= 4,
		CONTACTEUR_SHARP_HAUT		= 7,
		CONTACTEUR_SHARP_BAS		= 9,
		CONTACTEUR_RETOURNEMENT 	= 1,
		CONTACTEUR_SAISIE 			= 5,
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

	// Déclaration des positions
	extern const int positionGriffeFruit[POSITION_GRIFFE_NBR];
	extern const int positionPanierFruit[POSITION_PANIER_NBR];
	extern const int positionPinceSaisie[POSITION_PINCE_S_NBR];
	extern const int positionPinceRetournement[POSITION_PINCE_R_NBR];
	extern const int positionTVentouse[POSITION_T_VENTOUSE_NBR];
	extern const int positionSerpeBas[POSITION_SERPE_BAS_NBR];
	extern const int positionVideFeu[POSITION_VIDE_FEU_NBR];
	
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
	
	// Déclaration des positions
	extern const int tableauPositionsAsc[NBR_POSITIONS_ASC];
	
	/////////////////// CARTES ///////////////////
	namespace IDCartesToumai2014 {
		enum {
			ID_CARTE_CAN_USB = 0,
			PREMIER_ID_CARTE 						= 1,
			ID_CARTE_DEPLACEMENT 					= PREMIER_ID_CARTE,
			ID_CARTE_ASSERVISSEMENT_ASCENSEUR		= 2,
			ID_CARTE_SERVOS_1 						= 4,
			ID_CARTE_SERVOS_2	 					= 5,
			ID_CARTE_IO 							= 6,
			ID_CARTE_EVITEMENT 						= 8,
			ID_CARTE_PNEUMATIQUE 					= 9,
//			ID_CARTE_DEBUG_DESSIN 					= 10,
			DERNIER_ID_CARTE
			//ID_CARTE_DEBUG = 11
		};
	}
	using namespace IDCartesToumai2014;
	
	template<int ID_CARTE>
	struct CarteToumai {};
	
	/*template<>
	 struct CarteToumai<ID_CARTE_CAN_USB> {
	 typedef int type;
	 };*/

	
#ifndef TARGET_SIMULATEUR
	template<>
		struct CarteToumai<ID_CARTE_CAN_USB> {
			typedef CarteCAN_USB type;
	};
#endif

	template<>
	struct CarteToumai<ID_CARTE_DEPLACEMENT> {
		typedef CarteDeplacement2009 type;
		static constexpr char const *name = "Carte déplacement";
	};
	template<>
	struct CarteToumai<ID_CARTE_ASSERVISSEMENT_ASCENSEUR> {
		typedef CarteAsservissement2009 type;
	};
	template<>
	struct CarteToumai<ID_CARTE_EVITEMENT> {
		typedef CarteDetectAdv2009 type;
	};
	template<>
	struct CarteToumai<ID_CARTE_IO> {
		typedef CarteIO2014 type;
	};
	template<>
	struct CarteToumai<ID_CARTE_SERVOS_1> {
		typedef CarteServosNova2009 type;
	};
	template<>
	struct CarteToumai<ID_CARTE_SERVOS_2> {
		typedef CarteServosNova2009 type;
	};
	template<>
	struct CarteToumai<ID_CARTE_PNEUMATIQUE> {
		typedef CartePneumatique2014 type;
	};
//	template<>
//	struct CarteToumai<ID_CARTE_DEBUG_DESSIN> {
//		typedef CarteDebugDessin type;
//	};
}
#endif

