//
// Created by paul on 04/02/16.
//

#ifndef ROOT_CYLINDER_H
#define ROOT_CYLINDER_H

#include "../core/Object3D.h"

class Cylinder : public Object3D {

public:
	/**
	 * Crée un objet 3D en forme de cylindre
	 * @param radius
	 * @param sizeZ
	 * @param position correspond au point du centre du cercle à la base du cylindre
	 * @param type = Cylinder
	 * @param mass par défaut : LIGHT = 100_g
	 * @param world monde du simulateur qui possède le cylindre
	 * @param color couleur d'affichage
	 */
	Cylinder(Length radius, Length sizeZ, Vector3m position, Type type, Mass mass, World& world, std::string color);
	Cylinder(Length radius, Length sizeZ, Length x, Length y, Length z, Type type, Mass mass, World& world, std::string color);

	/**
	 * Crée un message JSON propre aux cylindres à envoyer au serveur
	 */
	virtual JSON getCreationMessage() override;

	/**
	 * Crée une matrice qui permet au cylindre de réaliser des rotations plausibles
	 */
	Matrix4f generateTransformationMatrix() override;

private:
	/// rayon du cylindre
	Length _radius;
	/// hauteur du cylindre
	Length _height;
};


#endif // ROOT_CYLINDER_H
