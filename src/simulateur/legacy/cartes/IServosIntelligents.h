// IServosIntelligents.h

#ifndef ISERVOS_INTELLIGENTS_H
#define ISERVOS_INTELLIGENTS_H

#include "Commun.h"

class IServosIntelligents {
public:
	IServosIntelligents() {}
	virtual ~IServosIntelligents() {}

	// Modification de l'angle d'un servomoteur
	virtual void setAngle(unsigned char idCarte, unsigned char numServo, Angle angle) = 0;

	// Lecture de l'angle d'un servomoteur intelligent
	virtual Angle getAngle(unsigned char idCarte, unsigned char numServo) = 0;

	// Indique si le mouvement du servo demandé est terminé ou pas.
	// Si estMouvementTermine() renvoie true, elle prend note du fait qu'elle a renvoyé true
	// et renverra false tant que le prochain mouvement ne sera pas terminé.
	virtual bool isMovementCompleted(unsigned char idCarte, unsigned char numServo) = 0;

	// Modification de la vitesse d'un servomoteur
	virtual void setSpeed(unsigned char idCarte, unsigned char numServo, unsigned char speed) = 0;

	// Renvoie true si le servo est bloque
	virtual bool isBlocked(unsigned char idCarte, unsigned char numServo) const = 0;
};

#endif // ISERVOS_INTELLIGENTS_H
