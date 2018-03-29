// Constantes.h

#ifndef CONSTANTES_ROBOT_PRINCIPAL__H
#define CONSTANTES_ROBOT_PRINCIPAL__H


#ifdef TARGET_SIMULATEUR

#include "../../simulateur/communication/cartes/CarteAsservissement2009.h"
#include "../../simulateur/communication/cartes/CarteCAN_USB.h"
//#include "../../simulateur/communication/cartes/CarteContacteurs2007.h"
#include "../../simulateur/communication/cartes/CarteDebugDessin.h"
#include "../../simulateur/communication/cartes/CarteDeplacement2009.h"
#include "../../simulateur/communication/cartes/CarteDetectAdv2009.h"
//#include "../../simulateur/cartes/CarteElectrovannes2016.h"
#include "../../simulateur/communication/cartes/Carte.h"
#include "../../simulateur/communication/cartes/CarteIO2014.h"
#include "../../simulateur/communication/cartes/CarteServosNova2017.h"

#else

#include "../../robot/Cartes/Carte.h"
#include "../../robot/Cartes/CarteAsservissement2009.h"
#include "../../robot/Cartes/CarteCAN_USB.h"
//#include "../../robot/Cartes/CarteContacteurs2007.h"
//#include "../../robot/Cartes/CarteDebugDessin.h"
#include "../../robot/Cartes/CarteDeplacement2009.h"
//#include "../../robot/Cartes/CarteElectrovannes2016.h"
#include "../../robot/Cartes/CarteIO2014.h"
//#include "../../robot/Cartes/CartePneumatique.h"
//#include "../../robot/Cartes/CartePneumatique2014.h"
//#include "../../robot/Cartes/CarteColorimetre2014.h"
#include "../../robot/Cartes/CarteDetectAdv2009.h"
#include "../../robot/Cartes/CarteServosNova2017.h"

#endif

#include "../../../../CodeCommun/Informations_cartesElec_2018.h"

#include "Commun.h"
#include "ConstantesCommunes.h"
#include <type_traits>

namespace IDCartesPrincipal {
	enum {
		ID_CARTE_DEPLACEMENT = ID_CARTE_ELEC_DEPLACEMENT,
		ID_CARTE_EVITEMENT = ID_CARTE_ELEC_EVITEMENT,
		ID_CARTE_IO = ID_CARTE_ELEC_IO,
		ID_CARTE_SERVOS = ID_CARTE_ELEC_SERVOS,
		ID_CARTE_MOTEUR_ASCENSEUR_G = ID_CARTE_ELEC_MOTEUR_ASCENSEUR_GAUCHE,
		ID_CARTE_MOTEUR_ASCENSEUR_D = ID_CARTE_ELEC_MOTEUR_ASCENSEUR_DROIT,
		ID_CARTE_MOTEUR_AVALEUR_G = ID_CARTE_ELEC_MOTEUR_AVALEUR_GAUCHE,
		ID_CARTE_MOTEUR_AVALEUR_D = ID_CARTE_ELEC_MOTEUR_ASCENSEUR_DROIT,

		/*ID_CARTE_DEBUG_DESSIN = 8*/
	};

	ENUM_NS(IDCartesPrincipal, IDCartes, DEPLACEMENT, EVITEMENT, IO, SERVOS, MOTEUR_ASCENSEUR_G, MOTEUR_ASCENSEUR_D, MOTEUR_AVALEUR_G, MOTEUR_AVALEUR_D)

	ENUM_CLASS_NS(IDCartesPrincipal, IDCarteServos, PORTE_CUBE, PORTE_SOUTE_G, PORTE_SOUTE_D, ABEILLE_G, ABEILLE_D)
}

namespace ConstantesPrincipal {
	using IDCartes = IDCartesPrincipal::IDCartes;
	using IDCartesServos = IDCartesPrincipal::IDCarteServos;
	using IDCartesMoteurs = IDCartesPrincipal::IDCarteServos;

	const repere::Repere REFERENCE_ORANGE =
	    repere::Repere({0_m, 0_m}, repere::Multiplicateur::SENS_POSITIF, repere::Multiplicateur::SENS_POSITIF);
	const repere::Repere REFERENCE_GREEN =
	    repere::Repere({3_m, 0_m}, repere::Multiplicateur::SENS_NEGATIF, repere::Multiplicateur::SENS_POSITIF);

	const Vector3m START_ROBOT_POSITION(1.05_m - 29.4_cm / 2, 2_m - 29.4_cm / 2 - 2.1_cm, 15_cm);
	const Angle START_ROBOT_ANGLE(-90_deg);

	// POSITION_TOURELLE = { décalage avant, décalage vers la droite }
	const Vector2m POSITION_TOURELLE(10_cm, 5_cm);

	/////////////////// CONTACTEURS ///////////////////

	// Numéros des contacteurs et les rôles associés
	enum class Contacteur {
		TIRETTE = ID_CONTACTEUR_TIRETTE,

		NBR = 1
	};

	enum class PositionAscenseur { NBR = 0 };

	/////////////////// SERVOS ///////////////////

	enum class Servo {
		LOQUET = ID_SERVO_LOQUET,
		PORTE_SOUTE_GAUCHE = ID_SERVO_OUVERTURE_PORTE_SOUTE_GAUCHE,
		PORTE_SOUTE_DROIT = ID_SERVO_OUVERTURE_PORTE_SOUTE_DROIT,
		ABEILLE_GAUCHE = ID_SERVO_ABEILLE_GAUCHE,
		ABEILLE_DROIT = ID_SERVO_ABEILLE_DROIT,

		NBR = 5
	};

	// Différentes positions des servos
	enum class PorteCube { OUVERT = 0, FERME = 1, NBR = 2 };

	enum class SouteGauche { OUVERT = 0, FERME = 1, NBR = 2 };

	enum class SouteDroit { OUVERT = 0, FERME = 1, NBR = 2 };

	enum class AbeilleGauche { OUVERT = 0, FERME = 1, NBR = 2 };

	enum class AbeilleDroit { OUVERT = 0, FERME = 1, NBR = 2 };

	extern Angle const positionPorteCube[enumToInt(PorteCube::NBR)];
	extern Angle const positionSouteGauche[enumToInt(SouteGauche::NBR)];
	extern Angle const positionSouteDroit[enumToInt(SouteDroit::NBR)];
	extern Angle const positionAbeilleGauche[enumToInt(AbeilleGauche::NBR)];
	extern Angle const positionAbeilleDroit[enumToInt(AbeilleDroit::NBR)];

	/////////////////// MOTEURS ///////////////////

	// Différentes positions des moteurs
	enum class AvaleurGauche { POS_0 = 0, POS_1 = 1, NBR = 2 };

	enum class AvaleurDroit { POS_0 = 0, POS_1 = 1, NBR = 2 };

	enum class AscenseurGauche { POS_0 = 0, POS_1 = 1, POS_2 = 2, NBR = 3 };

	enum class AscenseurDroit { POS_0 = 0, POS_1 = 1, POS_2 = 2, NBR = 3 };

	extern Angle const positionAvaleurGauche[enumToInt(AvaleurGauche::NBR)];
	extern Angle const positionAvaleurDroit[enumToInt(AvaleurDroit::NBR)];
	extern Angle const positionAscenseurGauche[enumToInt(AscenseurGauche::NBR)];
	extern Angle const positionAscenseurDroit[enumToInt(AscenseurDroit::NBR)];

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

	// TODO : gérer la carte mère

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
		using typeCarte = CarteDetectAdv2009;
		enum : std::uint8_t { idCarte = IDCartesPrincipal::ID_CARTE_EVITEMENT };
	};

	// TODO : refaire la carte IO, elle ne sert plus qu'à la tirette
	template <>
	struct CarteInfo<IDCartesPrincipal::IO> {
		using typeCarte = CarteIO2014;
		enum : std::uint8_t { idCarte = IDCartesPrincipal::ID_CARTE_IO };
	};

	template <>
	struct CarteInfo<IDCartesPrincipal::SERVOS> {
		using typeCarte = CarteServosNova2017;
		enum : std::uint8_t { idCarte = IDCartesPrincipal::ID_CARTE_SERVOS };
	};

	// FIXME : voir si du côté élec ils ne préfèrent pas 2 interfaces moteur asservi / non asservi pour les moteurs
	template <>
	struct CarteInfo<IDCartesPrincipal::MOTEUR_ASCENSEUR_G> {
		using typeCarte = CarteAsservissement2009;
		enum : std::uint8_t { idCarte = IDCartesPrincipal::ID_CARTE_MOTEUR_ASCENSEUR_G };
	};

	template <>
	struct CarteInfo<IDCartesPrincipal::MOTEUR_ASCENSEUR_D> {
		using typeCarte = CarteAsservissement2009;
		enum : std::uint8_t { idCarte = IDCartesPrincipal::ID_CARTE_MOTEUR_ASCENSEUR_D };
	};

	template <>
	struct CarteInfo<IDCartesPrincipal::MOTEUR_AVALEUR_G> {
		using typeCarte = CarteAsservissement2009;
		enum : std::uint8_t { idCarte = IDCartesPrincipal::ID_CARTE_MOTEUR_AVALEUR_G };
	};

	template <>
	struct CarteInfo<IDCartesPrincipal::MOTEUR_AVALEUR_D> {
		using typeCarte = CarteAsservissement2009;
		enum : std::uint8_t { idCarte = IDCartesPrincipal::ID_CARTE_MOTEUR_AVALEUR_D };
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
