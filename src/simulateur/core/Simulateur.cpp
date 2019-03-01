#include "Simulateur.h"

#include <csignal>
#include <cstdlib>
#include <functional>

#include "Commun.h"
#include "log/Log.h"
#include "SimulateurConstantes.h"
#include "../graphique/irrlicht/Scene.h"
#include "../gui/gtk/GtkSimuContext.h"
#include "../physique/box2d/Box2DPhysicalContext.h"
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

using Constants::RobotColor;

std::unique_ptr<Simulateur> Simulateur::_instance = nullptr;

Simulateur::Simulateur()
        : _graphicalCtx(std::make_unique<Scene>())
        , _physicalCtx(std::make_unique<Box2DPhysicalContext>(b2Vec2(0, 0)))
        , _guiClient(*this)
        , _theWorld(_physicalCtx.get(), _graphicalCtx.get())
        , _resetWorld(false)
        , _enablePhysics(true) {

	// Ajout du handler pour savoir quand la scène est fermée par l'utilisateur
	if(dynamic_cast<Scene*>(_graphicalCtx.get())) {
		dynamic_cast<Scene&>(*_graphicalCtx).addWindowListener(&UserHandler::instance);
	}

}

Simulateur::~Simulateur() {}

void Simulateur::start() {
	_guiCtx = std::make_unique<GtkSimuContext>(0, nullptr, "simu.gtk.app", _guiClient);

	_simuAlive = true;
	auto last = TimePoint::now();

	while(_simuAlive) {
		update(10_ms);

		// On se cale sur 60 fps.
		auto now = TimePoint::now();
		sleep(std::max(0_s, 1 / 60_Hz - (now - last)));
		last = now;
	}

	destroyWorld();
	_guiCtx->close();
}

void Simulateur::update(Duration time) {
	// Reset du monde si on a le flag
	if(this->_resetWorld) {
		resetWorld();
		_resetWorld = false;
	}

	// Mise à jour du monde
	_robot->update(time);
	_theWorld.update(time);

	_guiCtx->update();
}

void Simulateur::requestStop() {
	_simuAlive = false;
}

void Simulateur::setJSONFile(const std::string& file) {
	_json_file = file;
}

Vector3m const CubeData::getPosition() {
	return Vector3m(position.x + size.x / 2, position.y + size.y / 2, position.z + size.z / 2);
}

void Simulateur::initWorld() {
	// Pour lire à partir du JSON
	if(!_json_file.empty()) {
		_theWorld.loadWorldFromFile(_json_file);
	} else {
		_theWorld.loadWorldFromJSON(GLOBAL_CONSTANTS().TABLE_2018());
	}
	// Pour obtenir le JSON à partir du code de création de la table
	//_theWorld.createTable();
	//_theWorld.createAllObjects(getRobotColor());
	//_theWorld.createDebugObjects();
	//_theWorld.writeJSONToFile("/tmp/table_simu.json");
}

void Simulateur::resetWorld() {
	// Suppression de tous les objets
	destroyWorld();

	// Reconstruction de la table
	initWorld();

	if(_robot != nullptr) {
		addRobot(_robot->getName(), _robot->getColor());
	}
}

void Simulateur::destroyWorld() {
	_theWorld.removeAllObject();
}

void Simulateur::disableSimulation() {
	_theWorld.enableCollisions(false);
}

void Simulateur::addRobot(std::string name, Constants::RobotColor color) {
	Object3D& robotObj = (_json_file.empty() ? _theWorld.createRobotFromJSON(GLOBAL_CONSTANTS().TABLE_2018(), color) :
	                                           _theWorld.createRobotFromFile(_json_file, color));

	_robot = std::make_unique<Simu::SimuRobot>(name, color, robotObj);
}

void Simulateur::sendTextMessage(const std::string& message) {
	_graphicalCtx->displayMessage(message);
}
