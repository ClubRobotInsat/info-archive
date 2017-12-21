//
// Created by paul on 04/02/16.
//

#include "World.h"
#include "Simulateur.h"


World::World(IPhysicalContext* physics, IGraphicalContext* graphics)
        : _physicalCtx(physics), _graphicalCtx(graphics), _maxId(-1) {}

World::~World() {
	removeAllObject();
}


void World::update(Duration time) {
	_physicalCtx->update();

	for(auto& obj : _objectsList) {
		obj->update();
	}

	_graphicalCtx->update();
}

Object3D& World::createCube(const Vector3m& dimensions, const Vector3m& position, Mass mass, BodyType type, const Color3f& color) {
	IPhysicalInstance* physicProp = _physicalCtx->createCuboid(position, mass, type, dimensions);
	IGraphicalInstance* graphicProp = _graphicalCtx->createCuboid(position, dimensions);
	graphicProp->setColor(color);
	return createObject(graphicProp, physicProp, position);
}

Object3D& World::createCylinder(Length radius, Length height, const Vector3m& position, Mass mass, BodyType type, const Color3f& color) {
	IPhysicalInstance* physicProp = _physicalCtx->createCylinder(position, mass, type, radius, height);
	IGraphicalInstance* graphicProp = _graphicalCtx->createCylinder(position, radius, height);
	graphicProp->setColor(color);
	return createObject(graphicProp, physicProp, position);
}

Object3D& World::createSphere(Length radius, const Vector3m& position, Mass mass, BodyType type, const Color3f& color) {
	// Attention, il faudra changer les propriétés physiques si on passe sur un moteur en 3D !
	IPhysicalInstance* physicProp = _physicalCtx->createCylinder(position, mass, type, radius, 1_m);
	IGraphicalInstance* graphicProp = _graphicalCtx->createSphere(position, radius);
	graphicProp->setColor(color);
	return createObject(graphicProp, physicProp, position);
}

Object3D& World::createModel(const Vector3m& position, Mass mass, BodyType type, const std::string& model, const Color3f& color) {
	IPhysicalInstance* physicProp = _physicalCtx->createDefaultObject(position, type);
	// TODO Faire un objet physique adapté à la forme du robot ?
	IGraphicalInstance* graphicProp = _graphicalCtx->createModel(position, model);
	graphicProp->setColor(color);
	return createObject(graphicProp, physicProp, position);
}

void World::removeObject(const Object3D* object) {
	auto it = _objectsList.begin();

	for(; it != _objectsList.end(); it++) {
		if((*it).get() == object) {
			_physicalCtx->remove(&(*it)->getPhysics());
			_graphicalCtx->remove(&(*it)->getGraphics());
			_objectsList.erase(it);
			return;
		}
	}
}

void World::removeAllObject() {
	for(auto& object : _objectsList) {
		_physicalCtx->remove(&object->getPhysics());
		_graphicalCtx->remove(&object->getGraphics());
	}

	_objectsList.clear();
}

int World::nextId() {
	_maxId++;
	return _maxId;
}