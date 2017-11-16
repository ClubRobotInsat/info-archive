//
// Created by paul on 18/02/16.
//

#include "BeachCabin.h"
#include "../core/SimulateurConstantes.h"
#include "../core/World.h"


BeachCabin::BeachCabin(Vector3m position, World& world, std::string color) {

	_objectType = BEACHCABIN;
	_scale = BEACHCABIN_DIMENSIONS;

	PhysicalObjectDefinition doorDefinition;
	doorDefinition.setShapeRectangle(_scale.x, _scale.y);
	doorDefinition.setType(b2_staticBody);
	_physicProperties =
	    std::make_unique<PhysicalObject>(doorDefinition, world.getPhysicalWorld(), Vector2m{position.x, position.y});
	_modelName = "beachCabin.dae";
	_zPosition = position.z;
	_color = color;
}

BeachCabin::BeachCabin(Length x, Length y, Length z, World& world, std::string color)
        : BeachCabin(Vector3m(x, y, z), world, color) {}

JSON BeachCabin::getCreationMessage() {
	JSON message;
	message["type"] = "newcuboid";
	message["id"] = _ID;
	message["matrix"] = toJSON(generateTransformationMatrix());
	message["color"] = _color;
	return message;
}
