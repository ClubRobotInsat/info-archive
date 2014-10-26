// Constantes2014.h

#ifndef CONSTANTES_LUCY_2014_H
#define CONSTANTES_LUCY_2014_H


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

#define STRUCT_NAMESPACE(STRUCT_NAME, NAMESPACE_NAME, ...) \
struct STRUCT_NAME __VA_ARGS__; \
namespace NAMESPACE_NAME __VA_ARGS__

#define EXPLICIT_INSTANCIATION(SCOPE1, SCOPE2, ...) \
template<> struct SCOPE1::__VA_ARGS__ \
template<> struct SCOPE2::__VA_ARGS__


// Définit la struct IDCartesLucy2014, et le namespace IDCartesLucy2014Namespace.
// Les 2 ont le même contenu, la struct peut être utilisée en paramètre template et le namespace peut être mis dans un using namespace.
STRUCT_NAMESPACE(IDCartesLucy2014, IDCartesLucy2014Namespace,
				 {
					 enum {
						 CAN_USB 			= 0,
						 DEPLACEMENT 		= 1,
						 CONTACTEURS 		= 3,
						 SERVOS 			= 4,
						 EVITEMENT 			= 8,
						 DEBUG_DESSIN 		= 5,
						 IHM 				= 7,
						 POMPE 				= 9,
						 IO					= 3
					 };
				 }
				 )

// Le contenu de ce namespace va être ajouté enrichi d'autres éléments plus bas, et son contenu final sera copié dans une struct ConstantesLucy2014.
namespace ConstantesLucy2014Namespace {
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

// Définit la struct ConstantesLucy2014, et le namespace ConstantesLucy2014Namespace.
// Les 2 ont le même contenu, la struct peut être utilisée en paramètre template et le namespace peut être mis dans un using namespace.
STRUCT_NAMESPACE(ConstantesLucy2014, ConstantesLucy2014Namespace,
				 {
					 using NumContacteurs = ConstantesLucy2014Namespace::NumContacteurs;
					 using idFresques = ConstantesLucy2014Namespace::idFresques;
					 using Servos = ConstantesLucy2014Namespace::Servos;
					 using PositionServos = ConstantesLucy2014Namespace::PositionServos;
					 
					 //Position des servos
					 const int positionsServoCatapulte[PositionServos::SERVO_LUCY_NBR] = {1350, 1500};
					 const int positionsServoLanceG[PositionServos::SERVO_LUCY_NBR] = {1760, 1900};
					 const int positionsServoLanceD[PositionServos::SERVO_LUCY_NBR] = {1900, 2150};

					 /***********************************/
					 /************ CARTES ***************/
					 /***********************************/
					 template<int ID_CARTE>
					 struct CarteInfo {};
				 }
				 )

EXPLICIT_INSTANCIATION(ConstantesLucy2014, ConstantesLucy2014Namespace,
					   CarteInfo<IDCartesLucy2014::CAN_USB> {
						   typedef CarteCAN_USB type;
					   };
					   )

EXPLICIT_INSTANCIATION(ConstantesLucy2014, ConstantesLucy2014Namespace,
					   CarteInfo<IDCartesLucy2014::DEPLACEMENT> {
						   typedef CarteDeplacement2009 type;
						   static constexpr char const *name = "Carte déplacement";
					   };)
EXPLICIT_INSTANCIATION(ConstantesLucy2014, ConstantesLucy2014Namespace,
					   CarteInfo<IDCartesLucy2014::SERVOS> {
						   typedef CarteServosNova2009 type;
					   };
					   )

EXPLICIT_INSTANCIATION(ConstantesLucy2014, ConstantesLucy2014Namespace,
					   CarteInfo<IDCartesLucy2014::EVITEMENT> {
						   typedef CarteDetectAdv2009 type;
					   };
					   )

EXPLICIT_INSTANCIATION(ConstantesLucy2014, ConstantesLucy2014Namespace,
					   CarteInfo<IDCartesLucy2014::DEBUG_DESSIN> {
						   typedef CarteDebugDessin type;
					   };
					   )

EXPLICIT_INSTANCIATION(ConstantesLucy2014, ConstantesLucy2014Namespace,
					   CarteInfo<IDCartesLucy2014::IO> {
						   typedef CarteIO2014 type;
					   };
					   )

#endif

