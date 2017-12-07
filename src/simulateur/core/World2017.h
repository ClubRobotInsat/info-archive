//
// Created by benji on 07/10/16.
//

#ifndef ROOT_WORLD2017_H
#define ROOT_WORLD2017_H

#include <Box2D/Dynamics/b2World.h>

#include "ConstantesCommunes.h"
#include "Object3D.h"
#include "World.h"

using namespace positionObjetsTable;

class World2017 : public World {

public:
	/**
	 * Construction du monde de la simulation spécifique à l'année 2016-2017
	 * @param gravity gravité à donner au moteur physique box2D
	 */
	World2017(IPhysicalContext* physics, IGraphicalContext* graphics);

	/**
	 * Obtient la taille de la table de jeu
	 */
	virtual Vector2m getSize();

	/**
	 * On crée tous les objets statiques (composant la table du Moon Village)
	 */
	void createTable();

	/**
	 * On crée tous les éléments de jeu dynamiques
	 * @param robot si 0 : pas de robot ; si 1 : robot bleu donc en enlève le bon module ; si 2 : robot jaune, idem
	 */
	void createAllObjects(Constantes::RobotColor color);

	/**
	 * Crée l'objet 3D du robot, à sa position de départ, suivant
	 * sa couleur.
	 * @param color La couleur de la zone de départ du robot
	 */
	Object3D& createRobot(Constantes::RobotColor color);

	/**
	 * Traduit l'enum en couleur 3f
	 * @param color BLUE, YELLOW ou MULTICOLOR
	 */
	Color3f getColor(ColorLunarModule color);

	/**
	 * Fonction de débug
	 */
	void createDebugObjects();
};


#endif // ROOT_WORLD2017_H
