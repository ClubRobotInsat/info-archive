#include "Simulateur.h"

#include <csignal>
#include <cstdlib>
#include <functional>

#include "../communication/Robot2017.h"
#include "../graphique/WebGraphicalContext.h"
#include "../physique/Box2DPhysicalContext.h"
#include "SimulateurConstantes.h"

using namespace Constantes;

Simulateur* Simulateur::_instance = nullptr;

Simulateur::Simulateur()
        : _physics(std::make_unique<Box2DPhysicalContext>(b2Vec2(0, 0)))
        , _graphics(std::make_unique<WebGraphicalContext>(5000))
        , _theWorld(_physics.get(), _graphics.get())
        , _resetWorld(false)
        , _enablePhysics(true) {

	_instance = this;
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

void Simulateur::sendTextMessage(const std::string& message) {}

Vector3m const CubeData::getPosition() {
	return Vector3m(position.x + size.x / 2, position.y + size.y / 2, position.z + size.z / 2);
}

void Simulateur::initWorld() {
	_theWorld.createTable();
	_theWorld.createAllObjects(getRobotColor());
	//_theWorld.createDebugObjects();
}

void Simulateur::disableSimulation() {
	// TODO disable simulation
}

void Simulateur::addRobot(Constantes::RobotColor color) {
	_robot = std::make_unique<Robot2017>("LOCAL", color);
	Object3D& robotObj = _theWorld.createRobot(color);
	_robot->setPhysicalObject(&robotObj.getPhysics());
}

void Simulateur::resetWorld() {
	// Suppression de tous les objets
	_theWorld.removeAllObject();

	// Reconstruction de la table
	_theWorld.createTable();
	_theWorld.createAllObjects(getRobotColor());
	Object3D& robotObj = _theWorld.createRobot(getRobotColor());
	_robot->setPhysicalObject(&robotObj.getPhysics());
}

// Est-ce que cette méthode est vraiment nécessaire ?
// Le simu s'arrête à l'appel du destructeur dans tous les cas -- Louis
void Simulateur::endWorld() {
	if(_robot != nullptr)
		_robot = nullptr;

	_theWorld.removeAllObject();
}

Constantes::RobotColor Simulateur::getRobotColor() {
	return _robot == nullptr ? RobotColor::Undef : _robot->getColor();
}