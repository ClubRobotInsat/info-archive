#include "World.h"

#include <fstream>

#include <Json.h>

// TODO Définir le comportement lorsque l'utilisateur fait une erreur dans le json, ou en cas de problèmes de
// lecture/écriture fichier
// Actuellement, ça peut être une grosse exception, comme ça peut passer inaperçu
// Peut-être essayer de charger le Json comme on peut, et afficher des warnings quand il y a des erreurs ?

void World::loadJSON(const JSON& json) {
	const JSON& objects = json["objects"];

	for(const Json::Value& object : objects) {
		std::string type(object["type"].asString());

		Vector3m position(Json::toVector3m(object["position"]));
		Angle angle(Angle::makeFromDeg(object["angle"].asDouble()));

		Mass mass(Mass::makeFromKg(object["mass"].asDouble()));
		BodyType bodyType(object["dynamic"].asBool() ? DYNAMIC_BODY : STATIC_BODY);

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

			if(!object["enabled"].asBool()) {
				createdObject->getPhysics().enableSimulation(false);
			}
		}
	}
}

void World::loadJSONFromFile(std::string filename) {
	Json::CharReaderBuilder builder;
	builder["collectComments"] = false;

	JSON value;
	std::string errs;
	std::ifstream in(filename);
	bool ok = parseFromStream(builder, in, &value, &errs);

	in.close();

	if(ok) {
		loadJSON(value);
	} else {
		// TODO
	}
}

Json::Value World::getJSON() const {
	JSON world;
	JSON objects;

	for(const std::unique_ptr<Object3D>& object : _objectsList) {
		JSON jsonObject(object->getMetadata());

		jsonObject["position"] = Json::fromVector3m(object->getPosition());
		jsonObject["angle"] = object->getRotation().z.toDeg();

		auto& physics = object->getPhysics();
		jsonObject["mass"] = physics.getMass().toKg();
		jsonObject["dynamic"] = physics.isDynamic();
		jsonObject["enabled"] = physics.isEnabled();

		auto& graphics = object->getGraphics();
		jsonObject["color"] = Json::fromColor3f(graphics.getColor());

		objects.append(jsonObject);
	}
	world["objects"] = objects;

	return world;
}

void World::writeJSONToFile(std::string filename) const {
	JSON json = getJSON();
	std::ofstream file(filename);
	Json::StyledWriter writer;
	file << writer.write(json) << std::endl;
	file.close();
}
