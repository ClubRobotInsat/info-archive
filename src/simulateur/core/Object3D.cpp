//
// Created by paul on 04/02/16.
//

#include "Object3D.h"
#include "Simulateur.h"
#include "log/Log.h"


Object3D::Object3D(int id, IGraphicalInstance* graphics, IPhysicalInstance* physics, const Vector3m& position)
        : _id(id), _graphics(graphics), _physics(physics), _position(position) {}

Object3D::~Object3D() {}

void Object3D::update() {
	_physics->update(*this);
	_graphics->update(*this);
}

void Object3D::setPosition(const Vector3m& position) {
	_position = position;
}

void Object3D::setPosition(const Vector2m& position) {
	_position.x = position.x;
	_position.y = position.y;
}

void Object3D::setRotation(const Vector3ang& rotation) {
	_rotation = rotation;
}

void Object3D::addTag(const std::string& tag) {
	_tags.insert(tag);
}

bool Object3D::hasTag(const std::string& tag) {
	return _tags.find(tag) != _tags.end();
}
