//
// Created by scriptopathe on 05/03/16.
//

#ifndef ROOT_ROBOT2017_H
#define ROOT_ROBOT2017_H

// Robot20162017.h

#ifndef ROBOT_TOUMAI_2015_H_
#define ROBOT_TOUMAI_2015_H_

#include "RobotLogic.h"
#include <array>
class FormeRobot2017;

#include "../../commun/Commun.h"
#include "../../commun/sockets/Socket.h"

// Identifiants des cartes
#include "../../commun/ConstantesCommunes.h"

#define TARGET_SIMULATEUR
#include "../../commun/RobotPrincipal/Constantes.h"
#undef TARGET_SIMULATEUR


// Interfaces
#include "../cartes/IDeplacement.h"
#include "modules/ModuleContactor.h"
#include "modules/ModuleMove.h"
#include "modules/ModuleServos.h"

using namespace Constantes;

class Monde2017;

class Robot2017 : public RobotLogicTemplate<IDCartesPrincipal::IDCartes, ConstantesPrincipal::CarteInfo, Constantes::RobotColor> {
public:
	using Base = RobotLogicTemplate<IDCartesPrincipal::IDCartes, ConstantesPrincipal::CarteInfo, Constantes::RobotColor>;

	static const Mass MASSE_ROBOT;

	// constructeur
	Robot2017(World& world, const std::string& connexion, RobotColor col);

	// destructeur
	virtual ~Robot2017();

	// Accesseur sur FormeRobot
	FormeRobot* getFormeRobot() {
		return _obj;
	};

	// mise à jour du robot
	virtual void update(Duration elapsed) override;
	void resetRobot(World& world);
	// Recrée et ajoute la forme physique du robot au monde passé en paramètres.
	void recreatePhysic(World& world);

	// TODO : toutes les méthodes sont à faire + vérifier que j'ai fait le bon truc -Benjamin
	// interface IServosIntelligents
	/*virtual void setAngle(uint8_t idCarte, uint8_t numServo, Angle angle);
	virtual Angle getAngle(uint8_t idCarte, uint8_t numServo);
	virtual bool isMovementCompleted(uint8_t idCarte, uint8_t numServo);
	virtual void setSpeed(uint8_t idCarte, uint8_t numServo, unsigned char speed);
	virtual bool isBlocked(uint8_t idCarte, uint8_t numServp) const;
	*/
	// interface IMoteurAsservi
	virtual void setEngineAngularSpeed(uint8_t idCarte, AngularSpeed speed);
	virtual Angle getEngineAngle(uint8_t idCarte);
	virtual bool isBlocked(uint8_t idCarte) const;

	// interface IContacteursV2
	virtual bool& getContactor(int contactorID);
	virtual int getNumberofContactor();

	Constantes::RobotColor getColor();


protected:
	void listenSocket();

private:
	std::unique_ptr<ModuleMove> _moduleMove = nullptr;
	std::unique_ptr<ModuleContactor> _moduleContactor = nullptr;
	std::unique_ptr<ModuleServos> _moduleServos = nullptr;
	// TODO : rajouter tous les modules

	FormeRobot* _obj = nullptr;

	std::unique_ptr<Socket> _socketSimu = nullptr;
	std::thread _tcpServer;
	std::atomic_bool _alive = {true};

	// Liste des contacteurs, à mettre à jour chaque année
	bool _etatContacteur[6];

	Constantes::RobotColor _color;
};

#endif // Robot20162017


#endif // ROOT_ROBOT2017_H
