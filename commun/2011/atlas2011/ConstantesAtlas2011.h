// Constantes2010.h

#ifndef CONSTANTES_2010_H
#define CONSTANTES_2010_H

#include <iostream>

namespace ConstantesAtlas2011
{
// Type d'objet ramassable
enum TypeObjetRamassableAtlas
{
	OBJET_RIEN = 0,
	OBJET_TOMATE,
	OBJET_ORANGE,
	OBJET_MAIS
};

// Type d'objet ramasse avec la pince a l'avant
enum ResultatRamassagePinceAtlas
{
	OBJET_RAMASSE_RIEN = 0,
	OBJET_RAMASSE_TOMATE,
	OBJET_RAMASSE_ORANGE, // cas d'une orange tombee a terre
	OBJET_RAMASSE_MAIS,
	OBJET_RAMASSE_MAIS_NOIR,
	RESERVOIR_PLEIN
};

// Numeros des contacteurs et les roles associes
enum NumContacteurAtlas
{
	CONTACTEUR_ATLAS_TIRETTE = 0,
	CONTACTEUR_ATLAS_CHOIX_EQUIPE = 1,
	CONTACTEUR_ATLAS_RECALAGE_ASCENSEUR = 2,
	CONTACTEUR_ATLAS_RECALAGE_DETECT_ADV = 3,
	CONTACTEUR_ATLAS_ROBOT_AVANT = 4,
	CONTACTEUR_ATLAS_ROBOT_ARRIERE = 5
};

// Numeros des servos
// Indices des differents servos de ramassage dans le tableau de positions
enum ServosAvantAtlas{
	SERVO_ATLAS_AVANT_PINCE_GAUCHE = 0,
	SERVO_ATLAS_AVANT_PINCE_DROITE,
	SERVO_ATLAS_AVANT_NBR
};



// Numeros des positions des servos et moteur asservi et les roles associes
enum PositionPinceAtlasNum {POSITION_ATLAS_PINCE_OUVERTE = 0, POSITION_PINCE_BLOCAGE_PION, POSITION_ATLAS_PINCE_FERMEE, POSITION_ATLAS_PINCE_NBR};

// angles des positions des servos et moteur asservi (en valeur interne du servo)
static const int positionPinceGaucheAtlas[POSITION_ATLAS_PINCE_NBR] = {700, 1240, 1530};			// ouvert,  bloque tomate, ferme
static const int positionPinceDroiteAtlas[POSITION_ATLAS_PINCE_NBR] = {2380, 2050,  1575};		// ouvert, bloque tomate, ferme

}
#endif //CONSTANTES_2010_H

