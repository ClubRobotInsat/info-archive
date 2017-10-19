//
// Created by benji on 07/10/16.
//

#include "World2017.h"

#include "../3DObject/Cube.h"
#include "../3DObject/Cylinder.h"
#include "../3DObject/Sphere.h"
#include "../Simulateur.h"
#include "../SimulateurConstantes.h"
#include "../robot/FormeRobot.h"

using namespace ConstantesSimu2017;

World2017::World2017(b2Vec2 gravity) : World(gravity) {}


Vector2m World2017::getSize() {
	return Vector2m(WALLS_DIMENSIONS.x, WALLS_DIMENSIONS.y);
}

void World2017::createTable() {
	std::vector<Cube*> listTableObjects;

	/*******
	 * Sol *
	 *******/

	CubeData solData;
	// sol du bas de plus grande surface
	solData.setPosition(Vector3m(0_m, 0_m, -2_cm));
	solData.setSize(Vector3m(getSize().x, getSize().y - 36_cm, 2_cm));
	listTableObjects.push_back(&this->createCube(solData.getSize(), solData.getPosition(), mass::LIGHT, STATIC, COLOR_SOL));
	listTableObjects[listTableObjects.size() - 1]->enableSimulation(false);

	// sol entre les deux zones de départ
	solData.setPosition(Vector3m(107_cm, 2_m - 36_cm, -2_cm));
	solData.setSize(Vector3m(getSize().x - 2 * 107_cm, 36_cm, 2_cm));
	listTableObjects.push_back(&this->createCube(solData.getSize(), solData.getPosition(), mass::LIGHT, STATIC, COLOR_SOL));
	listTableObjects[listTableObjects.size() - 1]->enableSimulation(false);


	CubeData startingArea;
	// zone de départ à gauche, équipe bleue
	startingArea.setPosition(Vector3m(0_m, 2_m - 36_cm, -2_cm));
	startingArea.setSize(Vector3m(107_cm, 36_cm, 2_cm));
	listTableObjects.push_back(
	    &this->createCube(startingArea.getSize(), startingArea.getPosition(), mass::LIGHT, STATIC, COLOR_TEAM_BLUE));
	listTableObjects[listTableObjects.size() - 1]->enableSimulation(false);

	// zone de départ à droite, équipe jaune
	startingArea.setPosition(Vector3m(3_m - 107_cm, 2_m - 36_cm, -2_cm));
	listTableObjects.push_back(
	    &this->createCube(startingArea.getSize(), startingArea.getPosition(), mass::LIGHT, STATIC, COLOR_TEAM_YELLOW));
	listTableObjects[listTableObjects.size() - 1]->enableSimulation(false);


	/********************
	 * Murs de la table *
	 ********************/

	// murs horizontaux gris
	CubeData wall;
	wall.setSize(Vector3m(WALLS_DIMENSIONS.x - 107_cm * 2, WALLS_SIZE, WALLS_DIMENSIONS.z));
	wall.setPosition(Vector3m(107_cm, WALLS_DIMENSIONS.y, 0_m));
	listTableObjects.push_back(&this->createCube(wall.getSize(), wall.getPosition(), mass::HEAVY, STATIC, COLOR_WALL)); // haut

	wall.setSize(Vector3m(WALLS_DIMENSIONS.x + 2 * WALLS_SIZE, WALLS_SIZE, WALLS_DIMENSIONS.z));
	wall.setPosition(Vector3m(-1 * WALLS_SIZE, WALLS_SIZE * -1, 0_m));
	listTableObjects.push_back(&this->createCube(wall.getSize(), wall.getPosition(), mass::HEAVY, STATIC, COLOR_WALL)); // bas

	// murs horizontaux colorés
	wall.setSize(Vector3m(107_cm + WALLS_SIZE, WALLS_SIZE, WALLS_DIMENSIONS.z));
	wall.setPosition(Vector3m(WALLS_SIZE * -1, WALLS_DIMENSIONS.y, 0_m));
	listTableObjects.push_back(&this->createCube(wall.getSize(), wall.getPosition(), mass::HEAVY, STATIC, COLOR_TEAM_BLUE));

	wall.setPosition(Vector3m(1.93_m, WALLS_DIMENSIONS.y, 0_m));
	listTableObjects.push_back(&this->createCube(wall.getSize(), wall.getPosition(), mass::HEAVY, STATIC, COLOR_TEAM_YELLOW));

	// murs verticaux gris
	wall.setSize(Vector3m(WALLS_SIZE, WALLS_DIMENSIONS.y - 382_mm, WALLS_DIMENSIONS.z));
	wall.setPosition(Vector3m(WALLS_SIZE * -1, 0_m, 0_m));
	listTableObjects.push_back(&this->createCube(wall.getSize(), wall.getPosition(), mass::HEAVY, STATIC, COLOR_WALL)); // gauche

	wall.setPosition(Vector3m(WALLS_DIMENSIONS.x, 0_m, 0_m));
	listTableObjects.push_back(&this->createCube(wall.getSize(), wall.getPosition(), mass::HEAVY, STATIC, COLOR_WALL)); // droite

	// murs verticaux colorés
	wall.setSize(Vector3m(WALLS_SIZE, 382_mm + WALLS_SIZE, WALLS_DIMENSIONS.z));
	wall.setPosition(Vector3m(WALLS_SIZE * -1, WALLS_DIMENSIONS.y - 382_mm, 0_m));
	listTableObjects.push_back(&this->createCube(wall.getSize(), wall.getPosition(), mass::HEAVY, STATIC, COLOR_TEAM_BLUE));

	wall.setPosition(Vector3m(WALLS_DIMENSIONS.x, WALLS_DIMENSIONS.y - 382_mm, 0_m));
	listTableObjects.push_back(&this->createCube(wall.getSize(), wall.getPosition(), mass::HEAVY, STATIC, COLOR_TEAM_YELLOW));


	/***************************
	 * Murs colorés de la base *
	 ***************************/

	wall.setSize(Vector3m(71_cm, 22_mm, 3_cm));
	wall.setPosition(Vector3m(0_m, WALLS_DIMENSIONS.y - 382_mm, 0_m));
	listTableObjects.push_back(
	    &this->createCube(wall.getSize(), wall.getPosition(), mass::HEAVY, STATIC, BLUE)); // mur base gauche

	wall.setPosition(Vector3m(2.29_m, WALLS_DIMENSIONS.y - 382_mm, 0_m));
	listTableObjects.push_back(&this->createCube(wall.getSize(), wall.getPosition(), mass::HEAVY, STATIC, YELLOW)); // mur base droite


	/*********
	 * Soute *
	 *********/

	CubeData hold;
	hold.setSize(Vector3m(30_cm, 22.2_cm, 35_cm));
	hold.setPosition(Vector3m(5_cm, WALLS_DIMENSIONS.y, 0_cm));
	listTableObjects.push_back(&this->createCube(hold.getSize(), hold.getPosition(), mass::HEAVY, STATIC, COLOR_HOLD));

	hold.setPosition(Vector3m(2.65_m, WALLS_DIMENSIONS.y, 0_cm));
	listTableObjects.push_back(&this->createCube(hold.getSize(), hold.getPosition(), mass::HEAVY, STATIC, COLOR_HOLD));


	/***********************************
	 * Construction de la base lunaire *
	 ***********************************/

	// Zone centrale
	for(int i = 0; i < LIST_CENTRAL.size(); i++) {
		listTableObjects.push_back(
		    &this->createCube(Vector3m(3_cm, 60_cm, 3_cm), LIST_CENTRAL[i].getPos3D(REFERENCE_SIMULATOR), mass::HEAVY, STATIC, COLOR_LUNAR_BUILDINGS));
		listTableObjects[listTableObjects.size() - 1]->setAngle(LIST_CENTRAL[i].getAngle(REFERENCE_SIMULATOR));
	}
	Cylinder& centralCylinder =
	    this->createCylinder(20_cm, 3_cm, Vector3m(1.5_m, 0_m, 0_m), mass::HEAVY, STATIC, COLOR_LUNAR_BUILDINGS);

	// Zones latérales
	CubeData stick;
	stick.setSize(Vector3m(8_cm, 22_mm, 22_mm));
	for(int i = 0; i < LIST_LATERAL_H.size(); i++) {
		stick.setPosition(LIST_LATERAL_H[i].getPos3D(REFERENCE_SIMULATOR));
		listTableObjects.push_back(&this->createCube(stick.getSize(), stick.getPosition(), mass::HEAVY, STATIC, COLOR_LUNAR_BUILDINGS));
	}

	stick.setSize(Vector3m(3_cm, 45_cm, 3_cm));
	for(int i = 0; i < LIST_LATERAL_V.size(); i++) {
		stick.setPosition(LIST_LATERAL_V[i].getPos3D(REFERENCE_SIMULATOR));
		listTableObjects.push_back(&this->createCube(stick.getSize(), stick.getPosition(), mass::HEAVY, STATIC, COLOR_LUNAR_BUILDINGS));
	}

	/**********************************************************
	 * Placement des pavés qui délimitent les zones de billes *
	 **********************************************************/

	stick.setSize(Vector3m(10_cm, 3_cm, 4_mm));
	for(int i = 0; i < LIST_DELIMITERS.size(); i++) {
		stick.setPosition(LIST_DELIMITERS[i].getPos3D(REFERENCE_SIMULATOR));
		listTableObjects.push_back(&this->createCube(stick.getSize(), stick.getPosition(), mass::HEAVY, STATIC, COLOR_LUNAR_BUILDINGS));
		listTableObjects[listTableObjects.size() - 1]->setAngle(LIST_DELIMITERS[i].getAngle(REFERENCE_SIMULATOR));
	}

	/*
	 * les objets créés appartiennent à la table : on change leur type objet
	 */
	for(int i = 0; i < listTableObjects.size(); i++) {
		listTableObjects[i]->setObjectType(Object3D::ObjectType::TABLE);
	}
	centralCylinder.setObjectType(Object3D::ObjectType::TABLE);

	listTableObjects.clear();
}

std::string World2017::getColor(ColorLunarModule color) {
	switch(color) {
		case ColorLunarModule::CYLINDER_BLUE:
			return COLOR_CYLINDER_BLUE;
		case ColorLunarModule::CYLINDER_YELLOW:
			return COLOR_CYLINDER_YELLOW;
		case ColorLunarModule::CYLINDER_MULTICOLOR:
			return COLOR_CYLINDER_MULTICOLOR;
	}
	return COLOR_CYLINDER_MULTICOLOR;
}

void World2017::createAllObjects(int robot) {
	for(int i = 0; i < LIST_LUNAR_MODULES.size(); i++) {
		this->createCylinder(63_mm,
		                     10_cm,
		                     LIST_LUNAR_MODULES[i].coords.getPos3D(REFERENCE_SIMULATOR),
		                     mass::LIGHT,
		                     DYNAMIC,
		                     getColor(LIST_LUNAR_MODULES[i].color));
	}

	if(robot == 1) {
		this->createCylinder(63_mm, 10_cm, Vector3m(95_cm, 1.8_m, 5_cm), mass::LIGHT, DYNAMIC, COLOR_CYLINDER_BLUE);
	} else if(robot == 2) {
		this->createCylinder(63_mm, 10_cm, Vector3m(2.05_m, 1.8_m, 5_cm), mass::LIGHT, DYNAMIC, COLOR_CYLINDER_YELLOW);
	} else {
		this->createCylinder(63_mm, 10_cm, Vector3m(95_cm, 1.8_m, 5_cm), mass::LIGHT, DYNAMIC, COLOR_CYLINDER_BLUE);
		this->createCylinder(63_mm, 10_cm, Vector3m(2.05_m, 1.8_m, 5_cm), mass::LIGHT, DYNAMIC, COLOR_CYLINDER_YELLOW);
	}
}

void World2017::createDebugObjects() {}