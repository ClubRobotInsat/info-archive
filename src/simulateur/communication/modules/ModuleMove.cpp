//
// Created by scriptopathe on 13/03/16.
//

#include "ModuleMove.h"
#include "log/Log.h"

ModuleMove::ModuleMove(FormeRobot* obj) : _obj(obj) {}

void ModuleMove::setPhysicalObject(FormeRobot* obj) {
	_obj = obj;
}
// ----------------------------------
// Implementation de IDeplacement
// ----------------------------------
Vector2m ModuleMove::getPosition() {
	return this->_obj->getPosition();
}

Speed ModuleMove::getLinearVelocity() const {
	return this->_obj->getLinearVelocity();
}

Angle ModuleMove::getAngle() {
	return this->_obj->getAngle();
}

AngularSpeed ModuleMove::getAngularVelocity() {
	return this->_obj->getAngularVelocity();
}

void ModuleMove::setLinearVelocity(Speed speed) {
	return this->_obj->setLinearVelocity(speed);
}

void ModuleMove::setAngularVelocity(AngularSpeed speed) {
	return this->_obj->setAngularVelocity(speed);
}