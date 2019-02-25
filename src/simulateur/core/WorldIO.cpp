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

	for(const JSON& object : objects) {
		std::string type(object["type"].get<std::string>());

		Vector3m position(Json::toVector3m(object["position"]));
		Angle angle(Angle::makeFromDeg(object["angle"].get<double>()));

		Mass mass(Mass::makeFromKg(object["simulateur"]["mass"].get<double>()));
		BodyType bodyType(object["simulateur"]["dynamic"].get<bool>() ? DYNAMIC_BODY : STATIC_BODY);

		Color3f color(Json::toColor3f(object["simulateur"]["color"]));
		Object3D* createdObject = nullptr;

		if(type == "cuboid") {
			Vector3m dimensions(Json::toVector3m(object["dimensions"]));
			createdObject = &createCube(dimensions, position, mass, bodyType, color);
		} else if(type == "cylinder") {
			Length radius(Length::makeFromM(object["radius"].get<double>()));
			Length height(Length::makeFromM(object["height"].get<double>()));
			createdObject = &createCylinder(radius, height, position, mass, bodyType, color);
		} else if(type == "sphere") {
			Length radius(Length::makeFromM(object["radius"].get<double>()));
			createdObject = &createSphere(radius, position, mass, bodyType, color);
		} else if(type == "model") {
			std::string model(object["model"].get<std::string>());
			createdObject = &createModel(position, mass, bodyType, model, color);
		}

		if(createdObject != nullptr) {
			createdObject->setAngle(angle);

			if(!object["simulateur"]["enabled"].get<bool>()) {
				createdObject->getPhysics().enableSimulation(false);
			}
		}
	}
}

void World::loadWorldFromFile(std::string filename) {
	JSON json;
	std::ifstream in(filename);

	if(in >> json) {
		loadWorldFromJSON(json);
	} else {
		// TODO
	}
	in.close();
}

Object3D& World::createRobotFromJSON(const JSON& json, Constants::RobotColor color) {
	// Permet de récupérer les spécificités du robot principal
	auto& robots = json["robot"];
	auto it = std::find_if(robots.begin(), robots.end(), [](const JSON& j) { return j["name"] == "principal"; });
	if(it == robots.end()) {
		// TODO
		Object3D* obj = &createCube({1_m, 1_m, 1_m}, {0_m, 0_m, 0_m}, 1_kg, STATIC_BODY, {0, 0, 0});
		return *obj;
	}

	const JSON& robot = *it;

	const repere::Coordinates coords_robot(Json::toVector2m(robot["position"]), Angle::makeFromDeg(robot["angle"].get<double>()) /*,
	                                       color == Constantes::RobotColor::Orange ? ConstantesPrincipal::REFERENCE_ORANGE :
	                                                                                 ConstantesPrincipal::REFERENCE_GREEN*/);

	auto position = coords_robot.getPos3D(REFERENCE_SIMULATOR);
	auto angle = coords_robot.getAngle(REFERENCE_SIMULATOR);

	Vector3m robotSize = Json::toVector3m(robot["size"]);
	position.z += robotSize.z / 2;
	IPhysicalInstance* physicProp = getPhysics().createCuboid(position, mass::HEAVY, DYNAMIC_BODY, robotSize);
	physicProp->setAngle(angle);

	// TODO : load la shape du robot
	// IGraphicalInstance* graphicProp = getGraphics().createModel(position, "robot");
	// graphicProp->setScale({0.008, 0.008, 0.012});
	IGraphicalInstance* graphicProp = getGraphics().createCuboid(position, robotSize);
	std::string str_color = toString(color);
	std::transform(str_color.cbegin(), str_color.cend(), str_color.begin(), ::tolower);

	// Set robot color
	if(robot.find("robot") != robot.end() && robot["robot"].find(str_color) != robot["robot"].end()) {
		graphicProp->setColor(Json::toColor3f(robot["robot"][str_color]));
	} else {
		graphicProp->setColor({0.5, 0.5, 0.5});
	}

	Object3D& created = createObject(graphicProp, physicProp, position);
	created.addTag(TAG_ROBOT);

	return created;
}

Object3D& World::createRobotFromFile(std::string filename, Constants::RobotColor color) {
	JSON json;
	std::ifstream in(filename);

	if(in >> json) {
		in.close();
		return createRobotFromJSON(json, color);
	} else {
		in.close();
		// TODO
		Object3D* obj = &createCube({1_m, 1_m, 1_m}, {0_m, 0_m, 0_m}, 1_kg, STATIC_BODY, {0, 0, 0});
		return *obj;
	}
}

JSON World::getJSON() const {
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

			objects.push_back(jsonObject);
		}
	}
	world["objects"] = objects;

	return world;
}

void World::writeJSONToFile(std::string filename) const {
	std::ofstream file(filename);
	file << getJSON() << std::endl;
	file.close();
}
