//
// Created by paul on 04/02/16.
//

#include "World.h"
#include "../Simulateur.h"
#include "../robot/Robot2017.h"


World::World(b2Vec2 gravity) : _physicWorld(gravity) {}

World::~World() {
	clearMessageList();
	removeAllObject();
}


void World::update(Duration time) {
	_physicWorld.Step(time.toS(),
	                  8,
	                  2); // En secondes c'est très bien en fait : si on augmente le timestep trop haut, le PID déconne.
	// Ajout des messages de mise à jour pour le serveur
	for(std::unique_ptr<Object3D>& object : _objectList) {
		auto message = object->getUpdateMessage();
		if(!message.empty())
			_messageList.append(message);
	}
}

void World::removeObject(const Object3D* object) {
	auto it = _objectList.begin();

	for(; it != _objectList.end(); it++) {
		if((*it).get() == object) {
			auto message = (*it)->getRemoveMessage();
			if(!message.empty())
				_messageList.append(message);
			_objectList.erase(it);
			return;
		}
	}
}

void World::removeAllObject() {
	for(std::unique_ptr<Object3D> const& object : _objectList) {
		auto message = object->getRemoveMessage();
		if(!message.empty())
			_messageList.append(message);
	}

	_objectList.clear();
}

void World::sendWorldState() const {
	JSON list;

	for(std::unique_ptr<Object3D> const& object : _objectList) {
		list.append(object->getCreationMessage());
	}

	Json::FastWriter writer;
	writer.omitEndingLineFeed();

	std::string s = writer.write(list);
	Simulateur::getInstance().getServer().broadcast(s);
}

void World::clearMessageList() {
	_messageList.clear();
}

Cube& World::createCube(Length sizeX, Length sizeY, Length sizeZ, Vector3m position, Mass mass, Type type, std::string color) {
	return createObject<Cube>(sizeX, sizeY, position, sizeZ, type, mass, *this, color);
}

Cube& World::createCube(Vector3m size, Vector3m position, Mass mass, Type type, std::string color) {
	return createObject<Cube>(size.x, size.y, position, size.z, type, mass, *this, color);
}

Cylinder& World::createCylinder(Length radius, Length size_Z, Vector3m position, Mass mass, Type type, std::string color) {
	return createObject<Cylinder>(radius, size_Z, position, type, mass, *this, color);
}

Sphere& World::createSphere(Length radius, Vector3m position, Mass mass, Type type, std::string color) {
	return createObject<Sphere>(radius, position, type, mass, *this, color);
}

// acquisition de la liste des ID des différents objets
std::vector<int> World::getListId(Object3D::ObjectType type) {
	std::vector<int> listId;

	for(std::unique_ptr<Object3D>& object : _objectList) {
		if(object->getObjectType() == type)
			listId.push_back(object->getId());
	}

	return listId;
}

void World::enableSimulation(bool enabled) {
	for(std::unique_ptr<Object3D> const& object : _objectList) {
		object->enableSimulation(enabled);
	}
}
