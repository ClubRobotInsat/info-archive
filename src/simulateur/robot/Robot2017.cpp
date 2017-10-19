//
// Created by scriptopathe on 05/03/16.
//

#include "Robot2017.h"
#include "../3DObject/Object3D.h"
#include "../Simulateur.h"
#include "FormeRobot.h"
// #include "../environnement/Monde2017.h" // pour avoir accès aux robots adv

#include "../cartes/CarteAsservissement2009.h"
#include "../cartes/CarteContacteurs2011.h"
#include "../cartes/CarteDebugDessin.h"
#include "../cartes/CarteDeplacement2009.h"
#include "../cartes/CarteDetectAdv2009.h"
#include "../cartes/CarteIHM.h"
//#include "../cartes/CarteServosNova2017.h"

using namespace ConstantesPrincipal;
using namespace IDCartesPrincipal;
using namespace Constantes;

#include "../simulateur/Simulateur.h"
#include "modules/ModuleMove.h"
//#include "../../robot/Commun/Environment2017.h"
#include "../SimulateurConstantes.h"
#include <list>

const Mass Robot2017::MASSE_ROBOT = 10_kg;

/****************
 * CONSTRUCTEUR *
 ****************/
// FIXME : pouvoir choisir le port, là c'est dégueu
Robot2017::Robot2017(World& world, const std::string& connexion, RobotColor col)
        : RobotLogicTemplate<IDCartes, CarteInfo, RobotColor>(std::make_unique<ConstantesCommunes>(),
                                                              std::make_unique<ConstantesRobotPrincipal>(),
                                                              col,
                                                              connexion) {
	logInfo("Principal en ", col, " (", connexion, ")");
	_color = col;

	// Initialisation des vannes, pompes et contacteurs
	for(int i = 0; i < getNumberofContactor(); i++) {
		_etatContacteur[i] = false;
	}

	// Initialisation de la forme physique (et du ModuleMove)
	recreatePhysic(world);

	// Initialisation des modules
	_moduleContactor = std::make_unique<ModuleContactor>(*this);
	_moduleServos = std::make_unique<ModuleServos>(*this);
	// TODO : rajouter tous les modules

	// Ajout des cartes
	this->addCarteHelper<DEPLACEMENT>(*this, *this, *_moduleMove, world.getSize().y);
	this->addCarteHelper<CAN_USB>(*this, *this);
	this->addCarteHelper<IO>(*this, *this, *_moduleContactor);
	// this->addCarteHelper<SERVOS>(*this, *this, *_moduleServos, enumToInt(ConstantesPrincipal::Servo::NBR));
	// TODO : rajouter toutes les cartes
	// this->addCarteHelper<EVITEMENT>(*this, *this, *_moduleEvitement);

	this->_socketSimu = std::make_unique<Socket>(SockProtocol::TCP);
	this->_tcpServer = std::thread(std::bind(&Robot2017::listenSocket, this));
}

Robot2017::~Robot2017() {
	_alive = false;
	// on libère la mémoire
	_socketSimu = nullptr;
	_moduleMove.reset(nullptr);
	_moduleContactor.reset(nullptr);
	_moduleServos.reset(nullptr);
	// TODO : reset tous les modules

	if(_tcpServer.joinable())
		_tcpServer.join();
}


void Robot2017::update(Duration elapsed) {
	Base::update(elapsed);
}

void Robot2017::resetRobot(World& world) {
	// On recrée la forme physique du robot
	recreatePhysic(world);

	// Initialisation des contacteurs
	for(int i = 0; i < getNumberofContactor(); i++) {
		_etatContacteur[i] = false;
	}

	// Position
	const repere::Coordonnees coords_robot(START_ROBOT_POSITION,
	                                       START_ROBOT_ANGLE,
	                                       getColor() == RobotColor::Blue ? REFERENCE_BLUE : REFERENCE_YELLOW);

	// TODO : changement de repère
	_obj->setPosition(coords_robot.getPos2D(REFERENCE_SIMULATOR));
	_obj->setAngle(coords_robot.getAngle(REFERENCE_SIMULATOR));

	getCarte<DEPLACEMENT>().reset();
}

void Robot2017::recreatePhysic(World& world) {
	// Les coordonnées du robot dans le simu dépend du repère de sa couleur
	const repere::Coordonnees coords_robot(START_ROBOT_POSITION,
	                                       START_ROBOT_ANGLE,
	                                       getColor() == RobotColor::Blue ? REFERENCE_BLUE : REFERENCE_YELLOW);

	// Recrée la forme physique en fonction de la couleur
	world.removeObject(_obj);
	_obj = &world.createObject<FormeRobot>(world,
	                                       coords_robot.getPos3D(REFERENCE_SIMULATOR),
	                                       Object3D::ObjectType::FRIEND_ROBOT_BIG,
	                                       getColor());
	_obj->setAngle(coords_robot.getAngle(REFERENCE_SIMULATOR));

	if(_moduleMove != nullptr) {
		_moduleMove->setPhysicalObject(_obj);
	} else {
		_moduleMove = std::make_unique<ModuleMove>(_obj);
	}
}

/**************
 * INTERFACES *
 **************/

/*
 * interface IMoteurAsservi
 */
void Robot2017::setEngineAngularSpeed(uint8_t /*idCarte*/, AngularSpeed /*speed*/) {}
Angle Robot2017::getEngineAngle(uint8_t /*idCarte*/) {
	return 0_rad;
}
bool Robot2017::isBlocked(uint8_t /*idCarte*/) const {
	return true;
}


/*
 * interface IContacteursV2
 */
int Robot2017::getNumberofContactor() {
	return sizeof(_etatContacteur);
}

bool& Robot2017::getContactor(int contactorID) {
	return _etatContacteur[contactorID];
}


/*************
 * PROTECTED *
 *************/

void Robot2017::listenSocket() {
	Socket clientSock(SockProtocol::TCP);
	setThreadName("ServeurTCPSimu");
	logDebug3("Simu TCP server listening...");
	_socketSimu->listen(ConstantesPrincipal::TCPIP_PORT_SIMU_DEFAULT);

	if(_socketSimu->accept(clientSock)) {
		while(_alive) {

			logDebug3("Waiting for a message on the simu side");
			sleep(2_s);
		}
	}
}

Constantes::RobotColor Robot2017::getColor() {
	return _color;
}