//
// Created by benji on 07/10/16.
//

#ifndef ROOT_SPHERE_H
#define ROOT_SPHERE_H

#include "Object3D.h"

class Sphere : public Object3D {

public:
	/**
	 * Crée un objet 3D en forme de sphère
	 * @param radius
	 * @param position correspond au centre de la sphère
	 * @param type = Sphere
	 * @param mass par défaut : LIGHT = 100_g
	 * @param world monde du simulateur qui possède la sphère
	 * @param color couleur d'affichage
	 */
	Sphere(Length radius, Vector3m position, Type type, Mass mass, World& world, std::string color);
	Sphere(Length radius, Length x, Length y, Length z, Type type, Mass mass, World& world, std::string color);

	/**
	 * Crée un message JSON propre aux sphères à envoyer au serveur
	 */
	virtual JSON getCreationMessage() override;

private:
	/// rayon de la sphère
	Length _radius;
};


#endif // ROOT_SPHERE_H
