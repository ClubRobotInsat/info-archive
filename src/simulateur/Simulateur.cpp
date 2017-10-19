//
// Created by louis on 07/02/16.
//

#include "Simulateur.h"
#include "SimulateurConstantes.h"
#include "robot/Robot2017.h"
#include <cstdlib>

#include <csignal>
#include <functional>


Simulateur* Simulateur::_instance = nullptr;
std::function<void()> exitHandler;


Simulateur::Simulateur() : _theWorld(b2Vec2(0, 0)), _server{5000}, _resetWorld(false), _enablePhysics(true) {

	_instance = this;
	exitHandler = [&]() {
		_server.stop();
		exit(0);
	};
	std::signal(SIGTERM, [](int) { exitHandler(); });
	std::signal(SIGINT, [](int) { exitHandler(); });
}

Simulateur::~Simulateur() {
	shutdownWebServer();
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
	if(_robot != nullptr) {
		_robot->update(time);
	}

	// Envoi des données à l'autre serveur
	if(!_theWorld.getMessageList().empty()) {
		auto const& json = _theWorld.getMessageList();
		Json::FastWriter writer;
		writer.omitEndingLineFeed();

		std::string s = writer.write(json);

		Simulateur::_server.broadcast(s);

		_theWorld.clearMessageList();
	}
}

void Simulateur::sendWorldData() {
	if(!_theWorld.getMessageList().empty()) {
		auto const& json = _theWorld.getMessageList();
		Json::FastWriter writer;
		writer.omitEndingLineFeed();

		std::string s = writer.write(json);

		Simulateur::_server.broadcast(s);

		_theWorld.clearMessageList();
	}
}

Vector3m const CubeData::getPosition() {
	return Vector3m(position.x + size.x / 2, position.y + size.y / 2, position.z + size.z / 2);
}

void Simulateur::initWorld() {
	// Permet de savoir si on place un module lunaire dans une  zone de départ et laquelle
	int robot = 0;
	if(_robot != nullptr and getRobotColor() == RobotColor::Yellow)
		robot = 1;
	if(_robot != nullptr and getRobotColor() == RobotColor::Blue)
		robot = 2;

	_theWorld.createTable();
	_theWorld.createAllObjects(robot);
	//_theWorld.createDebugObjects();
}

void Simulateur::disableSimulation() {
	_enablePhysics = false;
	_theWorld.enableSimulation(false);
	_robot->getFormeRobot()->enableSimulation(true);
}

void Simulateur::addRobot(Constantes::RobotColor color) {
	_robot = std::make_unique<Robot2017>(_theWorld, "LOCAL", color);
}

void Simulateur::resetWorld() {
	logDebug5("Réinitialisation du simulateur");
	_theWorld.removeAllObject();
	sendWorldData();

	_robot->resetRobot(_theWorld);
	initWorld();

	if(!_enablePhysics) {
		disableSimulation();
	}

	_theWorld.sendWorldState();
}

void Simulateur::endWorld() {
	if(_robot != nullptr)
		_robot = nullptr;

	_theWorld.removeAllObject();
}

void Simulateur::activateWebServer() {
	_server.start();
}

void Simulateur::shutdownWebServer() {
	if(_server.isClientConnected()) {
		_server.stop();
	}
}

void Simulateur::sendTextMessage(std::string messageToSend) {
	JSON list;
	JSON message;
	message["type"] = "log";
	message["message"] = messageToSend;
	list.append(message);
	Json::FastWriter writer;
	writer.omitEndingLineFeed();
	std::string messageConverted = writer.write(list);
	_server.broadcast(messageConverted);
}

Constantes::RobotColor Simulateur::getRobotColor() {
	return _robot->getColor();
}