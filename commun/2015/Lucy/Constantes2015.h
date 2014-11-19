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

	enum NumContacteurs {
		CONTACTEUR_TIRETTE = 1,
		CONTACTEUR_COULEUR = 3,
		CONTACTEUR_FRESQUE_GAUCHE = 6,
		CONTACTEUR_FRESQUE_DROITE = 5
	};

	enum idFresques {
		FRESQUE_DROITE = 0,
		FRESQUE_GAUCHE
	};


	/***********************************/
	/************ SERVOS ***************/
	/***********************************/

	//ID des servos
	enum Servos {
		SERVO_CATAPULTE 	= 1,
		SERVO_LANCE_G 		= 2,
		SERVO_LANCE_D 		= 0,
		SERVO_NBR
	};

	//Index des positions dans le tableau
	enum PositionServos {
		SERVO_LUCY_RENTRE = 0,
		SERVO_LUCY_SORTI,
		SERVO_LUCY_NBR
	};
}

// Définit la struct ConstantesLucy2015Type, et le namespace ConstantesLucy2015.
// Les 2 ont le même contenu, la struct peut être utilisée en paramètre template et le namespace peut être mis dans un using namespace.
STRUCT_NAMESPACE(ConstantesLucy2015Type, ConstantesLucy2015,
				 {
					 using NumContacteurs = ConstantesLucy2015::NumContacteurs;
					 using idFresques = ConstantesLucy2015::idFresques;
					 using Servos = ConstantesLucy2015::Servos;
					 using PositionServos = ConstantesLucy2015::PositionServos;
					 
					 //Position des servos
					 static constexpr int positionsServoCatapulte[PositionServos::SERVO_LUCY_NBR] = {1350, 1500};
					 static constexpr int positionsServoLanceG[PositionServos::SERVO_LUCY_NBR] = {1760, 1900};
					 static constexpr int positionsServoLanceD[PositionServos::SERVO_LUCY_NBR] = {1900, 2150};

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
					   };)
EXPLICIT_INSTANCIATION(ConstantesLucy2015Type, ConstantesLucy2015,
					   CarteInfo<IDCartesLucy2015::SERVOS> {
						   typedef CarteServosNova2009 type;
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
