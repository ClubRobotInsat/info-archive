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
#include "../../../robot/2015/Commun/CarteDeplacement2015.h"
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

STRUCT_NAMESPACE(IDCartesSecondaire2015Type, IDCartesSecondaire2015,
	{
		using IDCartes = IDCartesSecondaire2015::IDCartes;
		using IDCartesServo = IDCartesSecondaire2015::IDCartesServo;
	}
)

// Le contenu de ce namespace va être enrichi d'autres éléments plus bas, et son contenu final sera copié dans une struct ConstantesSecondaire2015Type.
namespace ConstantesSecondaire2015 {
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
		TAPIS_G 		= ID_SERVO_POLANSKI_G,
		CONTREPOIDS 	= 10,
		TAPIS_D 		= ID_SERVO_POLANSKI_D,
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
}

// Définit la struct ConstantesSecondaire2015Type, et le namespace ConstantesSecondaire2015.
// Les 2 ont le même contenu, la struct peut être utilisée en paramètre template et le namespace peut être mis dans un using namespace.
STRUCT_NAMESPACE(ConstantesSecondaire2015Type, ConstantesSecondaire2015,
	{
		using ConstantesCommunes = Constantes2015Type;
		using IDCartes = IDCartesSecondaire2015Type::IDCartes;
		using IDCartesServo = IDCartesSecondaire2015Type::IDCartesServo;

		using Contacteur = ConstantesSecondaire2015::Contacteur;
		using Servo = ConstantesSecondaire2015::Servo;
		using PositionServoContrepoids = ConstantesSecondaire2015::PositionServoContrepoids;
		using PositionServoTapis = ConstantesSecondaire2015::PositionServoTapis;

		static constexpr Duree TIMEOUT_DEPLACEMENT_DEFAUT = Constantes2015::TIMEOUT_DEPLACEMENT_DEFAUT;
        static constexpr Duree TIMEOUT_BLOCAGE_ADVERSAIRE_DEFAUT = Constantes2015::TIMEOUT_BLOCAGE_ADVERSAIRE_DEFAUT;
		static constexpr VitesseLineaire VITESSE_LINEAIRE_DEFAUT = Constantes2015::VITESSE_LINEAIRE_DEFAUT;
		static constexpr VitesseAngulaire VITESSE_ANGULAIRE_DEFAUT = Constantes2015::VITESSE_ANGULAIRE_DEFAUT;
		static constexpr Distance PRECISION_LINEAIRE_DEFAUT = Constantes2015::PRECISION_LINEAIRE_DEFAUT;
		static constexpr Angle PRECISION_ANGULAIRE_DEFAUT = Constantes2015::PRECISION_ANGULAIRE_DEFAUT;

		static constexpr Distance RAYON_ROTATION_ROBOT = 230_mm;
		static constexpr Vector3m TAILLE_ROBOT = {25_cm, 30_cm, 21_cm};
		static constexpr uint16_t TCPIP_PORT_SIMU_DEFAUT = 4321;
		// Cet offset sert à aligner le 0 de la balise avec le 0 de l'info
		// NB : 0 devant, PI/-PI derrière, PI/2 à gauche et -PI/2 à droite
		static constexpr Angle OFFSET_ANGLE_ADV = -0.5_PI;

		/***********************************/
		/************ CARTES ***************/
		/***********************************/
		template<IDCartesSecondaire2015Type::IDCartes ID_CARTE>
		struct CarteInfo {};

		template<IDCartesSecondaire2015Type::IDCartesServo ID_CARTE>
		struct CarteServoInfo {};
}
)

namespace ConstantesSecondaire2015 {
	// Position des servos
	extern Angle const positionContrepoids[enumToInt(PositionServoContrepoids::NBR)];
	extern Angle const positionTapis[enumToInt(PositionServoTapis::NBR)];
}

EXPLICIT_INSTANCIATION(ConstantesSecondaire2015Type, ConstantesSecondaire2015,
	CarteInfo<IDCartesSecondaire2015::CAN_USB> : public std::integral_constant<uint32_t, ConstantesSecondaire2015::ID_CARTE_CAN_USB> {
		typedef CarteCAN_USB type;
	};
)
EXPLICIT_INSTANCIATION(ConstantesSecondaire2015Type, ConstantesSecondaire2015,
	CarteInfo<IDCartesSecondaire2015::DEPLACEMENT> : public std::integral_constant<uint32_t, ConstantesSecondaire2015::ID_CARTE_DEPLACEMENT> {
#ifdef TARGET_SIMULATEUR
		typedef CarteDeplacement2009 type;
#else
		typedef CarteDeplacement2015 type;
#endif
	};
)
EXPLICIT_INSTANCIATION(ConstantesSecondaire2015Type, ConstantesSecondaire2015,
	CarteInfo<IDCartesSecondaire2015::SERVOS> : public std::integral_constant<uint32_t, ConstantesSecondaire2015::ID_CARTE_SERVOS> {
		typedef CarteServosNova2009 type;
		typedef ConstantesSecondaire2015::Servo Servo;
	};
)
EXPLICIT_INSTANCIATION(ConstantesSecondaire2015Type, ConstantesSecondaire2015,
	CarteInfo<IDCartesSecondaire2015::DEBUG_DESSIN> : public std::integral_constant<uint32_t, ConstantesSecondaire2015::ID_CARTE_DEBUG_DESSIN> {
		typedef CarteDebugDessin type;
	};
)
EXPLICIT_INSTANCIATION(ConstantesSecondaire2015Type, ConstantesSecondaire2015,
	CarteInfo<IDCartesSecondaire2015::IO> : public std::integral_constant<uint32_t, ConstantesSecondaire2015::ID_CARTE_IO> {
		typedef CarteIO2014 type;
	};
)
EXPLICIT_INSTANCIATION(ConstantesSecondaire2015Type, ConstantesSecondaire2015,
	CarteInfo<IDCartesSecondaire2015::EVITEMENT> : public std::integral_constant<uint32_t, ConstantesSecondaire2015::ID_CARTE_EVITEMENT> {
#ifdef TARGET_SIMULATEUR
		typedef CarteDetectAdv2012 type;
#else
		typedef CarteDetectAdv2009 type;
#endif
	};
)

EXPLICIT_INSTANCIATION(ConstantesSecondaire2015Type, ConstantesSecondaire2015,
	CarteServoInfo<IDCartesSecondaire2015::IDCartesServo::SERVOS> : public ConstantesSecondaire2015Type::CarteInfo<IDCartesSecondaire2015::SERVOS> {
		static auto const IDCarte = IDCartesSecondaire2015::SERVOS;
		typedef ConstantesSecondaire2015::Servo Servo;
	};
)



#endif

