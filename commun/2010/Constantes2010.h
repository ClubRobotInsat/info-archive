// Constante2010.h

#ifndef CONSTANTE_2010_H
#define CONSTANTE_2010_H

#include <iostream>

enum CouleurRobot {COULEUR_ROBOT_JAUNE = 1, COULEUR_ROBOT_BLEU = 0};

// Pour permettre l'affichage d'une Couleur :
inline std::ostream& operator<<(std::ostream& os, const CouleurRobot& couleur)
{
	switch(couleur)
	{
	case COULEUR_ROBOT_JAUNE:
		os << "jaune";
		break;
	case COULEUR_ROBOT_BLEU:
		os << "bleu";
		break;
	}

	return os;
}

// Numeros des contacteurs et les roles associes
enum NumContacteur
{
	CONTACTEUR_TIRETTE = 0,
	CONTACTEUR_CHOIX_EQUIPE = 1,
	CONTACTEUR_RECALAGE_ASCENSEUR = 2,
	CONTACTEUR_RECALAGE_DETECT_ADV = 3,
	CONTACTEUR_ROBOT_AVANT = 4,
	CONTACTEUR_ROBOT_ARRIERE = 5
};

// Numeros des servos

// VERSION 5 SERVOS PAR CARTE
/*// Indices des differents servos de ramassage dans le tableau de positions
enum ServosRamassage{
	SERVO_RAMASSAGE_PINCE_GAUCHE = 0,
	SERVO_RAMASSAGE_PINCE_DROITE,
	SERVO_RAMASSAGE_ORANGE_GAUCHE,
	SERVO_RAMASSAGE_ORANGE_DROIT,
	SERVO_RAMASSAGE_NBR
};

// Indices des differents servos de depose dans le tableau de positions
enum ServosDepose{
	SERVO_DEPOSE_PORTE = 0,
	SERVO_DEPOSE_PLANCHER,
	SERVO_DEPOSE_NBR
};*/

// VERSION 3 SERVOS PAR CARTE
// Indices des differents servos de ramassage dans le tableau de positions
enum ServosAvant{
	SERVO_AVANT_PINCE_GAUCHE = 0,
	SERVO_AVANT_PINCE_DROITE,
	SERVO_AVANT_PLANCHER,
	SERVO_AVANT_NBR
};

// Indices des differents servos de depose dans le tableau de positions
enum ServosArriere{
	SERVO_ARRIERE_PORTE = 0,
	SERVO_ARRIERE_ORANGE_GAUCHE,
	SERVO_ARRIERE_ORANGE_DROIT,
	SERVO_ARRIERE_NBR
};

// Incides des telemetres servant au ramassage
enum TelemetresRamassage{
    TELEMETRE_PINCE = 0,
    TELEMETRE_COMPTAGE_BAS,
    TELEMETRE_COMPTAGE_HAUT,
    TELEMETRE_RAMASSAGE_NBR
};

// Numeros des positions des servos et moteur asservi et les roles associes
enum PositionPinceNum {POSITION_PINCE_OUVERTE = 0, POSITION_PINCE_LACHER_OBJET, POSITION_PINCE_BLOCAGE_TOMATE, POSITION_PINCE_BLOCAGE_MAIS, POSITION_PINCE_FERMEE, POSITION_PINCE_NBR};
enum PositionFourcheNum {POSITION_FOURCHE_BAS = 0, POSITION_FOURCHE_MILIEU, POSITION_FOURCHE_HAUT, POSITION_FOURCHE_NBR};
enum NumeroFourcheNum {NUMERO_FOURCHE_GAUCHE = 0, NUMERO_FOURCHE_DROITE, NUMERO_FOURCHE_NBR};
enum PositionPorteNum {POSITION_PORTE_OUVERTE = 0, POSITION_PORTE_FERMEE, POSITION_PORTE_NBR};
enum PositionPlancherNum {POSITION_PLANCHER_HAUT = 0, POSITION_PLANCHER_BAS, POSITION_PLANCHER_NBR};
enum PositionAscenseurNum {POSITION_ASCENSEUR_BAS = 0, POSITION_ASCENSEUR_ARRACHAGE, POSITION_ASCENSEUR_HAUT, POSITION_ASCENSEUR_RECALAGE, POSITION_ASCENSEUR_NBR};

// angles des positions des servos et moteur asservi (en valeur interne du servo)
static const int positionPinceGauche[POSITION_PINCE_NBR] = {600, 950, 800, 1000, 1210};			// ouvert, bloque tomate, bloque mais, ferme
static const int positionPinceDroite[POSITION_PINCE_NBR] = {2375, 2175, 2150, 1950, 1750};		// ouvert, bloque tomate, bloque mais, ferme
// 1995 tomate environ
static const int positionAscenseur[POSITION_ASCENSEUR_NBR] = {-10000, -8000 /* TODO : mettre la bonne valeur*/, -1500, -1000}; 		// bas, pos arrachage, haut (position bascule)), hauteur en mm
static const int positionFourcheGauche[POSITION_FOURCHE_NBR] = {1400, 2050, 2500}; // bas, milieu, haut
static const int positionFourcheDroite[POSITION_FOURCHE_NBR] = {1400, 2050, 2500}; // bas, milieu, haut
static const int positionPorte[POSITION_PORTE_NBR] = {1600, 2140}; 				// ouvert, ferme
static const int positionPlancher[POSITION_PLANCHER_NBR] = {1580, 1370}; 		// haut = releve, bas = abaisse

static const int deltaPositionServo = 20; // > resolution sur les valeurs de position des servos
static const int deltaPositionMoteur = 5; // > resolution sur les valeurs de position des moteurs (TODO : mettre la bonne valeur)

// seuils de detection des telemetres de ramassage
static const int seuilTelemetreRamassage[TELEMETRE_RAMASSAGE_NBR] = {10, 10, 10}; // TODO : mettre les bonnes valeurs

#endif //CONSTANTE_2010_H
