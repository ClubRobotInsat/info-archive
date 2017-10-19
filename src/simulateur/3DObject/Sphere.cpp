//
// Created by benji on 07/10/16.
//

#include "Sphere.h"
#include "../world/World.h"


Sphere::Sphere(Length radius, Vector3m position, Type type, Mass mass, World& world, std::string color) {
	_objectType = SPHERE;

	_radius = radius;
	PhysicalObjectDefinition sphereDefinition;
	sphereDefinition.setShapeCircle(_radius);
	sphereDefinition.setMass(mass);
	sphereDefinition.setType(type);
	_modelName = "sphere.dae";
	_physicProperties =
	    std::make_unique<PhysicalObject>(sphereDefinition, world.getPhysicalWorld(), Vector2m{position.x, position.y});
	_zPosition = position.z;
	_scale = {unitarySize, unitarySize, unitarySize};
	_color = color;
}

Sphere::Sphere(Length radius, Length x, Length y, Length z, Type type, Mass mass, World& world, std::string color)
        : Sphere(radius, Vector3m(x, y, z), type, mass, world, color) {}

JSON Sphere::getCreationMessage() {

	// Partie qui envoie au serveur un nouvel objet
	JSON message;
	message["type"] = "newsphere";
	message["id"] = _ID;
	message["radius"] = toSimulation(_radius);
	message["color"] = _color;
	return message;
}