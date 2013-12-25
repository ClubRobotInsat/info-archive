// Constantes2013.h

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
		SERVO_BRAS_DROIT = 1,
		SERVO_BRAS_GAUCHE = 2,
		SERVO_RAMASSE = 0,
		SERVO_EJECTE = 3,
		SERVO_NBR = 4
	};
	
	//Index des positions dans le tableau
	enum PositionServosBras {
		SERVO_OUVRE_CADEAU = 0,
		SERVO_MILIEU_BRAS,
		SERVO_SOUFFLE_BOUGIE,
		SERVO_RANGE,
		SERVO_BRAS_NBR_POSITIONS
	};
	
	//Index des positions dans le tableau
	enum PositionServosRamasse {
		SERVO_BAS = 0,
		SERVO_MILIEU,
		SERVO_RELEVE,
		SERVO_RAMASSE_NBR_POSITIONS
	};
	
	//Position des servos
	extern const int positionsServosBrasGauche[SERVO_BRAS_NBR_POSITIONS];
	extern const int positionsServosBrasDroit[SERVO_BRAS_NBR_POSITIONS];
	extern const int positionServosRamasse[SERVO_RAMASSE_NBR_POSITIONS];
	
	
	/***********************************/
	/************ MOTEURS **************/
	/***********************************/
	
	enum VitesseRotation {
		ARRET = 0,
		CASE_1,
		CASE_2,
		CASE_3,
		CASE_4,
		CASE_5,
		LANCEUR_NBR_VITESSES
	};
	
	extern const bool SENS_EJECTION;
	
	//Vitesse de rotation du moteur du lanceur
	extern const int vitessesRotationLanceur[LANCEUR_NBR_VITESSES];
	
	namespace IDCartesLucy2014 {
		enum {
			ID_CARTE_CAN_USB = 0,
			ID_CARTE_DEPLACEMENT = 1,
			ID_CARTE_CONTACTEURS = 3,
			ID_CARTE_SERVOS = 4,
			ID_CARTE_EVITEMENT = 8,
			ID_CARTE_DEBUG_DESSIN = 6,
			ID_CARTE_IHM = 7,
			ID_CARTE_POMPE = 9
		};
	}
	using namespace IDCartesLucy2014;
	
	template<int ID_CARTE>
	struct CarteLucy {
		
	};
	
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

#endif //CONSTANTES_2013_H

