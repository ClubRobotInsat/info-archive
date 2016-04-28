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
#include "../../simulateur/cartes/CarteIO2014.h"
#include "../../simulateur/cartes/CartePneumatique2013.h"
#include "../../simulateur/cartes/CartePneumatique2014.h"
#include "../../simulateur/cartes/CarteServosNova2009.h"
#include "../../simulateur/cartes/CarteElectrovannes2016.h"
#include "../../simulateur/robot/Carte.h"

#else

#include "../../robot/Cartes/Carte.h"
#include "../../robot/Cartes/CarteAsservissement2009.h"
#include "../../robot/Cartes/CarteCAN_USB.h"
#include "../../robot/Cartes/CarteContacteurs2007.h"
#include "../../robot/Cartes/CarteDebugDessin.h"
#include "../../robot/Cartes/CarteDeplacementSymetrieY.h"
#include "../../robot/Cartes/CarteIO2014.h"
#include "../../robot/Cartes/CartePneumatique.h"
#include "../../robot/Cartes/CartePneumatique2014.h"
#include "../../robot/Cartes/CarteServosNova2009.h"
#include "../../robot/Cartes/CarteElectrovannes2016.h"
#include "../robot/Cartes/CarteDetectAdv2009.h"

#endif

#include "../../../CodeCommun/Informations_cartesElec.h"

#include "../Commun.h"
#include "../ConstantesCommunes.h"
#include <type_traits>

namespace IDCartesPrincipal {
	enum {
		ID_CARTE_CAN_USB = 0,
		ID_CARTE_DEPLACEMENT = ID_CARTE_CHUCK_DEPLACEMENT,
		ID_CARTE_SERVO_G = ID_CARTE_CHUCK_SERVOS_GAUCHE,
		ID_CARTE_SERVO_D = ID_CARTE_CHUCK_SERVOS_DROIT,
		ID_CARTE_IO = ID_CARTE_CHUCK_IO,
		ID_CARTE_PNEUMATIQUE_HAUTE = ID_CARTE_CHUCK_PNEUMATIQUE_HAUT,
		ID_CARTE_PNEUMATIQUE_BAS = ID_CARTE_CHUCK_PNEUMATIQUE_BAS,
		ID_CARTE_EVITEMENT = ID_CARTE_CHUCK_EVITEMENT,
		ID_CARTE_ELECTROVANNES = ID_CARTE_CHUCK_ELECTROVANNES,
		ID_CARTE_DEBUG_DESSIN = 9
	};

	ENUM_NS(IDCartesPrincipal, IDCartes, CAN_USB, DEPLACEMENT, SERVO_G, SERVO_D, IO, PNEUMATIQUE_HAUTE, PNEUMATIQUE_BAS, EVITEMENT, ELECTROVANNES, DEBUG_DESSIN);

	ENUM_CLASS_NS(IDCartesPrincipal, IDCartesServo, ASCENSEUR, AUTRES);
}

namespace ConstantesPrincipal {
	using IDCartes = IDCartesPrincipal::IDCartes;
	using IDCartesServo = IDCartesPrincipal::IDCartesServo;

	/////////////////// CONTACTEURS ///////////////////
	// Numéros des contacteurs et les rôles associés
	enum class Contacteur {
		TIRETTE = 2,
		BACK_CONTACT_1 = 3,
		BACK_CONTACT_2 = 4,
		BACK_CONTACT_3 = 5,
		BACK_CONTACT_4 = 6,
		BACK_CONTACT_5 = 7,
		BACK_CONTACT_6 = 8,
	};
	/////////////////// BRAS /////////////////////

	enum class Pince {
		GAUCHE = 0,
		DROIT = 1,
	};

	enum class PositionPince {
		RENTREE = 0,
		SORTIE = 1,
		NBR = 2,
	};

	enum class PositionCuillere {
		RENTRE,
		SORTIE,
		NBR = 2,
	};

	enum class PositionPelle {
		RENTRE,
		SORTIE,
		NBR = 2,
	};

	enum class PositionParasol {
		RENTRE,
		SORTIE,
		NBR = 2,
	};

	/////////////////// POMPES | VANNES ///////////////////

	enum class Pompe { HAUT, BAS };

	enum class Vanne {
		VANNE_1 = 1,
		VANNE_2 = 2,
		VANNE_3 = 3,
		VANNE_4 = 4,
		VANNE_5 = 5,
		VANNE_6 = 6,
		VANNE_TOUTES = 7,
	};


	/////////////////// SERVOS ///////////////////

	enum class Servo {
		PINCE_D = ID_SERVO_G_PINCE_DROITE,
		PARAPLUIE = ID_SERVO_D_PARAPLUIE,
		CUILLERE = ID_SERVO_G_CUILLERE,
		PELLE = ID_SERVO_D_PELLE,
		PINCE_G = ID_SERVO_G_PINCE_GAUCHE,
		NBR = 5
	};

	enum class Servo_D { NBR = 3 };
	enum class Servo_G { NBR = 2 };

	/////////////////// SERVOS ///////////////////
	// Durée attendue lors de l'envoi d'un message aux cartes élecs dans les fonctions blonquantes autre que les servos.
	auto const TIMEOUT_ELEC = 100_ms;

	/* Conservé à titre de référence. TODO: à virer
	// Position des servos
	enum class PositionPince {STOCKAGE = 0, FERME = 1, NBR};


	/////////////////// MOTEURS ///////////////////
	// Position des moteurs

	////////////////// UTILITAIRES ///////////////////
	enum class CotePince {
	    DROIT,
	    GAUCHE,
	};
	*/
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

	// Déclaration des positions
	extern Distance const ENTRAXE_PINCES;
	extern Distance const DISTANCE_CENTRE_PINCES;

	// Déclaration des positions
	extern Angle const positionPinceD[enumToInt(PositionPince::NBR)];
	extern Angle const positionPinceG[enumToInt(PositionPince::NBR)];
	extern Angle const positionPelle[enumToInt(PositionPelle::NBR)];
	extern Angle const positionCuillere[enumToInt(PositionCuillere::NBR)];
	extern Angle const positionParasol[enumToInt(PositionParasol::NBR)];

	/////////////////// CARTES ///////////////////

	template <IDCartesPrincipal::IDCartes ID_CARTE>
	struct CarteInfo {};

	template <IDCartesPrincipal::IDCartesServo ID_CARTE>
	struct CarteServoInfo {};

	template <>
	struct CarteInfo<IDCartesPrincipal::CAN_USB> : public std::integral_constant<uint32_t, IDCartesPrincipal::ID_CARTE_CAN_USB> {
		typedef CarteCAN_USB type;
	};
	template <>
	struct CarteInfo<IDCartesPrincipal::DEPLACEMENT>
	    : public std::integral_constant<uint32_t, IDCartesPrincipal::ID_CARTE_DEPLACEMENT> {
#ifdef TARGET_SIMULATEUR
		typedef CarteDeplacement2009 type;
#else
		typedef CarteDeplacementSymetrieY type;
#endif
	};
	template <>
	struct CarteInfo<IDCartesPrincipal::SERVO_G> : public std::integral_constant<uint32_t, IDCartesPrincipal::ID_CARTE_SERVO_G> {
		typedef CarteServosNova2009 type;
	};
	template <>
	struct CarteInfo<IDCartesPrincipal::SERVO_D> : public std::integral_constant<uint32_t, IDCartesPrincipal::ID_CARTE_SERVO_D> {
		typedef CarteServosNova2009 type;
	};
	template <>
	struct CarteInfo<IDCartesPrincipal::PNEUMATIQUE_BAS>
	    : public std::integral_constant<uint32_t, IDCartesPrincipal::ID_CARTE_PNEUMATIQUE_BAS> {
		typedef CartePneumatique2014 type;
	};
	template <>
	struct CarteInfo<IDCartesPrincipal::PNEUMATIQUE_HAUTE>
	    : public std::integral_constant<uint32_t, IDCartesPrincipal::ID_CARTE_PNEUMATIQUE_HAUTE> {
		typedef CartePneumatique2014 type;
	};
	template <>
	struct CarteInfo<IDCartesPrincipal::ELECTROVANNES>
	    : public std::integral_constant<uint32_t, IDCartesPrincipal::ID_CARTE_ELECTROVANNES> {
		typedef CarteElectrovannes2016 type;
	};
	template <>
	struct CarteInfo<IDCartesPrincipal::IO> : public std::integral_constant<uint32_t, IDCartesPrincipal::ID_CARTE_IO> {
		typedef CarteIO2014 type;
	};
	template <>
	struct CarteInfo<IDCartesPrincipal::EVITEMENT>
	    : public std::integral_constant<uint32_t, IDCartesPrincipal::ID_CARTE_EVITEMENT> {
		typedef CarteDetectAdv2009 type;
	};
	template <>
	struct CarteInfo<IDCartesPrincipal::DEBUG_DESSIN>
	    : public std::integral_constant<uint32_t, IDCartesPrincipal::ID_CARTE_DEBUG_DESSIN> {
		typedef CarteDebugDessin type;
	};

	/*
	template <>
	struct CarteServoInfo<IDCartesPrincipal::IDCartesServo::ASCENSEUR>
	    : public ConstantesPrincipal::CarteInfo<IDCartesPrincipal::SERVOS_ASCENSEUR> {
	    static auto const IDCarte = IDCartesPrincipal::SERVOS_ASCENSEUR;
	    typedef ConstantesPrincipal::ServoAscenseur Servo;
	};
	template <>
	struct CarteServoInfo<IDCartesPrincipal::IDCartesServo::AUTRES> : public CarteInfo<IDCartesPrincipal::SERVOS_AUTRES>
	{
	    static auto const IDCarte = IDCartesPrincipal::SERVOS_AUTRES;
	    typedef ConstantesPrincipal::Servo Servo;
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
