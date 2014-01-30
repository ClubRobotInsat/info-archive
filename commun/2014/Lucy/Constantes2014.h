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

#else

#include "../../../robot/Robot/Carte.h"
#include "../../../robot/Cartes/CarteDeplacement2009.h"
#include "../../../robot/Cartes/CarteContacteurs2007.h"
#include "../../../robot/Cartes/CarteIHM.h"
#include "../../../robot/Cartes/CarteServosNova2009.h"
#include "../../../robot/Cartes/CarteDebugDessin.h"
#include "../../../robot/Cartes/CartePneumatique.h"
#include "../../../robot/Cartes/CarteDetectAdv2009.h"

#endif

namespace ConstantesLucy2014 {
	
	/***********************************/
	/********** CONTACTEURS ************/
	/***********************************/
	
	enum NumContacteurs {
		CONTACTEUR_TIRETTE = 1,
		CONTACTEUR_COULEUR = 0,
	};
	
	
	/***********************************/
	/************ SERVOS ***************/
	/***********************************/
	
	//ID des servos
	enum Servos {
		SERVO_CATAPULTE 	= 0,
		SERVO_LANCE_G 		= 1,
		SERVO_LANCE_D 		= 2,
		SERVO_NBR
	};
	
	//Index des positions dans le tableau
	enum PositionServos {
		SERVO_LUCY_RENTRE = 0,
		SERVO_LUCY_SORTIE,
		SERVO_LUCY_NBR
	};
	
	//Position des servos
	extern const int positionsServoCatapulte[SERVO_LUCY_NBR];
	extern const int positionsServoLanceG[SERVO_LUCY_NBR];
	extern const int positionsServoLanceD[SERVO_LUCY_NBR];
	
	/***********************************/
	/************ CARTES ***************/
	/***********************************/
	
	namespace IDCartesLucy2014 {
		enum {
			ID_CARTE_CAN_USB 			= 0,
			ID_CARTE_DEPLACEMENT 		= 1,
			ID_CARTE_CONTACTEURS 		= 3,
			ID_CARTE_SERVOS 			= 4,
			ID_CARTE_EVITEMENT 			= 8,
			ID_CARTE_DEBUG_DESSIN 		= 6,
			ID_CARTE_IHM 				= 7,
			ID_CARTE_POMPE 				= 9
		};
	}
	using namespace IDCartesLucy2014;
	
	template<int ID_CARTE>
	struct CarteLucy {};
	
	/*template<>
	 struct CarteToumai<ID_CARTE_CAN_USB> {
	 typedef int type;
	 };*/
	template<>
	struct CarteLucy<ID_CARTE_DEPLACEMENT> {
		typedef CarteDeplacement2009 type;
		static constexpr char const *name = "Carte déplacement";
	};
	template<>
	struct CarteLucy<ID_CARTE_SERVOS> {
		typedef CarteServosNova2009 type;
	};
	template<>
	struct CarteLucy<ID_CARTE_EVITEMENT> {
		typedef CarteDetectAdv2009 type;
	};
	template<>
	struct CarteLucy<ID_CARTE_IHM> {
		typedef CarteIHM type;
	};
	template<>
	struct CarteLucy<ID_CARTE_CONTACTEURS> {
		typedef CarteContacteurs2007 type;
	};
	template<>
	struct CarteLucy<ID_CARTE_POMPE> {
#ifdef TARGET_SIMULATEUR
		typedef CartePneumatique2013 CartePneumatique;
#endif
		typedef CartePneumatique type;
	};
	template<>
	struct CarteLucy<ID_CARTE_DEBUG_DESSIN> {
		typedef CarteDebugDessin type;
	};
	
}

#endif

