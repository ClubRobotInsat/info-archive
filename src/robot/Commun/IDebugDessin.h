//
//  IDebugDessin.h
//  Club Robot
//
//  Created by Rémi on 21/12/2014.
//

#ifndef Club_Robot_IDebugDessin_h
#define Club_Robot_IDebugDessin_h

#include "../../commun/Commun.h"
#include "../Cartes/CarteDebugDessin.h"

namespace Commun {

	class IDebugDessin {
	public:
		virtual void setTailleGrille(Vector2u16 taille) = 0;
		// modifie la position d'affichage de la grille
		virtual void setCoordExtremite(Vector2m coordHautGauche, Vector2m coordBasDroit) = 0;
		// modifie la hauteur d'affichage de la grille
		virtual void setHauteur(Distance hauteur) = 0;
		// modifie la couleur d'une case
		virtual void setCouleurCase(CarteDebugDessin::Couleur couleur) = 0;
	};
}

#endif
