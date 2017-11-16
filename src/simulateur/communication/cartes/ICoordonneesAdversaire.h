// ICoordonneesAdversaire.h - utilisé pour détecter l'adversaire

#ifndef ICOORDONNEES_ADVERSAIRE_H
#define ICOORDONNEES_ADVERSAIRE_H

#include "Commun.h"

class ICoordonneesAdversaire {
public:
	// Position de l'adversaire
	virtual const Vector2m& getPositionAdversaire() const = 0;

	// Angle de l'adversaire
	virtual Angle getAngle2dAdversaire() const = 0;
};

#endif // ICOORDONNEES_ADVERSAIRE_H
