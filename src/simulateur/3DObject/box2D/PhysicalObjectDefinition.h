//
// Created by louis on 07/12/15.
//

#ifndef TESTBOX2D_PHYSICALOBJECTDEFINITION_H
#define TESTBOX2D_PHYSICALOBJECTDEFINITION_H

#include "../../PhysicalToSimulationUnits.h"
#include <Box2D/Collision/Shapes/b2CircleShape.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Common/b2Math.h>
#include <Box2D/Dynamics/b2Body.h>
#include <Units.h>
#include <iostream>
#include <list>
#include <memory>
#include <vector>

namespace mass {
	const Mass LIGHT = 100_g;
	const Mass MEDIUM = 1_kg;
	const Mass HEAVY = 10000_kg;
}

enum Type { STATIC, KINEMATIC, DYNAMIC };


class PhysicalObjectDefinition {
public:
	/**
	 * Crée la définition physique de l'objet à simuler en lui attribuant une forme et body type par box2D
	 * @param mass
	 * @param type objet statique ou dynamique (on lui attribue des mouvements possibles ou non?)
	 */
	PhysicalObjectDefinition(Mass mass = mass::LIGHT, const b2BodyType type = b2_dynamicBody)
	        : _mass(mass), _type(type) {

		createShape();
	}

	/**
	 * Destruction de la définition physique de l'objet
	 */
	virtual ~PhysicalObjectDefinition() = default;


	/**
	 * Modifie la masse attribuée à l'objet
	 * @param mass nouvelle masse
	 */
	void setMass(Mass mass) {
		_mass = mass;
	}

	/**
	 * Obtient la masse de l'objet
	 */
	Mass getMass() {
		return _mass;
	}

	/**
	 * Crée une shape qui devient la shape active
	 * Les autres shapes sont conservées et seront ajoutées à l'objet 2D, qui sera ainsi un objet composé
	 */
	void createShape();

	/**
	 * Modifie la shape en un cercle
	 * @param radius nouveau rayon
	 */
	void setShapeCircle(Length radius);

	/**
	 * Modifie la shape active en un rectangle
	 * @param width nouvelle largeur
	 * @param height nouvelle hauteur
	 */
	void setShapeRectangle(Length width, Length height);

	/**
	 * Modifie la shape active en un polygone
	 * @param points liste des sommets du polygone
	 */
	void setShapePoints(const std::list<b2Vec2>& points);

	/**
	 * Obtient la shape active
	 */
	std::shared_ptr<b2Shape> getShape() {
		return _shape;
	}

	/**
	 * Modifie le type de l'objet selon l'énumération de la classe
	 * @param type dynamic ou static
	 */
	void setType(Type type);

	/**
	 * Obtient le type de l'objet selon l'énumération de la classe
	 */
	void setType(b2BodyType type) {
		_type = type;
	}

	/**
	 * Obtient le type de l'objet selon le type box2D
	 */
	const b2BodyType getType() {
		return _type;
	}

private:
	/// masse de l'objet
	Mass _mass;
	/// type de l'objet de box2D
	b2BodyType _type;

	/// shape active
	std::shared_ptr<b2Shape> _shape;
};


#endif // TESTBOX2D_PHYSICALOBJECTDEFINITION_H
