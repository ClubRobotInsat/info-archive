//
// Created by paul on 18/02/16.
//

#ifndef ROOT_BEACHCABIN_H
#define ROOT_BEACHCABIN_H

#include "Object3D.h"

class BeachCabin : public Object3D {

public:
	/**
	 * Crée un objet 3D qui symbolise une cabine de plage
	 * Cet élément appartient à la table : il faut désactiver sa simulation (use enableSimulation(false) )
	 * @param position en (x,y) : correspond au centre de la cabine à la base  ;  en (z) ; correspond à la hauteur
	 * minimale
	 * @param world monde du simulateur qui possède la cabine de plage
	 * @param color couleur d'affichage
	 */
	BeachCabin(Vector3m position, World& world, std::string color);
	BeachCabin(Length x, Length y, Length z, World& world, std::string color);

	/**
	 * Crée un message JSON propre aux cabines de plage à envoyer au serveur
	 */
	virtual JSON getCreationMessage() override;
};


#endif // ROOT_BEACHCABIN_H
