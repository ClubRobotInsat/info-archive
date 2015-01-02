// Constantes2015.h

#ifndef CONSTANTES_LUCY_2015_H
#define CONSTANTES_LUCY_2015_H


#ifdef TARGET_SIMULATEUR

#include "../../../simulateur/simulateur/Robot/Carte.h"
#include "../../../simulateur/cartes/CarteDeplacement2009.h"
#include "../../../simulateur/cartes/CarteContacteurs2007.h"
#include "../../../simulateur/cartes/CarteIHM.h"
#include "../../../simulateur/cartes/CarteServosNova2009.h"
#include "../../../simulateur/cartes/CarteDebugDessin.h"
#include "../../../simulateur/cartes/CartePneumatique2013.h"
#include "../../../simulateur/cartes/CarteDetectAdv2009.h"
#include "../../../simulateur/cartes/CarteIO2014.h"
#include "../../../simulateur/cartes/CarteCAN_USB.h"

#else

#include "../../../robot/Cartes/Carte.h"
#include "../../../robot/Cartes/CarteDeplacement2009.h"
#include "../../../robot/Cartes/CarteContacteurs2007.h"
#include "../../../robot/Cartes/CarteIHM.h"
#include "../../../robot/Cartes/CarteServosNova2009.h"
#include "../../../robot/Cartes/CarteDebugDessin.h"
#include "../../../robot/Cartes/CartePneumatique.h"
#include "../../../robot/Cartes/CarteDetectAdv2009.h"
#include "../../../robot/Cartes/CarteIO2014.h"
#include "../../../robot/Cartes/CarteCAN_USB.h"

#endif

#include "../../Enum.h"
#include "../ConstantesCommunes2015.h"

// Définit la struct IDCartesLucy2015Type, et le namespace IDCartesLucy2015.
// Les 2 ont le même contenu, la struct peut être utilisée en paramètre template et le namespace peut être mis dans un using namespace.
namespace IDCartesLucy2015 {
	ENUM_NO_IMPL(IDCartes,
		 CAN_USB 			/*= 0*/,
		 DEPLACEMENT 		/*= 1*/,
		 PLACEHOLDER2,
		 CONTACTEURS 		/*= 3*/,
		 SERVOS 			/*= 4*/,
		 DEBUG_DESSIN 		/*= 5*/,
		 IO					/*= 6*/,
		 IHM 				/*= 7*/,
		 EVITEMENT 			/*= 8*/,
		 POMPE 				/*= 9*/
	);
}

ENUM_IMPL(IDCartes, IDCartesLucy2015);

STRUCT_NAMESPACE(IDCartesLucy2015Type, IDCartesLucy2015,
	{
		using IDCartes = IDCartesLucy2015::IDCartes;
	}
)

// Le contenu de ce namespace va être ajouté enrichi d'autres éléments plus bas, et son contenu final sera copié dans une struct ConstantesLucy2015Type.
namespace ConstantesLucy2015 {
	/***********************************/
	/********** CONTACTEURS ************/
	/***********************************/

	enum class Contacteur {
		TIRETTE 		= 1,
		COULEUR 		= 3
	};

	/***********************************/
	/************ SERVOS ***************/
	/***********************************/

	// ID des servos
	enum class Servo {
		CONTREPOIDS 	= 1,
		TAPIS_D 		= 2,
		TAPIS_G 		= 0,
		NBR
	};

	// Index des positions dans le tableau
	enum class PositionServo {
		OUVERT 		= 0,
		FERME		= 1,
		NBR
	};
}

// Définit la struct ConstantesLucy2015Type, et le namespace ConstantesLucy2015.
// Les 2 ont le même contenu, la struct peut être utilisée en paramètre template et le namespace peut être mis dans un using namespace.
STRUCT_NAMESPACE(ConstantesLucy2015Type, ConstantesLucy2015,
	{
		using Contacteur = ConstantesLucy2015::Contacteur;
		using Servo = ConstantesLucy2015::Servo;
		using PositionServo = ConstantesLucy2015::PositionServo;

		// Position des servos
		static constexpr angleRad positionContrepoids[enumToInt(PositionServo::NBR)] = {1350_mrad, 1500_mrad};
		static constexpr angleRad positionTapis[enumToInt(PositionServo::NBR)] = {1760_mrad, 1900_mrad};

		static constexpr dureeS TIMEOUT_DEPLACEMENT_DEFAUT = Constantes2015::TIMEOUT_DEPLACEMENT_DEFAUT;
		static constexpr vitesseM_s VITESSE_LINEAIRE_DEFAUT = Constantes2015::VITESSE_LINEAIRE_DEFAUT;
		static constexpr vitesseRad_s VITESSE_ANGULAIRE_DEFAUT = Constantes2015::VITESSE_ANGULAIRE_DEFAUT;
		static constexpr distanceM PRECISION_LINEAIRE_DEFAUT = Constantes2015::PRECISION_LINEAIRE_DEFAUT;
		static constexpr angleRad PRECISION_ANGULAIRE_DEFAUT = Constantes2015::PRECISION_ANGULAIRE_DEFAUT;

		static constexpr distanceM RAYON_ROTATION_ROBOT = 230_mm;
		static constexpr Vector3m TAILLE_ROBOT = {25_cm, 30_cm, 21_cm};

		/***********************************/
		/************ CARTES ***************/
		/***********************************/
		template<int ID_CARTE>
		struct CarteInfo {};
	}
)

EXPLICIT_INSTANCIATION(ConstantesLucy2015Type, ConstantesLucy2015,
	CarteInfo<IDCartesLucy2015::CAN_USB> {
		typedef CarteCAN_USB type;
	};
)
EXPLICIT_INSTANCIATION(ConstantesLucy2015Type, ConstantesLucy2015,
	CarteInfo<IDCartesLucy2015::DEPLACEMENT> {
		typedef CarteDeplacement2009 type;
		static constexpr char const *name = "Carte déplacement";
	};
)
EXPLICIT_INSTANCIATION(ConstantesLucy2015Type, ConstantesLucy2015,
	CarteInfo<IDCartesLucy2015::SERVOS> {
		typedef CarteServosNova2009 type;
		typedef ConstantesLucy2015::Servo Servo;
	};
)

EXPLICIT_INSTANCIATION(ConstantesLucy2015Type, ConstantesLucy2015,
	CarteInfo<IDCartesLucy2015::EVITEMENT> {
		typedef CarteDetectAdv2009 type;
	};
)

EXPLICIT_INSTANCIATION(ConstantesLucy2015Type, ConstantesLucy2015,
	CarteInfo<IDCartesLucy2015::DEBUG_DESSIN> {
		typedef CarteDebugDessin type;
	};
)

EXPLICIT_INSTANCIATION(ConstantesLucy2015Type, ConstantesLucy2015,
	CarteInfo<IDCartesLucy2015::IO> {
		typedef CarteIO2014 type;
	};
)

#endif

