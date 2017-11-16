// IDebugDessin.h

#ifndef IDEBUGDESSIN_H_
#define IDEBUGDESSIN_H_

#include "Commun.h"

class IDebugDessin {
public:
	virtual ~IDebugDessin() {}

	// modifie le nombre de case de la grille
	virtual void setTailleGrille(uint16_t grilleTailleX, uint16_t grilleTailleY) = 0;

	// modifie la position d'affichage de la grille
	virtual void setCoordExtremite(Vector2m coordHautGauche, Vector2m coordBasDroit) = 0;

	// modifie la hauteur d'affichage de la grille
	virtual void setHauteur(Distance hauteur) = 0;

	// modifie la couleur d'une case
	virtual void setCouleurCase(Vector4ub couleur) = 0;

	// Accesseur sur l'etat d'activation de l'affichage du debug
	virtual bool getEtatActivation() const = 0;

	// TODO autre forme graphique
};

#endif /*IDEBUGDESSIN_H_*/
