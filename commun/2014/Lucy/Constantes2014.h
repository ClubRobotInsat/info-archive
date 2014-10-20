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

struct IDCartesLucy2014 {
	enum {
		CAN_USB 			= 0,
		DEPLACEMENT 		= 1,
		CONTACTEURS 		= 3,
		SERVOS 			= 4,
		EVITEMENT 			= 8,
		DEBUG_DESSIN 		= 5,
		IHM 				= 7,
		POMPE 			= 9,
		IO					= 3
	};
};

namespace ConstantesLucy2014 {
	
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
	
	//Position des servos
	extern const int positionsServoCatapulte[SERVO_LUCY_NBR];
	extern const int positionsServoLanceG[SERVO_LUCY_NBR];
	extern const int positionsServoLanceD[SERVO_LUCY_NBR];
	
	/***********************************/
	/************ CARTES ***************/
	/***********************************/
	template<int ID_CARTE>
	struct CarteLucy {};
	
	template<>
	struct CarteLucy<IDCartesLucy2014::CAN_USB> {
		typedef CarteCAN_USB type;
	};

	template<>
	struct CarteLucy<IDCartesLucy2014::DEPLACEMENT> {
		typedef CarteDeplacement2009 type;
		static constexpr char const *name = "Carte déplacement";
	};
	template<>
	struct CarteLucy<IDCartesLucy2014::SERVOS> {
		typedef CarteServosNova2009 type;
	};
	
	template<>
	struct CarteLucy<IDCartesLucy2014::EVITEMENT> {
		typedef CarteDetectAdv2009 type;
	};
	
	template<>
	struct CarteLucy<IDCartesLucy2014::DEBUG_DESSIN> {
		typedef CarteDebugDessin type;
	};
	
#ifndef TARGET_SIMULATEUR
	template<>
	struct CarteLucy<IDCartesLucy2014::IO> {
		typedef CarteIO2014 type;
	};
#endif
}

#endif

