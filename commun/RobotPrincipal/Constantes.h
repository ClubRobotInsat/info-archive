// Constantes.h

#ifndef CONSTANTES_ROBOT_PRINCIPAL__H
#define CONSTANTES_ROBOT_PRINCIPAL__H


#ifdef TARGET_SIMULATEUR

#include "../../simulateur/cartes/CarteAsservissement2009.h"
#include "../../simulateur/cartes/CarteCAN_USB.h"
#include "../../simulateur/cartes/CarteContacteurs2007.h"
#include "../../simulateur/cartes/CarteDebugDessin.h"
#include "../../simulateur/cartes/CarteDeplacement2009.h"
#include "../../simulateur/cartes/CarteDetectAdv2009.h"
//#include "../../simulateur/cartes/CarteElectrovannes2016.h"
#include "../../simulateur/cartes/CarteIO2014.h"
#include "../../simulateur/cartes/CarteServosNova2009.h"
#include "../../simulateur/robot/Carte.h"

#else

#include "../../robot/Cartes/Carte.h"
#include "../../robot/Cartes/CarteAsservissement2009.h"
#include "../../robot/Cartes/CarteCAN_USB.h"
#include "../../robot/Cartes/CarteContacteurs2007.h"
//#include "../../robot/Cartes/CarteDebugDessin.h"
#include "../../robot/Cartes/CarteDeplacement2009.h"
//#include "../../robot/Cartes/CarteElectrovannes2016.h"
#include "../../robot/Cartes/CarteIO2014.h"
//#include "../../robot/Cartes/CartePneumatique.h"
//#include "../../robot/Cartes/CartePneumatique2014.h"
//#include "../../robot/Cartes/CarteColorimetre2014.h"
#include "../../robot/Cartes/CarteServosNova2009.h"
#include "../robot/Cartes/CarteDetectAdv2009.h"

#endif

#include "../../../CodeCommun/Informations_cartesElec_2017.h"

#include "../Commun.h"
#include "../ConstantesCommunes.h"
#include <type_traits>

namespace IDCartesPrincipal {
	enum {
		ID_CARTE_CAN_USB = 0,
		ID_CARTE_DEPLACEMENT = ID_CARTE_INYANGA_DEPLACEMENT,
		ID_CARTE_SERVOS = ID_CARTE_INYANGA_SERVOS,
		ID_CARTE_IO = ID_CARTE_INYANGA_IO,
		ID_CARTE_EVITEMENT = ID_CARTE_INYANGA_EVITEMENT,
		ID_CARTE_ASCENSEUR = ID_CARTE_INYANGA_ASCENSEUR,

		/*ID_CARTE_DEBUG_DESSIN = 8*/
	};

	ENUM_NS(IDCartesPrincipal, IDCartes, CAN_USB, DEPLACEMENT, SERVOS, IO, EVITEMENT, ASCENSEUR);


	// TODO : AJOUTER ASCENSEUR ?
	ENUM_CLASS_NS(IDCartesPrincipal, IDCartesServo, PINCE, LACET, TANGAGE, FUSEE, GAUCHE, DROITE);
}

namespace ConstantesPrincipal {
	using IDCartes = IDCartesPrincipal::IDCartes;
	using IDCartesServo = IDCartesPrincipal::IDCartesServo;

	const repere::Repere REFERENCE_BLUE =
	    repere::Repere({0_m, 0_m}, repere::Multiplicateur::SENS_POSITIF, repere::Multiplicateur::SENS_POSITIF);
	const repere::Repere REFERENCE_YELLOW =
	    repere::Repere({3_m, 0_m}, repere::Multiplicateur::SENS_NEGATIF, repere::Multiplicateur::SENS_POSITIF);

	const Vector3m START_ROBOT_POSITION(1.05_m - 29.4_cm / 2, 2_m - 29.4_cm / 2 - 2.1_cm, 15_cm);
	const Angle START_ROBOT_ANGLE(-90_deg);

	/////////////////// CONTACTEURS ///////////////////
	// Numéros des contacteurs et les rôles associés
	enum class Contacteur {
		TIRETTE = 2,
	};

	/*
	enum class Pince {
	    GAUCHE = 0,
	    DROIT = 1,
	};

	enum class PositionPince {
	    RENTRE_DEBUT = 0,
	    RENTRE = 1,
	    SORTIE = 2,
	    SORTIE_MAX = 3,
	    NBR = 4,
	};
	*/


	enum class PositionAscenseur {
		HAUT = 0,
		BAS = 1,
		STOCKAGE = 2,
		CYLINDRE = 3,
		NBR = 4,
	};

	enum class Pince {
		OUVERT = 0,
		FERME = 1,
		NBR = 2,
	};

	enum class Fusee {
		STOCKAGE = 0,
		RELACHEMENT = 1,
		NBR = 2,
	};

	enum class Gauche {
		FERME = 0,
		OUVERT = 1,
		NBR = 2,
	};

	enum class Droite {
		FERME = 0,
		OUVERT = 1,
		NBR = 2,
	};

	enum class Tangage {
		ATTRAPER = 0,
		RANGER = 1,
		NBR = 2,
	};
	/////////////////// SERVOS ///////////////////

	enum class Servo {
		PINCE = ID_SERVO_INYANGA_PINCE,
		LACET = ID_SERVO_INYANGA_LACET,     // mouvement de gauche à droite
		TANGAGE = ID_SERVO_INYANGA_TANGAGE, // mouvement de bas en haut
		FUSEE = ID_SERVO_INYANGA_FUSEE,
		GAUCHE = ID_SERVO_INYANGA_GAUCHE, // servos à l'arrière
		DROITE = ID_SERVO_INYANGE_DROITE,

		NBR = 6
	};


	/*enum class Servo_D { NBR = 2 };
	enum class Servo_G { NBR = 3 };*/

	// Durée attendue lors de l'envoi d'un message aux cartes élecs dans les fonctions blonquantes autre que les
	// servos.
	auto const TIMEOUT_ELEC = 100_ms;

	// On reprend par défaut les mêmes valeur que la partie commune aux 2 robots, à changer si nécessaire
	extern Duration const TIMEOUT_DEPLACEMENT_DEFAUT;
	extern Duration const TIMEOUT_BLOCAGE_ADVERSAIRE_DEFAUT;
	extern Speed const VITESSE_LINEAIRE_DEFAUT;
	extern AngularSpeed const VITESSE_ANGULAIRE_DEFAUT;
	extern Distance const PRECISION_LINEAIRE_DEFAUT;
	extern Angle const PRECISION_ANGULAIRE_DEFAUT;

	extern Distance const RAYON_ROTATION_ROBOT;
	extern Vector3m const TAILLE_ROBOT;
	extern uint16_t const TCPIP_PORT_SIMU_DEFAULT;
	// Cet offset sert à aligner le 0 de la balise avec le 0 de l'info
	// NB : 0 devant, PI/-PI derrière, PI/2 à gauche et -PI/2 à droite
	extern Angle const OFFSET_ANGLE_ADV;

	extern int const IDRobot;


	extern Angle const positionAscenseur[enumToInt(PositionAscenseur::NBR)];
	extern Angle const positionPince[enumToInt(Pince::NBR)];
	extern Angle const positionTangage[enumToInt(Tangage::NBR)];
	extern Angle const positionGauche[enumToInt(Gauche::NBR)];
	extern Angle const positionDroite[enumToInt(Droite::NBR)];
	extern Angle const positionFusee[enumToInt(Fusee::NBR)];

	/////////////////// CARTES ///////////////////

	template <IDCartesPrincipal::IDCartes ID_CARTE>
	struct CarteInfo {};

	template <IDCartesPrincipal::IDCartesServo ID_CARTE>
	struct CarteServoInfo {};

	template <>
	struct CarteInfo<IDCartesPrincipal::CAN_USB> {
		using typeCarte = CarteCAN_USB;
		enum : std::uint8_t { idCarte = IDCartesPrincipal::ID_CARTE_CAN_USB };
	};

	template <>
	struct CarteInfo<IDCartesPrincipal::DEPLACEMENT> {
		using typeCarte = CarteDeplacement2009;
		enum : std::uint8_t { idCarte = IDCartesPrincipal::ID_CARTE_DEPLACEMENT };
	};

	template <>
	struct CarteInfo<IDCartesPrincipal::SERVOS> {
		using typeCarte = CarteServosNova2009;
		enum : std::uint8_t { idCarte = IDCartesPrincipal::ID_CARTE_SERVOS };
	};

	template <>
	struct CarteInfo<IDCartesPrincipal::IO> {
		using typeCarte = CarteIO2014;
		enum : std::uint8_t { idCarte = IDCartesPrincipal::ID_CARTE_IO };
	};

	template <>
	struct CarteInfo<IDCartesPrincipal::EVITEMENT> {
		using typeCarte = CarteDetectAdv2009;
		enum : std::uint8_t { idCarte = IDCartesPrincipal::ID_CARTE_EVITEMENT };
	};

	template <>
	struct CarteInfo<IDCartesPrincipal::ASCENSEUR> {
		using typeCarte = CarteAsservissement2009;
		enum : std::uint8_t { idCarte = IDCartesPrincipal::ID_CARTE_ASCENSEUR };
	};


	// TODO : s'occuper des cartes ascenseur et aspiration
	/*
	    template <>
	    struct CarteInfo<IDCartesPrincipal::ASPIRATION> {
	        using typeCarte = CarteAspiration;
	        enum : std::uint8_t { idCarte = IDCartesPrincipal::ID_CARTE_ASPIRATION };
	    };

	        template <>
	        struct CarteInfo<IDCartesPrincipal::COLORIMETRIE> {
	            using typeCarte = CarteColorimetre2014;
	            enum : std::uint8_t { idCarte = IDCartesPrincipal::ID_CARTE_COLORIMETRIE };
	        };*/
};

struct ConstantesRobotPrincipal : public Commun::ConstantesRobot {
	virtual Duration getTimeoutMoveDefault() const override;
	virtual Duration getTimeoutOpponentBlockingDefault() const override;
	virtual Speed getLinearSpeedDefault() const override;
	virtual AngularSpeed getAngularSpeedDefault() const override;
	virtual Distance getLinearPrecisionDefault() const override;
	virtual Angle getAngularPrecisionDefault() const override;

	virtual Distance getRayRotation() const override;
	virtual Vector3m getRobotSize() const override;
	virtual Angle getOffsetAngleAdv() const override;

	virtual uint16_t getPortTCPIPDefault() const override;

	virtual int getIDRobot() const override;
};

#endif
