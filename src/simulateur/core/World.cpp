//
// Created by paul on 04/02/16.
//

#include "World.h"
#include "Simulateur.h"


World::World(IPhysicalContext* physics, IGraphicalContext* graphics)
        : _maxId(-1), _physics(physics), _graphics(graphics) {}

World::~World() {
	removeAllObject();
}


void World::update(Duration time) {
	_physics->update();

	for(auto& obj : _objectsList) {
		obj->update();
	}

	_graphics->update();
}

Object3D& World::createCube(const Vector3m& dimensions, const Vector3m& position, Mass mass, BodyType type, const Vector3f& color) {
	IPhysicalInstance* physicProp = _physics->createCuboid(position, mass, type, dimensions);
	IGraphicalInstance* graphicProp = _graphics->createCuboid(position, dimensions);
	graphicProp->setColor(color);
	return createObject(graphicProp, physicProp, position);
}

Object3D& World::createCylinder(Length radius, Length height, const Vector3m& position, Mass mass, BodyType type, const Vector3f& color) {
	IPhysicalInstance* physicProp = _physics->createCylinder(position, mass, type, radius, height);
	IGraphicalInstance* graphicProp = _graphics->createCylinder(position, radius, height);
	graphicProp->setColor(color);
	return createObject(graphicProp, physicProp, position);
}

Object3D& World::createSphere(Length radius, const Vector3m& position, Mass mass, BodyType type, const Vector3f& color) {
	// Attention, il faudra changer les propriétés physiques si on passe sur un moteur en 3D !
	IPhysicalInstance* physicProp = _physics->createCylinder(position, mass, type, radius, 1_m);
	IGraphicalInstance* graphicProp = _graphics->createSphere(position, radius);
	graphicProp->setColor(color);
	return createObject(graphicProp, physicProp, position);
}

Object3D& World::createModel(const Vector3m& position, Mass mass, BodyType type, const std::string& model, const Vector3f& color) {
	IPhysicalInstance* physicProp = _physics->createDefaultObject(position, type);
	IGraphicalInstance* graphicProp = _graphics->createModel(position, model);
	graphicProp->setColor(color);
	return createObject(graphicProp, physicProp, position);
}

void World::removeObject(const Object3D* object) {
	auto it = _objectsList.begin();

	for(; it != _objectsList.end(); it++) {
		if((*it).get() == object) {
			_physics->remove(&(*it)->getPhysics());
			_graphics->remove(&(*it)->getGraphics());
			_objectsList.erase(it);
			return;
		}
	}
}

void World::removeAllObject() {
	for(auto& object : _objectsList) {
		_physics->remove(&object->getPhysics());
		_graphics->remove(&object->getGraphics());
	}

	_objectsList.clear();
}

int World::nextId() {
	_maxId++;
	return _maxId;
}