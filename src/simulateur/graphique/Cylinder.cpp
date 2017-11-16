//
// Created by paul on 04/02/16.
//

#include "Cylinder.h"
#include "../core/World.h"


Cylinder::Cylinder(Length radius, Length sizeZ, Vector3m position, Type type, Mass mass, World& world, std::string color) {
	_objectType = CYLINDER;

	_radius = radius;
	PhysicalObjectDefinition cylinderDefinition;
	cylinderDefinition.setShapeCircle(_radius);
	cylinderDefinition.setMass(mass);
	cylinderDefinition.setType(type);
	_modelName = "cylindre.dae";
	_physicProperties =
	    std::make_unique<PhysicalObject>(cylinderDefinition, world.getPhysicalWorld(), Vector2m{position.x, position.y});
	_zPosition = position.z;
	_scale = {unitarySize, unitarySize, unitarySize};
	_height = sizeZ;
	_color = color;
	_offsetAngle = {90_deg, 0_deg, 0_deg};
}

Cylinder::Cylinder(Length radius, Length sizeZ, Length x, Length y, Length z, Type type, Mass mass, World& world, std::string color)
        : Cylinder(radius, sizeZ, Vector3m(x, y, z), type, mass, world, color) {}

JSON Cylinder::getCreationMessage() {

	// Partie qui envoie au serveur un nouvel objet
	JSON message;
	message["type"] = "newcylinder";
	message["id"] = _ID;
	message["matrix"] = toJSON(generateTransformationMatrix());
	message["radius"] = toSimulation(_radius);
	message["height"] = toSimulation(_height);
	message["color"] = _color;
	return message;
}

Matrix4f Cylinder::generateTransformationMatrix() {
	Matrix4f matIdentity;
	return matIdentity * Matrix4f::translationMatrix(toSimulation(
	                         {_physicProperties->getPosition().x, _physicProperties->getPosition().y, _zPosition})) *
	       Matrix4f::rotationMatrix({1, 0, 0}, _offsetAngle.x.toDeg()) *
	       Matrix4f::rotationMatrix({0, 1, 0}, _physicProperties->getAngle().toDeg() + _offsetAngle.y.toDeg()) *
	       Matrix4f::rotationMatrix({0, 0, 1}, _offsetAngle.z.toDeg()) * Matrix4f::scaleMatrix(toSimulation(_scale));
}