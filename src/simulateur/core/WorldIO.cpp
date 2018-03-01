#include "RobotPrincipal/Constantes.h"
#include "SimulateurConstantes.h"

#include "World.h"

#include <fstream>

#include <Json.h>

// TODO Définir le comportement lorsque l'utilisateur fait une erreur dans le json, ou en cas de problèmes de
// lecture/écriture fichier
// Actuellement, ça peut être une grosse exception, comme ça peut passer inaperçu
// Peut-être essayer de charger le Json comme on peut, et afficher des warnings quand il y a des erreurs ?

void World::loadWorldFromJSON(const JSON& json) {
	const JSON& objects = json["objects"];

	for(const Json::Value& object : objects) {
		std::string type(object["type"].asString());

		Vector3m position(Json::toVector3m(object["position"]));
		Angle angle(Angle::makeFromDeg(object["angle"].asDouble()));

		Mass mass(Mass::makeFromKg(object["simulateur"]["mass"].asDouble()));
		BodyType bodyType(object["simulateur"]["dynamic"].asBool() ? DYNAMIC_BODY : STATIC_BODY);

		Color3f color(Json::toColor3f(object["simulateur"]["color"]));
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

void World::loadWorldFromFile(std::string filename) {
	Json::CharReaderBuilder builder;
	builder["collectComments"] = false;

	JSON value;
	std::string errs;
	std::ifstream in(filename);
	bool ok = parseFromStream(builder, in, &value, &errs);

	in.close();

	if(ok) {
		loadWorldFromJSON(value);
	} else {
		// TODO
	}
}

Object3D& World::createRobotFromJSON(const Json::Value& json, Constantes::RobotColor color) {
	// Permet de récupérer les spécificités du robot principal
	auto it = std::find_if(json["robot"].begin(), json["robot"].end(), [](const Json::Value& j) {
		return j["name"] == "principal";
	});
	if(it == json["robot"].end()) {
		// TODO
		Object3D* obj = &createCube({1_m, 1_m, 1_m}, {0_m, 0_m, 0_m}, 1_kg, STATIC_BODY, {0, 0, 0});
		return *obj;
	}

	const JSON& robot = *it;

	const repere::Coordonnees coords_robot(Json::toVector2m(robot["position"]),
	                                       Angle::makeFromDeg(robot["angle"].asDouble()),
	                                       color == Constantes::RobotColor::Orange ? ConstantesPrincipal::REFERENCE_ORANGE :
	                                                                                 ConstantesPrincipal::REFERENCE_GREEN);

	auto position = coords_robot.getPos3D(REFERENCE_SIMULATOR);
	auto angle = coords_robot.getAngle(REFERENCE_SIMULATOR);

	Vector3m robotSize = Json::toVector3m(robot["size"]);
	IPhysicalInstance* physicProp = getPhysics().createCuboid(position, mass::HEAVY, DYNAMIC_BODY, robotSize);
	physicProp->setAngle(angle);

	// TODO : load la shape du robot
	// IGraphicalInstance* graphicProp = getGraphics().createModel(position, "robot");
	// graphicProp->setScale({0.008, 0.008, 0.012});
	IGraphicalInstance* graphicProp = getGraphics().createCuboid(position, robotSize);
	graphicProp->setColor(color == Constantes::RobotColor::Orange ? Json::toColor3f(robot["color"]["orange"]) :
	                                                                Json::toColor3f(robot["color"]["green"]));

	Object3D& created = createObject(graphicProp, physicProp, position);
	created.addTag(TAG_ROBOT);

	return created;
}

Object3D& World::createRobotFromFile(std::string filename, Constantes::RobotColor color) {
	Json::CharReaderBuilder builder;
	builder["collectComments"] = false;

	JSON value;
	std::string errs;
	std::ifstream in(filename);
	bool ok = parseFromStream(builder, in, &value, &errs);

	in.close();

	if(ok) {
		return createRobotFromJSON(value, color);
	} else {
		// TODO
		Object3D* obj = &createCube({1_m, 1_m, 1_m}, {0_m, 0_m, 0_m}, 1_kg, STATIC_BODY, {0, 0, 0});
		return *obj;
	}
}

Json::Value World::getJSON() const {
	JSON world;
	JSON objects;

	for(const std::unique_ptr<Object3D>& object : _objectsList) {
		if(object->hasTag("robot")) {
			JSON robot;
			robot["position"] = Json::fromVector3m(object->getPosition());
			robot["angle"] = object->getRotation().z.toDeg();
			robot["size"] = Json::fromVector3m({38_cm, 30_cm, 50_m});
			robot["color"]["orange"] = Json::fromColor3f({0.93, 0.5, 0.06});
			robot["color"]["green"] = Json::fromColor3f({0.0, 1.0, 0.0});

			world["robot"] = robot;
		} else {
			JSON jsonObject(object->getMetadata());

			jsonObject["position"] = Json::fromVector3m(object->getPosition());
			jsonObject["angle"] = object->getRotation().z.toDeg();

			auto& physics = object->getPhysics();
			jsonObject["simulator"]["mass"] = physics.getMass().toKg();
			jsonObject["simulator"]["dynamic"] = physics.isDynamic();
			jsonObject["simulator"]["enabled"] = physics.isEnabled();

			auto& graphics = object->getGraphics();
			jsonObject["simulator"]["color"] = Json::fromColor3f(graphics.getColor());

			objects.append(jsonObject);
		}
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
