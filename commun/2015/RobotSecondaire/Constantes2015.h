// Constantes2015.h

#ifndef CONSTANTES_SECONDAIRE_2015_H
#define CONSTANTES_SECONDAIRE_2015_H


#ifdef TARGET_SIMULATEUR

#include "../../../simulateur/simulateur/Robot/Carte.h"
#include "../../../simulateur/cartes/CarteDeplacement2009.h"
#include "../../../simulateur/cartes/CarteContacteurs2007.h"
#include "../../../simulateur/cartes/CarteIHM.h"
#include "../../../simulateur/cartes/CarteServosNova2009.h"
#include "../../../simulateur/cartes/CarteDebugDessin.h"
#include "../../../simulateur/cartes/CartePneumatique2013.h"
#include "../../../simulateur/cartes/CarteDetectAdv2012.h"
#include "../../../simulateur/cartes/CarteIO2014.h"
#include "../../../simulateur/cartes/CarteCAN_USB.h"

#else

#include "../../../robot/Cartes/Carte.h"
#include "../../../robot/Cartes/CarteDeplacementSymetrieY.h"
#include "../../../robot/Cartes/CarteContacteurs2007.h"
#include "../../../robot/Cartes/CarteIHM.h"
#include "../../../robot/Cartes/CarteServosNova2009.h"
#include "../../../robot/Cartes/CarteDebugDessin.h"
#include "../../../robot/Cartes/CartePneumatique.h"
#include "../../../robot/Cartes/CarteDetectAdv2009.h"
#include "../../../robot/Cartes/CarteIO2014.h"
#include "../../../robot/Cartes/CarteCAN_USB.h"

#endif

#include <type_traits>
#include "../../Commun.h"
#include "../ConstantesCommunes2015.h"
#include "../../../../CodeCommun/Informations_cartesElec.h"

// Définit la struct IDCartesSecondaire2015Type, et le namespace IDCartesSecondaire2015.
// Les 2 ont le même contenu, la struct peut être utilisée en paramètre template et le namespace peut être mis dans un using namespace.
namespace IDCartesSecondaire2015 {
	ENUM_NS(IDCartesSecondaire2015, IDCartes,
		 CAN_USB,
		 DEPLACEMENT,
		 SERVOS,
		 DEBUG_DESSIN,
		 IO,
		 EVITEMENT
			);
	ENUM_CLASS_NS(IDCartesSecondaire2015, IDCartesServo,
				  SERVOS
				  );
}

// Le contenu de ce namespace va être enrichi d'autres éléments plus bas, et son contenu final sera copié dans une struct ConstantesSecondaire2015Type.
namespace ConstantesSecondaire2015 {
	using IDCartes = IDCartesSecondaire2015::IDCartes;
	using IDCartesServo = IDCartesSecondaire2015::IDCartesServo;

	enum {
		ID_CARTE_CAN_USB = 0,
		ID_CARTE_DEPLACEMENT = ID_CARTE_POLANSKI_DEPLACEMENT,
		ID_CARTE_SERVOS = ID_CARTE_POLANSKI_SERVOS,
		ID_CARTE_DEBUG_DESSIN = 10,
		ID_CARTE_IO = ID_CARTE_POLANSKI_CAPTEURS,
		ID_CARTE_EVITEMENT = ID_CARTE_POLANSKI_EVITEMENT
	};

	/***********************************/
	/********** CONTACTEURS ************/
	/***********************************/

	enum class Contacteur {
		TIRETTE 		= 2,
		CHOIX_EQUIPE 	= 3
	};

	/***********************************/
	/************ SERVOS ***************/
	/***********************************/

	// ID des servos
	enum class Servo {
		TAPIS_G 		= 2, // ID_SERVO_POLANSKI_G
		CONTREPOIDS 	= 10,
		TAPIS_D 		= 1, //ID_SERVO_POLANSKI_D,
		NBR
	};

	// Index des positions dans le tableau
	enum class PositionServoContrepoids {
		OUVERT 		= 0,
		FERME 		= 1,
		NBR
	};
	enum class PositionServoTapis {
		OUVERT 		= 0,
		NBR
	};

	extern Duree const TIMEOUT_DEPLACEMENT_DEFAUT;
	extern Duree const TIMEOUT_BLOCAGE_ADVERSAIRE_DEFAUT;
	extern VitesseLineaire const VITESSE_LINEAIRE_DEFAUT;
	extern VitesseLineaire const VITESSE_LINEAIRE_LENTE;
	extern VitesseAngulaire const VITESSE_ANGULAIRE_DEFAUT;
	extern Distance const PRECISION_LINEAIRE_DEFAUT;
	extern Angle const PRECISION_ANGULAIRE_DEFAUT;

	extern Distance const RAYON_ROTATION_ROBOT;
	extern Vector3m const TAILLE_ROBOT;
	extern uint16_t const TCPIP_PORT_SIMU_DEFAUT;
	// Cet offset sert à aligner le 0 de la balise avec le 0 de l'info
	// NB : 0 devant, PI/-PI derrière, PI/2 à gauche et -PI/2 à droite
	extern Angle const OFFSET_ANGLE_ADV;

	extern int const IDRobot;

	// Position des servos
	extern Angle const positionContrepoids[enumToInt(PositionServoContrepoids::NBR)];
	extern Angle const positionTapis[enumToInt(PositionServoTapis::NBR)];

	/***********************************/
	/************ CARTES ***************/
	/***********************************/
	template<IDCartesSecondaire2015::IDCartes ID_CARTE>
	struct CarteInfo {};

	template<IDCartesSecondaire2015::IDCartesServo ID_CARTE>
	struct CarteServoInfo {};

	template<>
	struct CarteInfo<IDCartesSecondaire2015::CAN_USB> : public std::integral_constant<uint32_t, ConstantesSecondaire2015::ID_CARTE_CAN_USB> {
		typedef CarteCAN_USB type;
	};
	template<>
	struct CarteInfo<IDCartesSecondaire2015::DEPLACEMENT> : public std::integral_constant<uint32_t, ConstantesSecondaire2015::ID_CARTE_DEPLACEMENT> {
#ifdef TARGET_SIMULATEUR
		typedef CarteDeplacement2009 type;
#else
		typedef CarteDeplacementSymetrieY type;
#endif
	};
	template<>
	struct CarteInfo<IDCartesSecondaire2015::SERVOS> : public std::integral_constant<uint32_t, ConstantesSecondaire2015::ID_CARTE_SERVOS> {
		typedef CarteServosNova2009 type;
		typedef ConstantesSecondaire2015::Servo Servo;
	};
	template<>
	struct CarteInfo<IDCartesSecondaire2015::DEBUG_DESSIN> : public std::integral_constant<uint32_t, ConstantesSecondaire2015::ID_CARTE_DEBUG_DESSIN> {
		typedef CarteDebugDessin type;
	};
	template<>
	struct CarteInfo<IDCartesSecondaire2015::IO> : public std::integral_constant<uint32_t, ConstantesSecondaire2015::ID_CARTE_IO> {
		typedef CarteIO2014 type;
	};
	template<>
	struct CarteInfo<IDCartesSecondaire2015::EVITEMENT> : public std::integral_constant<uint32_t, ConstantesSecondaire2015::ID_CARTE_EVITEMENT> {
#ifdef TARGET_SIMULATEUR
		typedef CarteDetectAdv2012 type;
#else
		typedef CarteDetectAdv2009 type;
#endif
	};

	template<>
	struct CarteServoInfo<IDCartesSecondaire2015::IDCartesServo::SERVOS> : public ConstantesSecondaire2015::CarteInfo<IDCartesSecondaire2015::SERVOS> {
		static auto const IDCarte = IDCartesSecondaire2015::SERVOS;
		typedef ConstantesSecondaire2015::Servo Servo;
	};
}

struct ConstantesRobotSecondaire2015 : public ConstantesRobot {
	virtual Duree getTimeoutDeplacementDefaut() const override;
	virtual Duree getTimeoutBlocageAdversaireDefaut() const override;
	virtual VitesseLineaire getVitesseLineaireDefaut() const override;
	virtual VitesseAngulaire getVitesseAngulaireDefaut() const override;
	virtual Distance getPrecisionLineaireDefaut() const override;
	virtual Angle getPrecisionAngulaireDefaut() const override;

	virtual Distance getRayonRotation() const override;
	virtual Vector3m getTailleRobot() const override;
	virtual Angle getOffsetAngleAdv() const override;

	virtual uint16_t getPortTCPIPDefaut() const override;

	virtual int getIDRobot() const override;
};

#endif

