// Constantes2015.h

#ifndef CONSTANTES_ROBOT_PRINCIPAL_2015_H
#define CONSTANTES_ROBOT_PRINCIPAL_2015_H


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
#include "../../../robot/2015/Commun/CarteDeplacement2015.h"
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

namespace IDCartesPrincipal2015 {
	ENUM_NS(IDCartesPrincipal2015, IDCartes,
		CAN_USB,
		DEPLACEMENT,
		ASSERVISSEMENT_ASCENSEUR_D,
		ASSERVISSEMENT_ASCENSEUR_G,
		SERVOS_ASCENSEUR,
		SERVOS_AUTRES,
		IO,
		EVITEMENT,
		/* PNEUMATIQUE */
		DEBUG_DESSIN
		//ID_CARTE_DEBUG = 11
	);

	ENUM_CLASS_NS(IDCartesPrincipal2015, IDCartesServo,
		ASCENSEUR,
		AUTRES
	);
}

STRUCT_NAMESPACE(IDCartesPrincipal2015Type, IDCartesPrincipal2015,
	{
		using IDCartes = IDCartesPrincipal2015::IDCartes;
		using IDCartesServo = IDCartesPrincipal2015::IDCartesServo;
	}
)

namespace ConstantesPrincipal2015 {
	/////////////////// CONTACTEURS ///////////////////
	// Numéros des contacteurs et les rôles associés
	enum class Contacteur {
		CHOIX_EQUIPE	= 2,
		TIRETTE			= 6,
		// TODO: TBD
		LAMPE_G			= 10,
		LAMPE_D			= 11,
		PIED_G 			= 12,
		PIED_D			= 13,
		PINCE_D			= 14,
		PINCE_G			= 15
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
		GOBELET		= 0,
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

	enum class Bras {
		GAUCHE 		= 0,
		DROIT		= 1,
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
	enum class CoteAscenseur {
		DROIT,
		GAUCHE,
	};
}

STRUCT_NAMESPACE(ConstantesPrincipal2015Type, ConstantesPrincipal2015,
	{
		using ConstantesCommunes = Constantes2015Type;
		using IDCartes = IDCartesPrincipal2015Type::IDCartes;
		using IDCartesServo = IDCartesPrincipal2015Type::IDCartesServo;

		using Contacteur 		= ConstantesPrincipal2015::Contacteur;
		using ServoAscenseur 	= ConstantesPrincipal2015::ServoAscenseur;
		using Servo 			= ConstantesPrincipal2015::Servo;
		using PositionPince 	= ConstantesPrincipal2015::PositionPince;
		using PositionCoude 	= ConstantesPrincipal2015::PositionCoude;
		using PositionArriere 	= ConstantesPrincipal2015::PositionArriere;
		using PositionBras 		= ConstantesPrincipal2015::PositionBras;
		using PositionAscenseur = ConstantesPrincipal2015::PositionAscenseur;
		using CoteAscenseur 	= ConstantesPrincipal2015::CoteAscenseur;

		// Déclaration des positions
		static constexpr Angle positionPince[enumToInt(PositionPince::NBR)] = {950_mrad, 800_mrad, 1220_mrad, 0_mrad};
		static constexpr Angle positionCoude[enumToInt(PositionCoude::NBR)] = {1570_mrad, 2400_mrad, 1350_mrad};
		static constexpr Angle positionArriere[enumToInt(PositionArriere::NBR)] = {1300_mrad, 1790_mrad, 0_mrad};
		static constexpr Angle positionBras[enumToInt(PositionBras::NBR)] = {575_mrad, 2490_mrad, 0_mrad};

		// Déclaration des positions
		static constexpr Angle positionAscenseur[enumToInt(PositionAscenseur::NBR)] = {790_mrad, 1430_mrad, 2310_mrad, 0_mrad, 0_mrad, 0_mrad};

		// On reprend par défaut les mêmes valeur que la partie commune aux 2 robots, à changer si nécessaire
		static constexpr Duree TIMEOUT_DEPLACEMENT_DEFAUT = Constantes2015::TIMEOUT_DEPLACEMENT_DEFAUT;
		static constexpr VitesseLineaire VITESSE_LINEAIRE_DEFAUT = Constantes2015::VITESSE_LINEAIRE_DEFAUT;
		static constexpr VitesseAngulaire VITESSE_ANGULAIRE_DEFAUT = Constantes2015::VITESSE_ANGULAIRE_DEFAUT;
		static constexpr Distance PRECISION_LINEAIRE_DEFAUT = Constantes2015::PRECISION_LINEAIRE_DEFAUT;
		static constexpr Angle PRECISION_ANGULAIRE_DEFAUT = Constantes2015::PRECISION_ANGULAIRE_DEFAUT;

		static constexpr Distance RAYON_ROTATION_ROBOT = 230.0_mm;
		static constexpr Vector3m TAILLE_ROBOT = {20_cm, 30_cm, 20_cm};

		/////////////////// CARTES ///////////////////

		template<IDCartes ID_CARTE>
		struct CarteInfo {};

		template<IDCartesServo ID_CARTE>
		struct CarteServoInfo {};
	};
)

EXPLICIT_INSTANCIATION(ConstantesPrincipal2015Type, ConstantesPrincipal2015,
	CarteInfo<IDCartesPrincipal2015::CAN_USB> : public std::integral_constant<uint32_t, 0> {
		typedef CarteCAN_USB type;
	};
)
EXPLICIT_INSTANCIATION(ConstantesPrincipal2015Type, ConstantesPrincipal2015,
	CarteInfo<IDCartesPrincipal2015::DEPLACEMENT> : public std::integral_constant<uint32_t, 1> {
#ifdef TARGET_SIMULATEUR
		typedef CarteDeplacement2009 type;
#else
		typedef CarteDeplacement2015 type;
#endif
	};
)
EXPLICIT_INSTANCIATION(ConstantesPrincipal2015Type, ConstantesPrincipal2015,
	CarteInfo<IDCartesPrincipal2015::ASSERVISSEMENT_ASCENSEUR_D> : public std::integral_constant<uint32_t, 2> {
		typedef CarteAsservissement2009 type;
	};
)
EXPLICIT_INSTANCIATION(ConstantesPrincipal2015Type, ConstantesPrincipal2015,
	CarteInfo<IDCartesPrincipal2015::ASSERVISSEMENT_ASCENSEUR_G> : public std::integral_constant<uint32_t, 3> {
		typedef CarteAsservissement2009 type;
	};
)
EXPLICIT_INSTANCIATION(ConstantesPrincipal2015Type, ConstantesPrincipal2015,
	CarteInfo<IDCartesPrincipal2015::SERVOS_ASCENSEUR> : public std::integral_constant<uint32_t, 4> {
		typedef CarteServosNova2009 type;
	};
)
EXPLICIT_INSTANCIATION(ConstantesPrincipal2015Type, ConstantesPrincipal2015,
	CarteInfo<IDCartesPrincipal2015::SERVOS_AUTRES> : public std::integral_constant<uint32_t, 5> {
		typedef CarteServosNova2009 type;
	};
)
EXPLICIT_INSTANCIATION(ConstantesPrincipal2015Type, ConstantesPrincipal2015,
	CarteInfo<IDCartesPrincipal2015::IO> : public std::integral_constant<uint32_t, 6> {
		typedef CarteIO2014 type;
	};
)
EXPLICIT_INSTANCIATION(ConstantesPrincipal2015Type, ConstantesPrincipal2015,
	CarteInfo<IDCartesPrincipal2015::EVITEMENT> : public std::integral_constant<uint32_t, 8> {
		typedef CarteDetectAdv2009 type;
	};
)
/*
EXPLICIT_INSTANCIATION(ConstantesPrincipal2015Type, ConstantesPrincipal2015,
	CarteInfo<IDCartesPrincipal2015::PNEUMATIQUE> : public std::integral_constant<uint32_t, 9> {
		typedef CartePneumatique2014 type;
	};
)
*/
EXPLICIT_INSTANCIATION(ConstantesPrincipal2015Type, ConstantesPrincipal2015,
	CarteInfo<IDCartesPrincipal2015::DEBUG_DESSIN> : public std::integral_constant<uint32_t, 10> {
		typedef CarteDebugDessin type;
	};
)

EXPLICIT_INSTANCIATION(ConstantesPrincipal2015Type, ConstantesPrincipal2015,
	CarteServoInfo<IDCartesPrincipal2015::IDCartesServo::ASCENSEUR> : public ConstantesPrincipal2015Type::CarteInfo<IDCartesPrincipal2015::SERVOS_ASCENSEUR> {
		static auto const IDCarte = IDCartesPrincipal2015::SERVOS_ASCENSEUR;
		typedef ConstantesPrincipal2015::ServoAscenseur Servo;
	};
)
EXPLICIT_INSTANCIATION(ConstantesPrincipal2015Type, ConstantesPrincipal2015,
	CarteServoInfo<IDCartesPrincipal2015::IDCartesServo::AUTRES> : public CarteInfo<IDCartesPrincipal2015::SERVOS_AUTRES> {
		static auto const IDCarte = IDCartesPrincipal2015::SERVOS_AUTRES;
		typedef ConstantesPrincipal2015::Servo Servo;
	};
)

#endif
