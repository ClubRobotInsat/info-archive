//
// Created by louis on 07/12/15.
//

#include "PhysicalObjectDefinition.h"
#include <vector>

// ********************************************* //
//                     Shape                     //
// ********************************************* //
void PhysicalObjectDefinition::createShape() {
	_shape = std::make_unique<b2PolygonShape>();
}

void PhysicalObjectDefinition::setShapeCircle(Length radius) {
	_shape = std::make_unique<b2CircleShape>();
	_shape->m_radius = toSimulation(radius);
}

void PhysicalObjectDefinition::setShapeRectangle(Length width, Length height) {
	std::shared_ptr<b2PolygonShape> shape = std::make_shared<b2PolygonShape>();
	shape->SetAsBox(toSimulation(width) / 2, toSimulation(height) / 2);
	_shape = shape; // Le cast se fait tout seul
}

void PhysicalObjectDefinition::setShapePoints(const std::list<b2Vec2>& points) {
	std::shared_ptr<b2PolygonShape> shape = std::make_shared<b2PolygonShape>();
	const int size = (int)points.size();

	std::vector<b2Vec2> vecArray(size);
	int i = 0;
	for(b2Vec2 vec : points) {
		vecArray[i] = vec;
		i++;
	}
	shape->Set(&vecArray[0], size);
	_shape = shape; // Le cast se fait tout seul
}

// ********************************************* //
//                     Type                      //
// ********************************************* //
void PhysicalObjectDefinition::setType(Type type) {
	switch(type) {
		case STATIC:
			_type = b2_staticBody;
			break;
		case KINEMATIC:
			_type = b2_kinematicBody;
			break;
		case DYNAMIC:
			_type = b2_dynamicBody;
			break;
		default:
			throw std::string("PhysicalObjectDefinition:setType: invalid type");
	}
}