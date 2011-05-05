// Constantes2011.h

#ifndef CONSTANTES_2011_H
#define CONSTANTES_2011_H

#include <iostream>

namespace ConstantesTitan2011
{

enum Bras{
	BRAS_GAUCHE = 0,
	BRAS_DROIT = 1,
	NBR_BRAS
};

// Numeros des contacteurs et les roles associes
enum NumContacteur{
	CONTACTEUR_TIRETTE = 0,
	CONTACTEUR_CHOIX_EQUIPE = 1,
	CONTACTEUR_ASCENSEUR =2,
	CONTACTEUR_SOUS_BRAS_GAUCHE=3,
	CONTACTEUR_SOUS_BRAS_DROIT=4,
	CONTACTEUR_DEVANT_BRAS_GAUCHE = 5,
	CONTACTEUR_DEVANT_BRAS_DROIT = 6
};

enum NumCodeBarre{
	CODE_BARRE_DROIT  = 0,
	CODE_BARRE_GAUCHE = 1
};

// Numeros des servos
enum ServosNovaAscenseur{
	GRIFFE_GAUCHE = 0,
	GRIFFE_DROITE,
	PINCE_GAUCHE,
	PINCE_DROITE,
	SERVO_ASC_NBR
};

// Numeros des positions des servos et moteur asservi et les roles associes
enum PositionGriffeNum {POSITION_GRIFFE_OUVERTE = 0, POSITION_GRIFFE_BLOCAGE_PION, POSITION_GRIFFE_FERMEE, POSITION_GRIFFE_NBR};
enum PositionPinceNum {POSITION_PINCE_OUVERTE = 0, POSITION_PINCE_BLOCAGE_PION, POSITION_PINCE_FERMEE, POSITION_PINCE_NBR};
enum PositionBrasNum {POSITION_BRAS_OUVERT = 0, POSITION_BRAS_FERME, POSITION_BRAS_NBR};

enum PositionAscenseurNum {POSITION_ASCENSEUR_BAS = 0, POSITION_ASCENSEUR_POUR_CLIPSER, POSITION_ASCENSEUR_POUR_NE_PAS_CLIPSER, POSITION_ASCENSEUR_NBR};

// angles des positions des servos et moteur asservi (en valeur interne du servo)
static const int positionGriffeGauche[POSITION_GRIFFE_NBR] = {750, 1240, 1530};
static const int positionGriffeDroite[POSITION_GRIFFE_NBR] = {1550, 1240, 700};

// angles des positions des servos et moteur asservi (en valeur interne du servo)
static const int positionPinceGauche[POSITION_PINCE_NBR] = {700, 920, 2100};
static const int positionPinceDroite[POSITION_PINCE_NBR] = {2350, 1240, 700};

static const int positionBrasGauche[POSITION_BRAS_NBR]= {20000, 0};
static const int positionBrasDroit[POSITION_BRAS_NBR]={-20000, 0};

static const int positionAscenseur[POSITION_ASCENSEUR_NBR]={0, 350000, 435000};

}
#endif //CONSTANTES_2011_H
