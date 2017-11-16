//
// Created by paul on 04/02/16.
//

#include "FormeRobot.h"
#include "../../core/SimulateurConstantes.h"
#include "RobotPrincipal/Constantes.h"
#include <Box2D/Common/b2Math.h>

FormeRobot::FormeRobot(World& world, Vector3m position, ObjectType typeRobot, Constantes::RobotColor color) {
	_objectType = typeRobot;
	_scale = {0.8_mm, 0.8_mm, 1.2_mm}; // valeurs qui correspondent à un affichage cohérent sur le simu
	PhysicalObjectDefinition robotDefinition;
	robotDefinition.setType(DYNAMIC);
	robotDefinition.setShapeRectangle(38_cm, 30_cm); // vraie taille du robot
	robotDefinition.setMass(mass::HEAVY);
	_physicProperties = std::make_unique<PhysicalObject>(robotDefinition, world.getPhysicalWorld(), toVec2(position));
	_zPosition = position.z;
	_modelName = "robot";
	if(color == Constantes::RobotColor::Yellow)
		_color = YELLOW;
	else
		_color = BLUE;
	_offsetAngle = {90_deg, 90_deg, 0_deg};
}

// Implémentation de IDeplacement, c'est des méthodes déjà implémentée dans Object3D
// Angle IDeplacement::getAngle() const {return}
void FormeRobot::setAngularVelocity(AngularSpeed speed) {
	_physicProperties->setAngularVelocity(speed);
}

void FormeRobot::setLinearVelocity(Speed speed) {
	_physicProperties->setLinearVelocity(speed);
}

Vector2m FormeRobot::getPosition() {
	return _physicProperties->getPosition();
}

Angle FormeRobot::getAngle() {
	return _physicProperties->getAngle();
}

Speed FormeRobot::getLinearVelocity() const {
	return _physicProperties->getLinearVelocity();
}

AngularSpeed FormeRobot::getAngularVelocity() {
	return _physicProperties->getAngularVelocity();
}

JSON FormeRobot::getCreationMessage() {

	JSON message;
	message["type"] = "newmodel";
	message["modelName"] = "robot";
	message["id"] = _ID;
	message["matrix"] = toJSON(generateTransformationMatrix());
	message["color"] = _color;

	return message;
}

Matrix4f FormeRobot::generateTransformationMatrix() {
	Matrix4f matIdentity;
	return matIdentity * Matrix4f::translationMatrix(toSimulation(
	                         {_physicProperties->getPosition().x, _physicProperties->getPosition().y, _zPosition})) *
	       Matrix4f::rotationMatrix({1, 0, 0}, _offsetAngle.x.toDeg()) *
	       Matrix4f::rotationMatrix({0, 1, 0}, _physicProperties->getAngle().toDeg() + _offsetAngle.y.toDeg()) *
	       Matrix4f::rotationMatrix({0, 0, 1}, _offsetAngle.z.toDeg()) * Matrix4f::scaleMatrix(toSimulation(_scale));
}