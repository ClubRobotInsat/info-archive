// Constante2010.h

#ifndef CONSTANTE_2010_H
#define CONSTANTE_2010_H

enum CouleurRobot {COULEUR_ROBOT_JAUNE = 1, COULEUR_ROBOT_BLEU = 0};

// Numeros des contacteurs et les roles associes
enum NumContacteur
{
	CONTACTEUR_TIRETTE = 0,
	CONTACTEUR_CHOIX_EQUIPE = 1,
	//CONTACTEUR_RECALAGE_ASCENCEUR = 2,
	CONTACTEUR_RECALAGE_DETECT_ADV = 3,
	CONTACTEUR_ROBOT_AVANT = 4,
	CONTACTEUR_ROBOT_ARRIERE = 5
};

// Numeros des servos
// Indices des differents servos de ramassage dans le tableau de positions
enum ServosRamassage{
	SERVO_RAMASSAGE_PINCE_GAUCHE = 0,
	SERVO_RAMASSAGE_PINCE_DROITE = 1,
	SERVO_RAMASSAGE_ORANGE_GAUCHE = 2,
	SERVO_RAMASSAGE_ORANGE_DROIT = 3
};

// Indices des differents servos de depose dans le tableau de positions
enum ServosDepose{
	SERVO_DEPOSE_PORTE = 0,
	SERVO_DEPOSE_PLANCHER = 1
};

// Numeros des positions des servos et moteur asservi et les roles associes
enum PositionPinceNum {POSITION_PINCE_NUM_OUVERT = 0, POSITION_PINCE_NUM_FERME, POSITION_PINCE_NBR};
enum PositionFourcheNum {POSITION_FOURCHE_BAS = 0, POSITION_FOURCHE_HAUT, POSITION_FOURCHE_NBR};
enum NumeroFourcheNum {NUMERO_FOURCHE_GAUCHE = 0, NUMERO_FOURCHE_DROITE, NUMERO_FOURCHE_NBR};
enum PositionPorteNum {POSITION_PORTE_OUVERT = 0, POSITION_PORTE_FERME, POSITION_PORTE_NBR};
enum PositionPlancherNum {POSITION_PLANCHER_HAUT = 0, POSITION_PLANCHER_BAS, POSITION_PLANCHER_NBR};
enum PositionAscenseurNum {POSITION_ASCENSEUR_BAS = 0, POSITION_ASCENSEUR_HAUT, POSITION_ASCENSEUR_NBR};

// angles des positions des servos et moteur asservi (en valeur interne du servo)
static const int positionPince[POSITION_PINCE_NBR] = {1900, 1239};
static const int positionPinceDroite[POSITION_PINCE_NBR] = {1200, 1810};
static const int positionAscenseur[POSITION_ASCENSEUR_NBR] = {100, 200};
static const int positionFourcheGauche[POSITION_FOURCHE_NBR] = {1900, 1239};
static const int positionFourcheDroite[POSITION_FOURCHE_NBR] = {1900, 1239};
static const int positionPorte[POSITION_PORTE_NBR] = {1900, 1239};
static const int positionPlancher[POSITION_PLANCHER_NBR] = {1200, 1810};

#endif //CONSTANTE_2010_H
