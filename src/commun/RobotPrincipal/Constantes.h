// Constantes.h

#ifndef CONSTANTES_ROBOT_PRINCIPAL__H
#define CONSTANTES_ROBOT_PRINCIPAL__H


#ifdef TARGET_SIMULATEUR

#include "../../simulateur/communication/cartes/CarteAsservissement2009.h"
#include "../../simulateur/communication/cartes/CarteCAN_USB.h"
//#include "../../simulateur/communication/cartes/CarteContacteurs2007.h"
#include "../../simulateur/communication/cartes/CarteDebugDessin.h"
#include "../../simulateur/communication/cartes/CarteDeplacement2009.h"
#include "../../simulateur/communication/cartes/CarteEvitement2018.h"
#include "../../simulateur/communication/cartes/CarteMoteurs2018.h"
//#include "../../simulateur/cartes/CarteElectrovannes2016.h"
#include "../../simulateur/communication/cartes/Carte.h"
#include "../../simulateur/communication/cartes/CarteIO2018.h"
#include "../../simulateur/communication/cartes/CarteServosNova2017.h"

#else

#include "../../robot/Cartes/Carte.h"
#include "../../robot/Cartes/CarteAsservissement2009.h"
#include "../../robot/Cartes/CarteCAN_USB.h"
//#include "../../robot/Cartes/CarteContacteurs2007.h"
//#include "../../robot/Cartes/CarteDebugDessin.h"
#include "../../robot/Cartes/CarteDeplacement2009.h"
//#include "../../robot/Cartes/CarteElectrovannes2016.h"
#include "../../robot/Cartes/CarteIO2018.h"
#include "../../robot/Cartes/CarteMoteurs2018.h"
//#include "../../robot/Cartes/CartePneumatique.h"
//#include "../../robot/Cartes/CartePneumatique2014.h"
//#include "../../robot/Cartes/CarteColorimetre2014.h"
#include "../../robot/Cartes/CarteEvitement2018.h"
#include "../../robot/Cartes/CarteServosNova2017.h"

#endif

#include "../../elec/code-elec-nucleo/src/IDs_2018.h"

#include "Commun.h"
#include "ConstantesCommunes.h"
#include <type_traits>

namespace IDCartesPrincipal {
	enum {
		ID_CARTE_DEPLACEMENT = ID_ELEC_CARD_MOVEMENT,
		ID_CARTE_EVITEMENT = ID_ELEC_CARD_AVOIDANCE,
		ID_CARTE_IO = ID_ELEC_CARD_IO,
		ID_CARTE_SERVOS = ID_ELEC_CARD_SERVOS,
		ID_CARTE_MOTEURS = ID_ELEC_CARD_MOTORS,

		/*ID_CARTE_DEBUG_DESSIN = 8*/
	};

	ENUM_NS(IDCartesPrincipal, IDCartes, DEPLACEMENT, EVITEMENT, IO, SERVOS, MOTEURS)

	ENUM_CLASS_NS(IDCartesPrincipal, IDCarteServos, PORTE_CUBE, PORTE_SOUTE_G, PORTE_SOUTE_D, ABEILLE_G, ABEILLE_D)
	ENUM_CLASS_NS(IDCartesPrincipal, IDCarteMoteurs, ASCENSEUR_GAUCHE, ASCENSEUR_DROIT, AVALEUR_GAUCHE, AVALEUR_DROIT, BRUSHLESS_GAUCHE, BRUSHLESS_DROIT)
}

namespace ConstantesPrincipal {
	using IDCartes = IDCartesPrincipal::IDCartes;
	using IDCartesServos = IDCartesPrincipal::IDCarteServos;
	using IDCartesMoteurs = IDCartesPrincipal::IDCarteServos;

	const repere::Repere REFERENCE_GREEN =
	    repere::Repere({0_m, 0_m}, repere::Multiplicateur::SENS_POSITIF, repere::Multiplicateur::SENS_POSITIF);
	const repere::Repere REFERENCE_ORANGE =
	    repere::Repere({3_m, 0_m}, repere::Multiplicateur::SENS_NEGATIF, repere::Multiplicateur::SENS_POSITIF);

	const Vector3m START_ROBOT_POSITION(25_cm, 1.50_m, 15_cm);
	const Angle START_ROBOT_ANGLE(-90_deg);

	// POSITION_TOURELLE = { décalage avant, décalage vers la droite }
	const Vector2m POSITION_TOURELLE(10_cm, 5_cm);

	/////////////////// CONTACTEURS ///////////////////

	// Numéros des contacteurs et les rôles associés
	enum class Contacteur {
		TIRETTE = ID_CONTACTOR_TIRETTE,

		NBR = 1
	};

	enum class PositionAscenseur { NBR = 0 };

	/////////////////// SERVOS ///////////////////

	enum class Servo {
		PORTE_CUBE = ID_SERVO_CUBE_DOOR,
		PORTE_SOUTE_GAUCHE = ID_SERVO_HOLD_DOOR_LEFT,
		PORTE_SOUTE_DROIT = ID_SERVO_HOLD_DOOR_RIGHT,
		ABEILLE = ID_SERVO_BEE,
		LOQUET = ID_SERVO_LOQUET,

		NBR = 5
	};

	// Différentes positions des servos
	enum class PorteCube { OUVERT = 0, FERME = 1, NBR = 2 };

	enum class SouteGauche { OUVERT = 0, FERME = 1, NBR = 2 };

	enum class SouteDroit { OUVERT = 0, FERME = 1, NBR = 2 };

	enum class Abeille { OUVERT = 0, FERME = 1, NBR = 2 };

	enum class Loquet { HAUT = 0, BAS = 1, NBR = 2 };

	extern Angle const positionPorteCube[enumToInt(PorteCube::NBR)];
	extern Angle const positionSouteGauche[enumToInt(SouteGauche::NBR)];
	extern Angle const positionSouteDroit[enumToInt(SouteDroit::NBR)];
	extern Angle const positionAbeille[enumToInt(Abeille::NBR)];
	extern Angle const positionLoquet[enumToInt(Loquet::NBR)];

	/////////////////// MOTEURS ///////////////////

	enum class Moteurs {
		ASCENSEUR_GAUCHE = ID_MOTOR_ELEVATOR_LEFT,
		ASCENSEUR_DROIT = ID_MOTOR_ELEVATOR_RIGHT,
		AVALEUR_GAUCHE = ID_MOTOR_SWALLOW_LEFT,
		AVALEUR_DROIT = ID_MOTOR_SWALLOW_RIGHT,
		BRUSHLESS_GAUCHE = ID_MOTOR_BRUSHLESS_LEFT,
		BRUSHLESS_DROIT = ID_MOTOR_BRUSHLESS_RIGHT,

		NBR = 6
	};

	// Durée attendue lors de l'envoi d'un message aux cartes élecs dans les fonctions blonquantes autre que les
	// servos.
	auto const TIMEOUT_ELEC = 100_ms;

	// On reprend par défaut les mêmes valeurs que la partie commune aux 2 robots, à changer si nécessaire
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

	/////////////////// CARTES ///////////////////

	template <IDCartesPrincipal::IDCartes ID_CARTE>
	struct CarteInfo {};

	template <IDCartesPrincipal::IDCarteServos ID_CARTE>
	struct CarteServoInfo {};

	/*template <>
	struct CarteInfo<IDCartesPrincipal::CAN_USB> {
	    using typeCarte = CarteCAN_USB;
	    enum : std::uint8_t { idCarte = IDCartesPrincipal::ID_CARTE_CAN_USB };
	};*/

	template <>
	struct CarteInfo<IDCartesPrincipal::DEPLACEMENT> {
		using typeCarte = CarteDeplacement2009;
		enum : std::uint8_t { idCarte = IDCartesPrincipal::ID_CARTE_DEPLACEMENT };
	};

	template <>
	struct CarteInfo<IDCartesPrincipal::EVITEMENT> {
		using typeCarte = CarteEvitement2018;
		enum : std::uint8_t { idCarte = IDCartesPrincipal::ID_CARTE_EVITEMENT };
	};

	template <>
	struct CarteInfo<IDCartesPrincipal::IO> {
		using typeCarte = CarteIO2018;
		enum : std::uint8_t { idCarte = IDCartesPrincipal::ID_CARTE_IO };
	};

	template <>
	struct CarteInfo<IDCartesPrincipal::SERVOS> {
		using typeCarte = CarteServosNova2017;
		enum : std::uint8_t { idCarte = IDCartesPrincipal::ID_CARTE_SERVOS };
	};

	template <>
	struct CarteInfo<IDCartesPrincipal::MOTEURS> {
		using typeCarte = CarteMoteurs2018;
		enum : std::uint8_t { idCarte = IDCartesPrincipal::ID_CARTE_MOTEURS };
	};

	//----- pas utilisé cette année -----//
	/*template <>
	struct CarteInfo<IDCartesPrincipal::ASCENSEUR> {
	    using typeCarte = CarteAsservissement2009;
	    enum : std::uint8_t { idCarte = IDCartesPrincipal::ID_CARTE_ASCENSEUR };
	};*/
}

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
