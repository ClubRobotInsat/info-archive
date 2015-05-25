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

#include "../../../../CodeCommun/Informations_cartesElec.h"

#include <type_traits>
#include "../../Commun.h"
#include "../ConstantesCommunes2015.h"

namespace IDCartesPrincipal2015 {
	enum {
		ID_CARTE_CAN_USB = 0,
		ID_CARTE_DEPLACEMENT = ID_CARTE_KUBRICK_DEPLACEMENT,
		ID_CARTE_ASSERVISSEMENT_ASCENSEUR_D = ID_CARTE_KUBRICK_ASSERVISSEMENT_BRAS_DROIT,
		ID_CARTE_ASSERVISSEMENT_ASCENSEUR_G = ID_CARTE_KUBRICK_ASSERVISSEMENT_BRAS_GAUCHE,
		ID_CARTE_SERVOS_ASCENSEUR = ID_CARTE_KUBRICK_SERVO_PINCES,
		ID_CARTE_SERVOS_AUTRES = ID_CARTE_KUBRICK_SERVO_PANIERS,
		ID_CARTE_IO = ID_CARTE_KUBRICK_CAPTEURS,
		ID_CARTE_EVITEMENT = ID_CARTE_KUBRICK_EVITEMENT,
		ID_CARTE_DEBUG_DESSIN = 9
	};

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
		PIED_G 			= 6,
		PIED_D			= 7,
		CHOIX_EQUIPE	= 1, // TODO: à debug
		TIRETTE			= 2, // TODO: à debug

		// TODO: TBD
		LAMPE_G			= 10,
		LAMPE_D			= 11,
		PINCE_D			= 14,
		PINCE_G			= 15
	};

	/////////////////// SERVOS ///////////////////
	// Indice des servos
	enum class ServoAscenseur {
		PINCE_D 	= ID_SERVO_KUBRICK_PINCE_D,
		PINCE_G 	= ID_SERVO_KUBRICK_PINCE_G,
		COUDE_D 	= ID_SERVO_KUBRICK_COUDE_D,
		COUDE_G	 	= ID_SERVO_KUBRICK_COUDE_G,
		NBR = 4
	};

	enum class Servo {
		ARRIERE		= ID_SERVO_KUBRICK_PANIER,
		BRAS_D	 	= ID_SERVO_KUBRICK_VOLET_D,
		BRAS_G		= ID_SERVO_KUBRICK_VOLET_G,
		NBR = 3
	};

	// Position des servos
	enum class PositionPince {
		FERME		= 0,
		GOBELET		= 1,
		LAMPE		= 2,
		PIED		= 3,
		GROS_PIED   = 4,
		OUVERT		= 5,
		NBR
	};

	enum class PositionCoude {
		STOCKAGE 	= 0,
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
		BAS 		= 4,
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

        using Contacteur = ConstantesPrincipal2015::Contacteur;
        using Servo = ConstantesPrincipal2015::Servo;

        // On reprend par défaut les mêmes valeur que la partie commune aux 2 robots, à changer si nécessaire
		static constexpr Duree TIMEOUT_DEPLACEMENT_DEFAUT = Constantes2015::TIMEOUT_DEPLACEMENT_DEFAUT;
		static constexpr Duree TIMEOUT_BLOCAGE_ADVERSAIRE_DEFAUT = Constantes2015::TIMEOUT_BLOCAGE_ADVERSAIRE_DEFAUT;
		static constexpr VitesseLineaire VITESSE_LINEAIRE_DEFAUT = Constantes2015::VITESSE_LINEAIRE_DEFAUT;
		static constexpr VitesseAngulaire VITESSE_ANGULAIRE_DEFAUT = 3.75_rad_s;
		static constexpr Distance PRECISION_LINEAIRE_DEFAUT = Constantes2015::PRECISION_LINEAIRE_DEFAUT;
		static constexpr Angle PRECISION_ANGULAIRE_DEFAUT = Constantes2015::PRECISION_ANGULAIRE_DEFAUT;

		static constexpr Distance RAYON_ROTATION_ROBOT = 230.0_mm;
		static constexpr Vector3m TAILLE_ROBOT = {30_cm, 30_cm, 40_cm};
		static constexpr uint16_t TCPIP_PORT_SIMU_DEFAUT = 4321;
		// Cet offset sert à aligner le 0 de la balise avec le 0 de l'info
		// NB : 0 devant, PI/-PI derrière, PI/2 à gauche et -PI/2 à droite
		static constexpr Angle OFFSET_ANGLE_ADV = -0.5_PI;

		static constexpr int IDRobot = 0xDEAD;


		/////////////////// CARTES ///////////////////

		template<IDCartes ID_CARTE>
		struct CarteInfo {};

		template<IDCartesServo ID_CARTE>
		struct CarteServoInfo {};
	};
)

namespace ConstantesPrincipal2015 {
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
	extern Angle const positionPince[enumToInt(PositionPince::NBR)];
	extern Angle const positionCoude[enumToInt(PositionCoude::NBR)];
	extern Angle const positionArriere[enumToInt(PositionArriere::NBR)];
	extern Angle const positionBras[enumToInt(PositionBras::NBR)];
	extern Distance const ENTRAXE_PINCES;
	extern Distance const DISTANCE_CENTRE_PINCES;

	// Déclaration des positions
	extern Angle const positionAscenseur[enumToInt(PositionAscenseur::NBR)];
}

EXPLICIT_INSTANCIATION(ConstantesPrincipal2015Type, ConstantesPrincipal2015,
	CarteInfo<IDCartesPrincipal2015::CAN_USB> : public std::integral_constant<uint32_t, IDCartesPrincipal2015::ID_CARTE_CAN_USB> {
		typedef CarteCAN_USB type;
	};
)
EXPLICIT_INSTANCIATION(ConstantesPrincipal2015Type, ConstantesPrincipal2015,
	CarteInfo<IDCartesPrincipal2015::DEPLACEMENT> : public std::integral_constant<uint32_t, IDCartesPrincipal2015::ID_CARTE_DEPLACEMENT> {
#ifdef TARGET_SIMULATEUR
		typedef CarteDeplacement2009 type;
#else
		typedef CarteDeplacement2015 type;
#endif
	};
)
EXPLICIT_INSTANCIATION(ConstantesPrincipal2015Type, ConstantesPrincipal2015,
	CarteInfo<IDCartesPrincipal2015::ASSERVISSEMENT_ASCENSEUR_D> : public std::integral_constant<uint32_t, IDCartesPrincipal2015::ID_CARTE_ASSERVISSEMENT_ASCENSEUR_D> {
		typedef CarteAsservissement2009 type;
	};
)
EXPLICIT_INSTANCIATION(ConstantesPrincipal2015Type, ConstantesPrincipal2015,
	CarteInfo<IDCartesPrincipal2015::ASSERVISSEMENT_ASCENSEUR_G> : public std::integral_constant<uint32_t, IDCartesPrincipal2015::ID_CARTE_ASSERVISSEMENT_ASCENSEUR_G> {
		typedef CarteAsservissement2009 type;
	};
)
EXPLICIT_INSTANCIATION(ConstantesPrincipal2015Type, ConstantesPrincipal2015,
	CarteInfo<IDCartesPrincipal2015::SERVOS_ASCENSEUR> : public std::integral_constant<uint32_t, IDCartesPrincipal2015::ID_CARTE_SERVOS_ASCENSEUR> {
		typedef CarteServosNova2009 type;
	};
)
EXPLICIT_INSTANCIATION(ConstantesPrincipal2015Type, ConstantesPrincipal2015,
	CarteInfo<IDCartesPrincipal2015::SERVOS_AUTRES> : public std::integral_constant<uint32_t, IDCartesPrincipal2015::ID_CARTE_SERVOS_AUTRES> {
		typedef CarteServosNova2009 type;
	};
)
EXPLICIT_INSTANCIATION(ConstantesPrincipal2015Type, ConstantesPrincipal2015,
	CarteInfo<IDCartesPrincipal2015::IO> : public std::integral_constant<uint32_t, IDCartesPrincipal2015::ID_CARTE_IO> {
		typedef CarteIO2014 type;
	};
)
EXPLICIT_INSTANCIATION(ConstantesPrincipal2015Type, ConstantesPrincipal2015,
	CarteInfo<IDCartesPrincipal2015::EVITEMENT> : public std::integral_constant<uint32_t, IDCartesPrincipal2015::ID_CARTE_EVITEMENT> {
		typedef CarteDetectAdv2009 type;
	};
)
EXPLICIT_INSTANCIATION(ConstantesPrincipal2015Type, ConstantesPrincipal2015,
	CarteInfo<IDCartesPrincipal2015::DEBUG_DESSIN> : public std::integral_constant<uint32_t, IDCartesPrincipal2015::ID_CARTE_DEBUG_DESSIN> {
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
