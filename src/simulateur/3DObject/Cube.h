//
// Created by benjamin on 18/02/16.
//

#ifndef ROOT_CUBE_H
#define ROOT_CUBE_H

#include "Object3D.h"

class Cube : public Object3D {

public:
	/**
	 * Crée un objet 3D en forme de cube
	 * @param width
	 * @param length
	 * @param position en (x,y) : correspond au centre du carré à la base  ;  en (z) ; correspond à la hauteur minimale
	 * @param sizeZ
	 * @param type = Cube
	 * @param mass par défaut : LIGHT = 100_g
	 * @param world monde du simulateur qui possède le cube
	 * @param color couleur d'affichage
	 */
	Cube(Length width, Length length, Vector3m position, Length sizeZ, Type type, Mass mass, World& world, std::string color);
	Cube(Length width, Length length, Length x, Length y, Length z, Length sizeZ, Type type, Mass mass, World& world, std::string color);

	/**
	 * Crée un message JSON propre aux cubes à envoyer au serveur
	 */
	virtual JSON getCreationMessage() override;
};


#endif // ROOT_CUBE_H