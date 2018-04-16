#include "Simulateur.h"

#include <csignal>
#include <cstdlib>
#include <functional>

#include "../communication/Robot2018.h"
#include "../graphique/irrlicht/Scene.h"
//#include "../graphique/server/WebGraphicalContext.h"
#include "../physique/box2d/Box2DPhysicalContext.h"
#include "SimulateurConstantes.h"


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


using namespace Constantes;

Simulateur* Simulateur::_instance = nullptr;

Simulateur::Simulateur()
        : _graphicalCtx(std::make_unique<Scene>())
        , _physicalCtx(std::make_unique<Box2DPhysicalContext>(b2Vec2(0, 0)))
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
	_instance = nullptr;
}

void Simulateur::update(Duration time) {
	// Reset du monde si on a le flag
	if(this->_resetWorld) {
		resetWorld();
		_resetWorld = false;
	}

	// Mise à jour du monde
	_theWorld.update(time);

	// Mise à jour de la communication avec l'IA
	if(_robot != nullptr) {
		_robot->update(time);
	}
}

void Simulateur::setJSONFile(std::string file) {
	_json_file = file;
}

Vector3m const CubeData::getPosition() {
	return Vector3m(position.x + size.x / 2, position.y + size.y / 2, position.z + size.z / 2);
}

void Simulateur::initWorld() {
	// Pour lire à partir du JSON
	_theWorld.loadWorldFromFile(_json_file);

	// Pour obtenir le JSON à partir du code de création de la table
	//_theWorld.createTable();
	//_theWorld.createAllObjects(getRobotColor());
	//_theWorld.createDebugObjects();
	//_theWorld.writeJSONToFile("/tmp/table_simu.json");
}

void Simulateur::disableSimulation() {
	_theWorld.enableCollisions(false);
}

void Simulateur::addRobot(Constantes::RobotColor color) {
	_robot = std::make_unique<Robot2018>("LOCAL", color);
	Object3D& robotObj = _theWorld.createRobotFromFile(_json_file, color);
	_robot->setPhysicalObject(&robotObj.getPhysics());
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
	if(_robot != nullptr)
		_robot = nullptr;

	_theWorld.removeAllObject();
}

void Simulateur::sendTextMessage(const std::string& message) {
	_graphicalCtx->displayMessage(message);
}

Constantes::RobotColor Simulateur::getRobotColor() {
	return _robot == nullptr ? RobotColor::Undef : _robot->getColor();
}