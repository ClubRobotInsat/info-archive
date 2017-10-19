//
// Created by benji on 07/10/16.
//

#ifndef ROOT_WORLD2017_H
#define ROOT_WORLD2017_H

#include "../../commun/ConstantesCommunes.h"
#include "../3DObject/Cube.h"
#include "../3DObject/Object3D.h"
#include "../server/SimuWebSocketServer.h"
#include "World.h"
#include <Box2D/Dynamics/b2World.h>

using namespace positionObjetsTable;

class World2017 : public World {

public:
	/**
	 * Construction du monde de la simulation spécifique à l'année 2016-2017
	 * @param gravity gravité à donner au moteur physique box2D
	 */
	World2017(b2Vec2 gravity);

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
	void createAllObjects(int robot);

	/**
	 * Retourne le code xCode (std::string) de la couleur du cylindre
	 * @param color BLUE, YELLOW ou MULTICOLOR
	 */
	std::string getColor(ColorLunarModule color);

	/**
	 * Fonction de débug
	 */
	void createDebugObjects();
};


#endif // ROOT_WORLD2017_H
