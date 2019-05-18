#include "Simulateur.h"

#include <csignal>
#include <cstdlib>
#include <functional>

#include "../../robot/Communication/CommunicatorParsing.h"
#include "../graphique/irrlicht/Scene.h"
#include "../gui/gtk/GtkSimuContext.h"
#include "../physique/box2d/Box2DPhysicalContext.h"
#include "Commun.h"
#include "SimulateurConstantes.h"
#include "log/Log.h"

//#include "../graphique/server/WebGraphicalContext.h"

// Gestion de l'arrêt du simulateur.
class UserHandler : public IGraphicalUserListener {
public:
	static UserHandler instance;
	void onExit() override {
		Simulateur::getInstance().requestStop();
	}
};

UserHandler UserHandler::instance = UserHandler();

using namespace Communication::Arguments;
using Constants::RobotColor;

std::unique_ptr<Simulateur> Simulateur::_instance = nullptr;


Simulateur::Simulateur()
        : _graphicalCtx(std::make_unique<Scene>())
        , _physicalCtx(std::make_unique<Box2DPhysicalContext>(b2Vec2(0, 0)))
        , _theWorld(_physicalCtx.get(), _graphicalCtx.get()) {

	// Ajout du handler pour savoir quand la scène est fermée par l'utilisateur
	if(dynamic_cast<Scene*>(_graphicalCtx.get())) {
		dynamic_cast<Scene&>(*_graphicalCtx).addWindowListener(&UserHandler::instance);
	}
}

Simulateur::~Simulateur() {}

void Simulateur::start() {
	initWorld();
	_guiCtx = std::make_unique<GtkSimuContext>(0, nullptr, "simu.gtk.app", *this);

	_simuAlive = true;
	auto last = TimePoint::now();

	while(_simuAlive) {
		update(10_ms);

		// On se cale sur 60 fps.
		auto now = TimePoint::now();
		sleep(std::max(0_s, 1 / 60_Hz - (now - last)));
		last = now;
	}

	_guiCtx->close();
	destroyWorld();
}

void Simulateur::update(Duration time) {
	// Mise à jour du monde
	if(_robot) {
		_robot->update(time);
	}
	_theWorld.update(time);

	_guiCtx->update();
}

void Simulateur::requestStop() {
	logDebug7("Stop request received");
	_simuAlive = false;
}

void Simulateur::setJSONFile(const std::string& file) {
	_json_file = file;
}

void Simulateur::setPhysicsEnabled(bool enabled) {
	_enablePhysics = enabled;
	_theWorld.enableCollisions(_enablePhysics);
}

void Simulateur::resetWorld() {
	// Suppression de tous les objets
	destroyWorld();

	// Reconstruction de la table
	initWorld();
}

void Simulateur::connect(const ConnectionData& connectionData) {
	std::vector<std::string> args = connectionData.parameters;
	args.insert(args.begin(), connectionData.method);

	auto result = Parser::make_protocol(args);
	if(result.first != typeid(void)) {
		logDebug("Connecting to robot with : ", connectionData.method);
		_robot->connect(std::move(result.second));
	} else {
		logError("Failed to connect with given arguments");
	}
}

void Simulateur::createIAProcess(const IAProcessData& iaProcessData, const ConnectionData& connectionData) {
	logDebug("Launching process : ", iaProcessData.executablePath);
}

void Simulateur::testNavigationForward(Distance distance) {
	if(_robot != nullptr) {
		auto& controller = _robot->getController();
		controller.forward(distance);
	}
}

void Simulateur::testNavigationTurn(Angle angle) {
	if(_robot != nullptr) {
		auto& controller = _robot->getController();
		controller.turnRelative(angle);
	}
}

void Simulateur::resetWorld(const ResetData& resetData) {
	auto color = fromString<RobotColor>(resetData.color);

	setRobotName(resetData.name);
	setRobotColor(color);
	setWorldEnabled(resetData.enableWorld);
	resetWorld();
}

ResetData Simulateur::getResetData() {
	return ResetData{
	    _robotName,
	    toString(_robotColor),
	    _enableWorld,
	    _enablePhysics,
	};
}

std::vector<std::string> Simulateur::getRobotColors() const {
	std::vector<std::string> result;

	for(RobotColor robotColor : getEnumValues<RobotColor>()) {
		result.emplace_back(toString(robotColor));
	}

	return result;
}

void Simulateur::initWorld() {
	// Pour lire à partir du JSON
	if(_enableWorld) {
		if(!_json_file.empty()) {
			_theWorld.loadWorldFromFile(_json_file);
		} else {
			_theWorld.loadWorldFromJSON(GLOBAL_CONSTANTS().TABLE_2019());
		}
	}

	if(_robotName != "off") {
		addRobot(_robotName, _robotColor);
	}
	_theWorld.enableCollisions(_enablePhysics);

	// Pour obtenir le JSON à partir du code de création de la table
	//_theWorld.createTable();
	//_theWorld.createAllObjects(getRobotColor());
	//_theWorld.createDebugObjects();
	//_theWorld.writeJSONToFile("/tmp/tablejson");
}

void Simulateur::destroyWorld() {
	_robot = nullptr;

	_theWorld.removeAllObject();
}

void Simulateur::addRobot(std::string name, Constants::RobotColor color) {
	Object3D& robotObj = (_json_file.empty() ? _theWorld.createRobotFromJSON(GLOBAL_CONSTANTS().TABLE_2019(), color) :
	                                           _theWorld.createRobotFromFile(_json_file, color));

	_robot = std::make_unique<Simu::SimuRobot>(name, color, robotObj);
}

void Simulateur::sendTextMessage(const std::string& message) {
	_graphicalCtx->displayMessage(message);
}
