//
// Created by paul on 23/02/17.
//

#include "Object.h"

#include "../../core/Object3D.h"

Object::Object(int id, irr::scene::ISceneNode* ptr) : _id(id), _objectPtr(ptr) {
	// Changement du materiau pour qu'il accepte la couleur diffuse
	ptr->getMaterial(0).ColorMaterial = irr::video::ECM_NONE;
}

irr::scene::ISceneNode* Object::getInternalPtr() const {
	return _objectPtr;
}

void Object::ObjectSetColor(irr::scene::ISceneNode* Target, irr::u32 a, irr::u32 r, irr::u32 g, irr::u32 b) {
	irr::video::SMaterial& material_object = Target->getMaterial(42);
	material_object.AmbientColor.set(a, r, g, b);
	material_object.SpecularColor.set(a, r, g, b);
	material_object.DiffuseColor.set(a, r, g, b);
};

int Object::getId() const {
	return _id;
}


void Object::setScale(const Vector3f& scale) {
	_objectPtr->setScale(SimulationToIrrlicht::VectorIrr(scale));
}
Color3f Object::getColor() {
	irr::video::SMaterial& material_object = _objectPtr->getMaterial(0);
	float r = material_object.DiffuseColor.getRed() / 255.f;
	float b = material_object.DiffuseColor.getBlue() / 255.f;
	float g = material_object.DiffuseColor.getGreen() / 255.f;

	return Color3f(r, g, b);
}

void Object::setColor(const Color3f& color) {
	irr::video::SMaterial& material_object = _objectPtr->getMaterial(0);
	material_object.DiffuseColor.setBlue(irr::u32(color.b * 255));
	material_object.DiffuseColor.setRed(irr::u32(color.r * 255));
	material_object.DiffuseColor.setGreen(irr::u32(color.g * 255));
}

void Object::update(Object3D& parent) {
	_objectPtr->setPosition(SimulationToIrrlicht::VectorIrr(parent.getPosition()));
	_objectPtr->setRotation(SimulationToIrrlicht::VectorAngIrr(parent.getRotation()));
}


/*
virtual void Object::setColor(const Color3f& color) {
   ObjectSetColor(getInternalPtr(), )
}
 */