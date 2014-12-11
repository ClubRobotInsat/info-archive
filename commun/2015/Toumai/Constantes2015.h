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
	struct _Contacteur {
		enum type {
			CHOIX_EQUIPE= 2,
			TIRETTE		= 6,
			// TODO:
			LAMPE		= 10,
			PIED 		= 11,
			PINCE_D		= 12,
			PINCE_G		= 13
		};
	};
	typedef _Contacteur::type Contacteur;

	/////////////////// SERVOS ///////////////////
	// Indice des servos
	struct _ServoAscenseur {
		enum type {
			PINCE_D 	= 0,
			PINCE_G 	= 1,
			COUDE_D 	= 2,
			COUDE_G	 	= 3,
			NBR
		};
	};
	typedef _ServoAscenseur::type ServoAscenseur;

	struct _Servo {
		enum type {
			ARRIERE		= 0,
			BRAS_D	 	= 1,
			BRAS_G		= 2,
			NBR
		};
	};
	typedef _Servo::type Servo;

	// Position des servos
	struct _PositionPince {
		enum type {
			VERRE 		= 0,
			LAMPE		= 1,
			PIED		= 2,
			OUVERT		= 3,
			NBR
		};
	};
	typedef _PositionPince::type PositionPince;

	struct _PositionCoude {
		enum type {
			ROBOT 		= 0,
			GOBELET		= 1,
			MILIEU		= 2,
			NBR
		};
	};
	typedef _PositionCoude::type PositionCoude;

	struct _PositionArriere {
		enum type {
			FERMEE		= 0,
			MILIEU		= 1,
			OUVERT		= 2,
			NBR
		};
	};
	typedef _PositionArriere::type PositionArriere;

	struct _PositionBras {
		enum type {
			FERMEE 		= 0,
			DISTRIB		= 1,
			OUVERT		= 2,
			NBR
		};
	};
	typedef _PositionBras::type PositionBras;

	/////////////////// MOTEURS ///////////////////
	// Position des moteurs
	struct _PositionAscenseur {
		enum type {
			MAX_BAS		= 0,
			RDC 		= 1,
			ETAGE1		= 2,
			ETAGE2		= 3,
			ETAGE3		= 4,
			MAX_HAUT	= 5,
			NBR
		};
	};
	typedef _PositionAscenseur::type PositionAscenseur;

	////////////////// UTILITAIRES ///////////////////
	struct _Ascenseur {
		enum type {
			DROIT,
			GAUCHE
		};
	};
	typedef _Ascenseur::type Ascenseur;
}

STRUCT_NAMESPACE(ConstantesToumai2015Type, ConstantesToumai2015,
	{
		using Contacteur 		= ConstantesToumai2015::Contacteur;
		using ServoAscenseur 	= ConstantesToumai2015::ServoAscenseur;
		using Servo 			= ConstantesToumai2015::Servo;
		using PositionPince 	= ConstantesToumai2015::PositionPince;
		using PositionCoude 	= ConstantesToumai2015::PositionCoude;
		using PositionArriere 	= ConstantesToumai2015::PositionArriere;
		using PositionBras 		= ConstantesToumai2015::PositionBras;
		using PositionAscenseur = ConstantesToumai2015::PositionAscenseur;
		using Ascenseur 		= ConstantesToumai2015::Ascenseur;

		// Déclaration des positions
		static constexpr distanceM positionPince[PositionPince::NBR] = {950_mm, 800_mm, 1220_mm, 0_mm};
		static constexpr distanceM positionCoude[PositionCoude::NBR] = {1570_mm, 2400_mm, 1350_mm};
		static constexpr distanceM positionArriere[PositionArriere::NBR] = {1300_mm, 1790_mm, 0_mm};
		static constexpr distanceM positionBras[PositionBras::NBR] = {575_mm, 2490_mm, 0_mm};

		// Déclaration des positions
		static constexpr angleRad positionAscenseur[PositionAscenseur::NBR] = {790_mrad, 1430_mrad, 2310_mrad, 0_mrad, 0_mrad, 0_mrad};

		/////////////////// CARTES ///////////////////

		template<int ID_CARTE>
		struct CarteInfo {};
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
