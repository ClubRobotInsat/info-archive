#include "Simulateur.h"

#include <csignal>
#include <cstdlib>
#include <functional>

#include "../graphique/irrlicht/Scene.h"
#include "SimulateurConstantes.h"
//#include "../graphique/server/WebGraphicalContext.h"
#include "../gui/gtk/GtkSimuContext.h"
#include "../physique/box2d/Box2DPhysicalContext.h"

// Gestion de l'arrêt du simulateur.
// TODO L'arrêt du simulateur doit être géré par le simulateur (retrait de la variable "simuAlive" dans le main)
extern void stopSimu();

class UserHandler : public IGraphicalUserListener {
public:
	static UserHandler instance;
	void onExit() override {
		stopSimu();
	}
};

UserHandler UserHandler::instance = UserHandler();


using namespace Constants;

Simulateur* Simulateur::_instance = nullptr;

Simulateur::Simulateur()
        : _graphicalCtx(std::make_unique<Scene>())
        , _physicalCtx(std::make_unique<Box2DPhysicalContext>(b2Vec2(0, 0)))
        , _guiClient(*this)
        , _theWorld(_physicalCtx.get(), _graphicalCtx.get())
        , _resetWorld(false)
        , _enablePhysics(true) {

	_instance = this;

	// Ajout du handler pour savoir quand la scène est fermée par l'utilisateur
	if(dynamic_cast<Scene*>(_graphicalCtx.get())) {
		dynamic_cast<Scene&>(*_graphicalCtx).addWindowListener(&UserHandler::instance);
	}
}

Simulateur::~Simulateur() {
	endWorld();
	_guiCtx->close();
	_instance = nullptr;
}

void Simulateur::start() {
	_guiCtx = std::make_unique<GtkSimuContext>(0, nullptr, "simu.gtk.app", _guiClient);
}

void Simulateur::update(Duration time) {
	// Reset du monde si on a le flag
	if(this->_resetWorld) {
		resetWorld();
		_resetWorld = false;
	}

	// Mise à jour du monde
	_theWorld.update(time);

	_guiCtx->update();
}

void Simulateur::setJSONFile(const std::string& file) {
	_json_file = file;
}

Vector3m const CubeData::getPosition() {
	return Vector3m(position.x + size.x / 2, position.y + size.y / 2, position.z + size.z / 2);
}

void Simulateur::initWorld() {
	// Pour lire à partir du JSON
	if(_json_file != "") {
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

void Simulateur::disableSimulation() {
	_theWorld.enableCollisions(false);
}

void Simulateur::addRobot(Constants::RobotColor color) {
	Object3D& robotObj = (_json_file.empty() ? _theWorld.createRobotFromJSON(GLOBAL_CONSTANTS().TABLE_2018(), color) :
	                                           _theWorld.createRobotFromFile(_json_file, color));

	_robot = std::make_unique<Simu::SimuRobot>("primary", robotObj);
}

void Simulateur::resetWorld() {
	// Suppression de tous les objets
	_theWorld.removeAllObject();

	// Reconstruction de la table
	initWorld();
	addRobot(getRobotColor());
}

// Est-ce que cette méthode est vraiment nécessaire ?
// Le simu s'arrête à l'appel du destructeur dans tous les cas -- Louis
void Simulateur::endWorld() {
	_theWorld.removeAllObject();
}

void Simulateur::sendTextMessage(const std::string& message) {
	_graphicalCtx->displayMessage(message);
}

Constants::RobotColor Simulateur::getRobotColor() const {
	return RobotColor::Undef;
}