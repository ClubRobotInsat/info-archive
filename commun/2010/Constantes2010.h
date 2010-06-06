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

// Type d'objet ramassable
enum TypeObjetRamassable
{
	OBJET_RIEN = 0,
	OBJET_TOMATE,
	OBJET_ORANGE,
	OBJET_MAIS
};

// Type d'objet ramasse avec la pince a l'avant
enum ResultatRamassagePince
{
	OBJET_RAMASSE_RIEN = 0,
	OBJET_RAMASSE_TOMATE,
	OBJET_RAMASSE_ORANGE, // cas d'une orange tombee a terre
	OBJET_RAMASSE_MAIS,
	OBJET_RAMASSE_MAIS_NOIR,
	RESERVOIR_PLEIN
};

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
	SERVO_ARRIERE_LOQUET,
	SERVO_ARRIERE_NBR
};

// Indices des telemetres servant au ramassage
enum TelemetresRamassage{
    TELEMETRE_PINCE = 0,
    TELEMETRE_COMPTAGE_BAS,
    TELEMETRE_COMPTAGE_HAUT,
    TELEMETRE_RAMASSAGE_NBR
};

// Numeros des positions des servos et moteur asservi et les roles associes
enum PositionPinceNum {POSITION_PINCE_OUVERTE = 0, POSITION_PINCE_LACHER_OBJET, POSITION_PINCE_BLOCAGE_TOMATE, POSITION_PINCE_BLOCAGE_MAIS, POSITION_PINCE_FERMEE, POSITION_PINCE_NBR};
enum PositionFourcheNum {POSITION_FOURCHE_TRES_BAS = 0, POSITION_FOURCHE_BAS, POSITION_FOURCHE_MILIEU, POSITION_FOURCHE_MILIEU_PLUS, POSITION_FOURCHE_HAUT, POSITION_FOURCHE_TRES_HAUT, POSITION_FOURCHE_NBR};
enum NumeroFourcheNum {NUMERO_FOURCHE_GAUCHE = 0, NUMERO_FOURCHE_DROITE, NUMERO_FOURCHE_NBR};
enum PositionPorteNum {POSITION_PORTE_OUVERTE = 0, POSITION_PORTE_FERMEE, POSITION_PORTE_NBR};
enum PositionPlancherNum {POSITION_PLANCHER_HAUT = 0, POSITION_PLANCHER_MILIEU, POSITION_PLANCHER_BAS, POSITION_PLANCHER_NBR};
enum PositionLoquetNum {POSITION_LOQUET_OUVERT = 0, POSITION_LOQUET_FERME, POSITION_LOQUET_NBR};
enum PositionAscenseurNum {POSITION_ASCENSEUR_BAS = 0, POSITION_ASCENSEUR_PENTE, POSITION_ASCENSEUR_ARRACHAGE, POSITION_ASCENSEUR_HAUT_TOMATE, POSITION_ASCENSEUR_HAUT_MAIS, POSITION_ASCENSEUR_RECALAGE, POSITION_ASCENSEUR_NBR};

// angles des positions des servos et moteur asservi (en valeur interne du servo)
static const int positionPinceGauche[POSITION_PINCE_NBR] = {700, 1050, 1240, 1425, 1530};			// ouvert, lache objet, bloque tomate, bloque mais, ferme
static const int positionPinceDroite[POSITION_PINCE_NBR] = {2380, 2100, 2050, 1750, 1575};		// ouvert, lache objet, bloque tomate, bloque mais, ferme

static const int positionAscenseur[POSITION_ASCENSEUR_NBR] = {-10000, -8500, -5000, -1900, -300, 0}; // bas, pos arrachage, haut (position bascule)), hauteur en mm
static const int positionFourcheGauche[POSITION_FOURCHE_NBR] = {930, 1210, 1800, 1870, 2080, 2400}; //tres_bas, bas, milieu, milieu_plus, haut, tres_haut
static const int positionFourcheDroite[POSITION_FOURCHE_NBR] = {2050, 1780, 1180, 1100, 900, 700}; //tres_bas, bas, milieu, milieu_plus, haut, tres_haut
static const int positionPorte[POSITION_PORTE_NBR] = {600, 1680}; 				// ouvert, ferme
static const int positionLoquet[POSITION_LOQUET_NBR] = {1390, 720}; 				// ouvert, ferme
static const int positionPlancher[POSITION_PLANCHER_NBR] = {1550, 2075, 2380}; 		// haut = releve, milieu, bas = abaisse

static const int deltaPositionServo = 20; // > resolution sur les valeurs de position des servos
static const int deltaPositionMoteur = 5; // > resolution sur les valeurs de position des moteurs (TODO : mettre la bonne valeur)

// seuils de detection des telemetres de ramassage
static const int seuilTelemetreRamassage[TELEMETRE_RAMASSAGE_NBR] = {10, 10, 10}; // TODO : mettre les bonnes valeurs

#endif //CONSTANTE_2010_H
