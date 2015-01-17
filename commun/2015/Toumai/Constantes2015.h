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

#include <type_traits>
#include "../../Commun.h"
#include "../ConstantesCommunes2015.h"

namespace IDCartesToumai2015 {
	ENUM_NS(IDCartesToumai2015, IDCartes,
		CAN_USB,
		DEPLACEMENT,
		ASSERVISSEMENT_ASCENSEUR_D,
		ASSERVISSEMENT_ASCENSEUR_G,
		SERVOS_ASCENSEUR,
		SERVOS_AUTRES,
		IO,
		EVITEMENT,
		PNEUMATIQUE,
		DEBUG_DESSIN
		//ID_CARTE_DEBUG = 11
	);

	ENUM_CLASS_NS(IDCartesToumai2015, IDCartesServo,
		ASCENSEUR,
		AUTRES
	);
}

STRUCT_NAMESPACE(IDCartesToumai2015Type, IDCartesToumai2015,
	{
		using IDCartes = IDCartesToumai2015::IDCartes;
		using IDCartesServo = IDCartesToumai2015::IDCartesServo;
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
		using ConstantesCommunes = Constantes2015Type;
		using IDCartes = IDCartesToumai2015Type::IDCartes;
		using IDCartesServo = IDCartesToumai2015Type::IDCartesServo;

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

		// On reprend par défaut les mêmes valeur que la partie commune aux 2 robots, à changer si nécessaire
		static constexpr dureeS TIMEOUT_DEPLACEMENT_DEFAUT = Constantes2015::TIMEOUT_DEPLACEMENT_DEFAUT;
		static constexpr vitesseM_s VITESSE_LINEAIRE_DEFAUT = Constantes2015::VITESSE_LINEAIRE_DEFAUT;
		static constexpr vitesseRad_s VITESSE_ANGULAIRE_DEFAUT = Constantes2015::VITESSE_ANGULAIRE_DEFAUT;
		static constexpr distanceM PRECISION_LINEAIRE_DEFAUT = Constantes2015::PRECISION_LINEAIRE_DEFAUT;
		static constexpr angleRad PRECISION_ANGULAIRE_DEFAUT = Constantes2015::PRECISION_ANGULAIRE_DEFAUT;

		static constexpr distanceM RAYON_ROTATION_ROBOT = 230.0_mm;
		static constexpr Vector3m TAILLE_ROBOT = {20_cm, 30_cm, 20_cm};

		/////////////////// CARTES ///////////////////

		template<IDCartes ID_CARTE>
		struct CarteInfo {};

		template<IDCartesServo ID_CARTE>
		struct CarteServoInfo {};
	};
)

EXPLICIT_INSTANCIATION(ConstantesToumai2015Type, ConstantesToumai2015,
	CarteInfo<IDCartesToumai2015::CAN_USB> : public std::integral_constant<uint32_t, 0> {
		typedef CarteCAN_USB type;
	};
)
EXPLICIT_INSTANCIATION(ConstantesToumai2015Type, ConstantesToumai2015,
	CarteInfo<IDCartesToumai2015::DEPLACEMENT> : public std::integral_constant<uint32_t, 1> {
		typedef CarteDeplacement2009 type;
	};
)
EXPLICIT_INSTANCIATION(ConstantesToumai2015Type, ConstantesToumai2015,
	CarteInfo<IDCartesToumai2015::ASSERVISSEMENT_ASCENSEUR_D> : public std::integral_constant<uint32_t, 2> {
		typedef CarteAsservissement2009 type;
	};
)
EXPLICIT_INSTANCIATION(ConstantesToumai2015Type, ConstantesToumai2015,
	CarteInfo<IDCartesToumai2015::ASSERVISSEMENT_ASCENSEUR_G> : public std::integral_constant<uint32_t, 3> {
		typedef CarteAsservissement2009 type;
	};
)
EXPLICIT_INSTANCIATION(ConstantesToumai2015Type, ConstantesToumai2015,
	CarteInfo<IDCartesToumai2015::SERVOS_ASCENSEUR> : public std::integral_constant<uint32_t, 4> {
		typedef CarteServosNova2009 type;
	};
)
EXPLICIT_INSTANCIATION(ConstantesToumai2015Type, ConstantesToumai2015,
	CarteInfo<IDCartesToumai2015::SERVOS_AUTRES> : public std::integral_constant<uint32_t, 5> {
		typedef CarteServosNova2009 type;
	};
)
EXPLICIT_INSTANCIATION(ConstantesToumai2015Type, ConstantesToumai2015,
	CarteInfo<IDCartesToumai2015::IO> : public std::integral_constant<uint32_t, 6> {
		typedef CarteIO2014 type;
	};
)
EXPLICIT_INSTANCIATION(ConstantesToumai2015Type, ConstantesToumai2015,
	CarteInfo<IDCartesToumai2015::EVITEMENT> : public std::integral_constant<uint32_t, 8> {
		typedef CarteDetectAdv2009 type;
	};
)
EXPLICIT_INSTANCIATION(ConstantesToumai2015Type, ConstantesToumai2015,
	CarteInfo<IDCartesToumai2015::PNEUMATIQUE> : public std::integral_constant<uint32_t, 9> {
		typedef CartePneumatique2014 type;
	};
)
EXPLICIT_INSTANCIATION(ConstantesToumai2015Type, ConstantesToumai2015,
	CarteInfo<IDCartesToumai2015::DEBUG_DESSIN> : public std::integral_constant<uint32_t, 10> {
		typedef CarteDebugDessin type;
	};
)

EXPLICIT_INSTANCIATION(ConstantesToumai2015Type, ConstantesToumai2015,
	CarteServoInfo<IDCartesToumai2015::IDCartesServo::ASCENSEUR> : public ConstantesToumai2015Type::CarteInfo<IDCartesToumai2015::SERVOS_ASCENSEUR> {
		static auto const IDCarte = IDCartesToumai2015::SERVOS_ASCENSEUR;
		typedef ConstantesToumai2015::ServoAscenseur Servo;
	};
)
EXPLICIT_INSTANCIATION(ConstantesToumai2015Type, ConstantesToumai2015,
	CarteServoInfo<IDCartesToumai2015::IDCartesServo::AUTRES> : public CarteInfo<IDCartesToumai2015::SERVOS_AUTRES> {
		static auto const IDCarte = IDCartesToumai2015::SERVOS_AUTRES;
		typedef ConstantesToumai2015::Servo Servo;
	};
)


#endif
