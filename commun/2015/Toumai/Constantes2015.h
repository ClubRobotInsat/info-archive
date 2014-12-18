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
		ASSERVISSEMENT_ASCENSEUR_D		/*= 2*/,
		ASSERVISSEMENT_ASCENSEUR_G		/*= 3*/,
		SERVOS_ASCENSEUR				/*= 4*/,
		SERVOS_AUTRES					/*= 5*/,
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
	enum class Contacteur {
		CHOIX_EQUIPE= 2,
		TIRETTE		= 6,
		// TODO:
		LAMPE		= 10,
		PIED 		= 11,
		PINCE_D		= 12,
		PINCE_G		= 13
	};

	/////////////////// SERVOS ///////////////////
	// Indice des servos
	enum class ServoAscenseur {
		PINCE_D 	= 0,
		PINCE_G 	= 1,
		COUDE_D 	= 2,
		COUDE_G	 	= 3,
		NBR
	};

	enum class Servo {
		ARRIERE		= 0,
		BRAS_D	 	= 1,
		BRAS_G		= 2,
		NBR
	};

	// Position des servos
	enum class PositionPince {
		VERRE 		= 0,
		LAMPE		= 1,
		PIED		= 2,
		OUVERT		= 3,
		NBR
	};

	enum class PositionCoude {
		ROBOT 		= 0,
		GOBELET		= 1,
		MILIEU		= 2,
		NBR
	};

	enum class PositionArriere {
		FERME		= 0,
		MILIEU		= 1,
		OUVERT		= 2,
		NBR
	};

	enum class PositionBras {
		FERME 		= 0,
		DISTRIB		= 1,
		OUVERT		= 2,
		NBR
	};

	/////////////////// MOTEURS ///////////////////
	// Position des moteurs
	enum class PositionAscenseur {
		MAX_BAS		= 0,
		ROULER 		= 1,
		ETAGE1		= 2,
		ETAGE2		= 3,
		ETAGE3		= 4,
		MAX_HAUT	= 5,
		NBR
	};

	////////////////// UTILITAIRES ///////////////////
	enum class Ascenseur {
		DROIT,
		GAUCHE
	};
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
		static constexpr angleRad positionPince[enumToInt(PositionPince::NBR)] = {950_mrad, 800_mrad, 1220_mrad, 0_mrad};
		static constexpr angleRad positionCoude[enumToInt(PositionCoude::NBR)] = {1570_mrad, 2400_mrad, 1350_mrad};
		static constexpr angleRad positionArriere[enumToInt(PositionArriere::NBR)] = {1300_mrad, 1790_mrad, 0_mrad};
		static constexpr angleRad positionBras[enumToInt(PositionBras::NBR)] = {575_mrad, 2490_mrad, 0_mrad};

		// Déclaration des positions
		static constexpr angleRad positionAscenseur[enumToInt(PositionAscenseur::NBR)] = {790_mrad, 1430_mrad, 2310_mrad, 0_mrad, 0_mrad, 0_mrad};

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
	CarteInfo<IDCartesToumai2015::ASSERVISSEMENT_ASCENSEUR_D> {
		typedef CarteAsservissement2009 type;
	};
)
EXPLICIT_INSTANCIATION(ConstantesToumai2015Type, ConstantesToumai2015,
	CarteInfo<IDCartesToumai2015::ASSERVISSEMENT_ASCENSEUR_G> {
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
	CarteInfo<IDCartesToumai2015::SERVOS_ASCENSEUR> {
		typedef CarteServosNova2009 type;
		typedef ConstantesToumai2015::ServoAscenseur Servo;
	};
)
EXPLICIT_INSTANCIATION(ConstantesToumai2015Type, ConstantesToumai2015,
	CarteInfo<IDCartesToumai2015::SERVOS_AUTRES> {
		typedef CarteServosNova2009 type;
		typedef ConstantesToumai2015::Servo Servo;
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
