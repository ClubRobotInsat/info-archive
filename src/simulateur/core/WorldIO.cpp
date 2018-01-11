#include "World.h"

void World::loadJSON(const JSON& json) {
	const JSON& objects = json["objects"];

	for(const Json::Value& object : objects) {
		std::string type(object["type"].asString());

		Vector3m position(Json::toVector3m(object["position"]));
		Angle angle(Angle::makeFromDeg(object["angle"].asDouble()));

		Mass mass(Mass::makeFromKg(object["mass"].asDouble()));
		std::string bodyTypeStr(object["body_type"].asString());
		BodyType bodyType(bodyTypeStr == "static" ? STATIC_BODY : DYNAMIC_BODY);

		Color3f color(Json::toColor3f(object["color"]));
		Object3D* createdObject = nullptr;

		if(type == "cuboid") {
			Vector3m dimensions(Json::toVector3m(object["dimensions"]));
			createdObject = &createCube(dimensions, position, mass, bodyType, color);
		} else if(type == "cylinder") {
			Length radius(Length::makeFromM(object["radius"].asDouble()));
			Length height(Length::makeFromM(object["height"].asDouble()));
			createdObject = &createCylinder(radius, height, position, mass, bodyType, color);
		} else if(type == "sphere") {
			Length radius(Length::makeFromM(object["radius"].asDouble()));
			createdObject = &createSphere(radius, position, mass, bodyType, color);
		} else if(type == "model") {
			std::string model(object["model"].asString());
			createdObject = &createModel(position, mass, bodyType, model, color);
		}

		if(createdObject != nullptr) {
			createdObject->setAngle(angle);
		}
	}
}
