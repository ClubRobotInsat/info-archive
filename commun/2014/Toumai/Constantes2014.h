// Constantes2014.h

#ifndef CONSTANTES_TOUMAI_2014_H
#define CONSTANTES_TOUMAI_2014_H


#ifdef TARGET_SIMULATEUR

#include "../../../simulateur/simulateur/Robot/Carte.h"
#include "../../../simulateur/cartes/CarteDeplacement2009.h"
#include "../../../simulateur/cartes/CarteContacteurs2007.h"
#include "../../../simulateur/cartes/CarteIHM.h"
#include "../../../simulateur/cartes/CarteServosNova2009.h"
#include "../../../simulateur/cartes/CarteAsservissement2009.h"
#include "../../../simulateur/cartes/CarteDebug2011.h"
#include "../../../simulateur/cartes/CarteDebugDessin.h"
#include "../../../simulateur/cartes/CartePneumatique2013.h"
#include "../../../simulateur/cartes/CarteDetectAdv2009.h"

#else

#include "../../../robot/Robot/Carte.h"
#include "../../../robot/Cartes/CarteDeplacement2009.h"
#include "../../../robot/Cartes/CarteContacteurs2007.h"
#include "../../../robot/Cartes/CarteIHM.h"
#include "../../../robot/Cartes/CarteServosNova2009.h"
#include "../../../robot/Cartes/CarteAsservissement2009.h"
#include "../../../robot/Cartes/CarteDebug2011.h"
#include "../../../robot/Cartes/CarteDebugDessin.h"
#include "../../../robot/Cartes/CartePneumatique.h"
#include "../../../robot/Cartes/CarteDetectAdv2009.h"

#endif

namespace ConstantesToumai2014 {
	
	// Numeros des contacteurs et les roles associes
	enum NumContacteur	{
		CONTACTEUR_TIRETTE 			= 1,
		CONTACTEUR_CHOIX_EQUIPE     = 0
	};
	
	enum NumContacteurAsc{
		CONTACTEUR_GAUCHE_ASCG	= 0,
		CONTACTEUR_DROIT_ASCG	= 1,
		CONTACTEUR_GAUCHE_ASCD 	= 2,
		CONTACTEUR_DROIT_ASCD	= 3
	};
	
	// Indice des différents servo de ramassage dans le tableau de positions
	enum ServosAsc {
		SERVO_G_ASC_G     		= 0,
		SERVO_G_ASC_D        	= 1,
		SERVO_D_ASC_D      		= 2,
		SERVO_D_ASC_G        	= 3,
		SERVO_ASC_NBR         	= 4
	};
	
	// Indice des différents servo de ramassage dans le tableau de positions
	enum ServosBras {
		SERVO_BRAS_G_HAUT       	= 0,
		SERVO_BRAS_D_HAUT     		= 1,
		SERVO_BRAS_G_BAS      		= 2,
		SERVO_BRAS_D_BAS        	= 3,
		SERVO_BRAS_NBR  	       	= 4
	};
	
	// Numeros des positions des servos et moteur asservi et les roles associes
	enum PositionServosPinceAsc {
		POSITION_PINCE_ASC_OUVERTE = 0,
		POSITION_PINCE_ASC_EMPILE,
		POSITION_PINCE_ASC_BLOCAGE,
		POSITION_PINCE_ASC_RANGE,
		POSITION_PINCE_ASC_NBR};
	
	enum PositionServosBras {
		POSITION_BRAS_HAUT = 0,
		POSITION_BRAS_SOUFFLE_BOUGIE,
		POSITION_BRAS_RANGE,
		POSITION_BRAS_NBR};
	
	// angles des positions des servos et moteur asservi (en valeur interne du servo)
	//static const int positionServoGAscD[POSITION_PINCE_ASC_NBR] = {980, 880, 780, 1430};
	extern const int positionServoGAscD[POSITION_PINCE_ASC_NBR];
	//static const int positionServoGAscG[POSITION_PINCE_ASC_NBR] = {1200, 1100, 1000, 600};
	extern const int positionServoGAscG[POSITION_PINCE_ASC_NBR];
	//static const int positionServoDAscD[POSITION_PINCE_ASC_NBR] = {1510, 1610, 1710, 2160};
	extern const int positionServoDAscD[POSITION_PINCE_ASC_NBR];
	//static const int positionServoDAscG[POSITION_PINCE_ASC_NBR] = {1500, 1600, 1750, 980};
	extern const int positionServoDAscG[POSITION_PINCE_ASC_NBR];
	
	// angles des positions des servos et moteur asservi (en valeur interne du servo)
	extern const int positionServoBrasGHaut[POSITION_BRAS_NBR];
	extern const int positionServoBrasDHaut[POSITION_BRAS_NBR];
	extern const int positionServoBrasGBas[POSITION_BRAS_NBR];
	extern const int positionServoBrasDBas[POSITION_BRAS_NBR];
	
	//////////////////////////////////////////// Moteurs ///////////////////////////////////////////////////////
	
	// Positions possibles pour l'ascenseur gauche et droit
	enum PositionsAsc {
		ASC_HAUT = 0,
		ASC_MILIEU,
		ASC_MAGIC,
		ASC_BAS,
		NBR_POSITIONS_ASC
	};
	
	extern const int tableauPositionsAscGauche[NBR_POSITIONS_ASC];
	extern const int tableauPositionsAscDroit[NBR_POSITIONS_ASC];
	
	namespace IDCartesToumai2014 {
		enum {
			//ID_CARTE_CAN_USB = 0,
			PREMIER_ID_CARTE = 0,
			ID_CARTE_DEPLACEMENT = PREMIER_ID_CARTE,
			ID_CARTE_ASSERVISSEMENT_ASCENSEUR_GAUCHE = 3,
			ID_CARTE_ASSERVISSEMENT_ASCENSEUR_DROIT = 2,
			ID_CARTE_SERVOS_ASCENSEUR = 4,
			ID_CARTE_SERVOS_BRAS = 5,
			ID_CARTE_CONTACTEURS_ASC = 6,
			ID_CARTE_IHM = 7,
			ID_CARTE_EVITEMENT = 8,
			ID_CARTE_PNEUMATIQUE = 9,
			ID_CARTE_DEBUG_DESSIN = 10,
			DERNIER_ID_CARTE
			//ID_CARTE_DEBUG = 11
		};
	}
	using namespace IDCartesToumai2014;
	
	template<int ID_CARTE>
	struct CarteToumai {

	};
	
	/*template<>
	struct CarteToumai<ID_CARTE_CAN_USB> {
		typedef int type;
	};*/
	template<>
	struct CarteToumai<ID_CARTE_DEPLACEMENT> {
		typedef CarteDeplacement2009 type;
		static constexpr char const *name = "Carte déplacement";
	};
	template<>
	struct CarteToumai<ID_CARTE_ASSERVISSEMENT_ASCENSEUR_DROIT> {
		typedef CarteAsservissement2009 type;
	};
	template<>
	struct CarteToumai<ID_CARTE_ASSERVISSEMENT_ASCENSEUR_GAUCHE> {
		typedef CarteAsservissement2009 type;
	};
	template<>
	struct CarteToumai<ID_CARTE_EVITEMENT> {
		typedef CarteDetectAdv2009 type;
	};
	template<>
	struct CarteToumai<ID_CARTE_IHM> {
		typedef CarteIHM type;
	};
	template<>
	struct CarteToumai<ID_CARTE_CONTACTEURS_ASC> {
		typedef CarteContacteurs2007 type;
	};
	template<>
	struct CarteToumai<ID_CARTE_SERVOS_ASCENSEUR> {
		typedef CarteServosNova2009 type;
	};
	template<>
	struct CarteToumai<ID_CARTE_SERVOS_BRAS> {
		typedef CarteServosNova2009 type;
	};
	template<>
	struct CarteToumai<ID_CARTE_PNEUMATIQUE> {
#ifdef TARGET_SIMULATEUR
		typedef CartePneumatique2013 CartePneumatique;
#endif
		typedef CartePneumatique type;
	};
	template<>
	struct CarteToumai<ID_CARTE_DEBUG_DESSIN> {
		typedef CarteDebugDessin type;
	};
}
#endif

