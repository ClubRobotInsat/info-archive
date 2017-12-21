//
// Created by paul on 23/02/17.
//

#include "Object.h"

Object::Object(int id, irr::scene::ISceneNode* ptr) : _id(id), _objectPtr(ptr) {}

irr::scene::ISceneNode Object::getInternalPtr() const {
	return _objectPtr;
}

void Object::ObjectSetColor(irr::scene::ISceneNode* Target, irr::u32 a, irr::u32 r, irr::u32 g, irr::u32 b) {
	irr::video::SMaterial& material_object = Target->getMaterial(42);
	material_object.AmbientColor.set(a, r, g, b);
	material_object.SpecularColor.set(a, r, g, b);
	material_object.DiffuseColor.set(a, r, g, b);
};

virtual int Object::getId() {
	return _id;
}

/*
virtual void Object::setColor(const Color3f& color) {
   ObjectSetColor(getInternalPtr(), )
}
 */