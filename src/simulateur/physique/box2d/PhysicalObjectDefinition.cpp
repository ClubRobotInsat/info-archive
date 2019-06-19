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
	_shape->m_radius = static_cast<float32>(toBox2D(radius));
}

void PhysicalObjectDefinition::setShapeRectangle(Length width, Length height) {
	std::shared_ptr<b2PolygonShape> shape = std::make_shared<b2PolygonShape>();
	shape->SetAsBox(static_cast<float32>(toBox2D(width) / 2), static_cast<float32>(toBox2D(height) / 2));
	_shape = shape; // Le cast se fait tout seul
}

void PhysicalObjectDefinition::setShapePoints(const std::list<b2Vec2>& points) {
	std::shared_ptr<b2PolygonShape> shape = std::make_shared<b2PolygonShape>();
	const int size = static_cast<int>(points.size());

	std::vector<b2Vec2> vecArray(static_cast<unsigned long>(size));
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
void PhysicalObjectDefinition::setType(BodyType type) {
	switch(type) {
		case STATIC_BODY:
			_type = b2_staticBody;
			break;
		case DYNAMIC_BODY:
			_type = b2_dynamicBody;
			break;
		/*case KINEMATIC_BODY:
			_type = b2_kinematicBody;
			break;*/
		default:
			throw std::string("PhysicalObjectDefinition:setType: invalid type");
	}
}