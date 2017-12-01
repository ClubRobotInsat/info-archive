//
// Created by louis on 30/11/17.
//

#include "Box2DPhysicalContext.h"

#include "PhysicalObject.h"
#include "PhysicalObjectDefinition.h"

Box2DPhysicalContext::Box2DPhysicalContext(const b2Vec2& gravity)
        : _b2world(gravity), _maxId(-1), _lastUpdateTime(TimePoint::now()) {}

b2World& Box2DPhysicalContext::getWorld() {
	return _b2world;
}

void Box2DPhysicalContext::update() {
	Duration elapsed = TimePoint::now() - _lastUpdateTime;
	Duration step = 20_ms; // TODO 20_ms à mettre en champ de classe

	while(elapsed > step) {
		elapsed -= step;
		_lastUpdateTime += step;

		_b2world.Step(step.toS(), 8, 2);
	}
}

// Cette méthode pourra disparaître si on fusionne BodyType et Type
Type conversion(BodyType type) {
	return type == STATIC_BODY ? STATIC : DYNAMIC;
}

IPhysicalInstance* Box2DPhysicalContext::createDefaultObject(const Vector3m& position, BodyType type) {
	// TODO changer l'objet par défaut

	PhysicalObjectDefinition def;
	def.setType(conversion(type));
	return addObject(def, this, nextId(), position.x, position.y);
}

IPhysicalInstance* Box2DPhysicalContext::createCuboid(const Vector3m& position, Mass mass, BodyType type, const Vector3m& dimensions) {
	PhysicalObjectDefinition cubeDefinition;
	cubeDefinition.setShapeRectangle(dimensions.x, dimensions.y);
	cubeDefinition.setMass(mass);
	cubeDefinition.setType(conversion(type));
	return addObject(cubeDefinition, this, nextId(), position.x, position.y);
}

IPhysicalInstance*
    Box2DPhysicalContext::createCylinder(const Vector3m& position, Mass mass, BodyType type, Length radius, Length height) {
	PhysicalObjectDefinition cylinderDefinition;
	cylinderDefinition.setShapeCircle(radius);
	cylinderDefinition.setMass(mass);
	cylinderDefinition.setType(conversion(type));
	return addObject(cylinderDefinition, this, nextId(), position.x, position.y);
}

void Box2DPhysicalContext::remove(IPhysicalInstance* object) {
	for(auto it = _objects.begin(); it == _objects.end(); it++) {
		if((*it)->getId() == object->getId()) {
			_objects.erase(it);
			return;
		}
	}
}

int Box2DPhysicalContext::nextId() {
	_maxId++;
	return _maxId;
}