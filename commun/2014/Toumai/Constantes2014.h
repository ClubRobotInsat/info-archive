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

#define STRUCT_NAMESPACE(STRUCT_NAME, NAMESPACE_NAME, ...) \
struct STRUCT_NAME __VA_ARGS__; \
namespace NAMESPACE_NAME __VA_ARGS__

#define EXPLICIT_INSTANCIATION(SCOPE1, SCOPE2, ...) \
template<> struct SCOPE1::__VA_ARGS__ \
template<> struct SCOPE2::__VA_ARGS__

namespace IDCartesToumai2014Namespace {
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

ENUM_IMPL(IDCartes, IDCartesToumai2014Namespace);

STRUCT_NAMESPACE(IDCartesToumai2014, IDCartesToumai2014Namespace,
				 {
					 using IDCartes = IDCartesToumai2014Namespace::IDCartes;
				 }
				 )

namespace ConstantesToumai2014Namespace {
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

STRUCT_NAMESPACE(ConstantesToumai2014, ConstantesToumai2014Namespace,
				 {
					 using NumContacteur = ConstantesToumai2014Namespace::NumContacteur;
					 using ServosCarte1 = ConstantesToumai2014Namespace::ServosCarte1;
					 using ServosCarte2 = ConstantesToumai2014Namespace::ServosCarte2;
					 using PositionGriffeFruit = ConstantesToumai2014Namespace::PositionGriffeFruit;
					 using PositionPanierFruit = ConstantesToumai2014Namespace::PositionPanierFruit;
					 using PositionPinceSaisie = ConstantesToumai2014Namespace::PositionPinceSaisie;
					 using PositionPinceRetournement = ConstantesToumai2014Namespace::PositionPinceRetournement;
					 using PositionTVentouse = ConstantesToumai2014Namespace::PositionTVentouse;
					 using PositionSerpeBas = ConstantesToumai2014Namespace::PositionSerpeBas;
					 using PositionVideFeu = ConstantesToumai2014Namespace::PositionVideFeu;
					 using PositionsMaxAscenseur = ConstantesToumai2014Namespace::PositionsMaxAscenseur;
					 using PositionsAsc = ConstantesToumai2014Namespace::PositionsAsc;

					 // Déclaration des positions
					 const int positionGriffeFruit[PositionGriffeFruit::POSITION_GRIFFE_NBR] = {950, 800, 1220};
					 const int positionPanierFruit[PositionPanierFruit::POSITION_PANIER_NBR] = {1570, 2400, 1350, 1700};
					 const int positionPinceSaisie[PositionPinceSaisie::POSITION_PINCE_S_NBR] = {1300, 1790};
					 const int positionPinceRetournement[PositionPinceRetournement::POSITION_PINCE_R_NBR] = {575, 2490};
					 const int positionTVentouse[PositionTVentouse::POSITION_T_VENTOUSE_NBR] = {790, 1430, 2310};
					 const int positionSerpeBas[PositionSerpeBas::POSITION_SERPE_BAS_NBR] = {1950, 1730, 580};
					 const int positionVideFeu[PositionVideFeu::POSITION_VIDE_FEU_NBR] = {1980, 1340};

					 // Déclaration des positions
					 const int tableauPositionsAsc[PositionsAsc::NBR_POSITIONS_ASC] = {/*14200*/15000, 20000, 24500, /*30671*/31000, 21500, /*17000*/17500, 2000, 0};

					 /////////////////// CARTES ///////////////////

					 template<int ID_CARTE>
					 struct CarteInfo {};

					 //using namespace Constantes;
				 };
				 )

EXPLICIT_INSTANCIATION(ConstantesToumai2014, ConstantesToumai2014Namespace,
					   CarteInfo<IDCartesToumai2014Namespace::CAN_USB> {
						   typedef CarteCAN_USB type;
					   };
					   )
EXPLICIT_INSTANCIATION(ConstantesToumai2014, ConstantesToumai2014Namespace,
					   CarteInfo<IDCartesToumai2014Namespace::DEPLACEMENT> {
						   typedef CarteDeplacement2009 type;
					   };
					   )
EXPLICIT_INSTANCIATION(ConstantesToumai2014, ConstantesToumai2014Namespace,
					   CarteInfo<IDCartesToumai2014Namespace::ASSERVISSEMENT_ASCENSEUR> {
						   typedef CarteAsservissement2009 type;
					   };
					   )
EXPLICIT_INSTANCIATION(ConstantesToumai2014, ConstantesToumai2014Namespace,
					   CarteInfo<IDCartesToumai2014Namespace::EVITEMENT> {
						   typedef CarteDetectAdv2009 type;
					   };
					   )
EXPLICIT_INSTANCIATION(ConstantesToumai2014, ConstantesToumai2014Namespace,
					   CarteInfo<IDCartesToumai2014Namespace::IO> {
						   typedef CarteIO2014 type;
					   };
					   )
EXPLICIT_INSTANCIATION(ConstantesToumai2014, ConstantesToumai2014Namespace,
					   CarteInfo<IDCartesToumai2014Namespace::SERVOS_1> {
						   typedef CarteServosNova2009 type;
					   };
					   )
EXPLICIT_INSTANCIATION(ConstantesToumai2014, ConstantesToumai2014Namespace,
					   CarteInfo<IDCartesToumai2014Namespace::SERVOS_2> {
						   typedef CarteServosNova2009 type;
					   };
					   )
EXPLICIT_INSTANCIATION(ConstantesToumai2014, ConstantesToumai2014Namespace,
					   CarteInfo<IDCartesToumai2014Namespace::PNEUMATIQUE> {
						   typedef CartePneumatique2014 type;
					   };
					   )
EXPLICIT_INSTANCIATION(ConstantesToumai2014, ConstantesToumai2014Namespace,
					   CarteInfo<IDCartesToumai2014Namespace::DEBUG_DESSIN> {
						   typedef CarteDebugDessin type;
					   };
					   )


#endif

