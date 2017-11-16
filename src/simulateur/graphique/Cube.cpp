//
// Created by benjamin on 18/02/16.
//

#include "Cube.h"
#include "../core/PhysicalToSimulationUnits.h"
#include "../core/World.h"


Cube::Cube(Length width, Length length, Vector3m position, Length sizeZ, Type type, Mass mass, World& world, std::string color) {
	_objectType = CUBE;

	_scale = {width, length, sizeZ};

	PhysicalObjectDefinition cubeDefinition;
	cubeDefinition.setShapeRectangle(_scale.x, _scale.y);
	cubeDefinition.setMass(mass);
	cubeDefinition.setType(type);
	_physicProperties =
	    std::make_unique<PhysicalObject>(cubeDefinition, world.getPhysicalWorld(), Vector2m{position.x, position.y});
	_modelName = "cube.dae";
	_zPosition = position.z;
	_color = color;
}

Cube::Cube(Length width, Length length, Length x, Length y, Length z, Length sizeZ, Type type, Mass mass, World& world, std::string color)
        : Cube(width, length, Vector3m(x, y, z), sizeZ, type, mass, world, color) {}


JSON Cube::getCreationMessage() {
	JSON message;
	message["type"] = "newcuboid";
	message["id"] = _ID;
	message["matrix"] = toJSON(generateTransformationMatrix());
	message["color"] = _color;
	return message;
}
