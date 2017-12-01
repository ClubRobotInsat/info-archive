//
// Created by scriptopathe on 05/03/16.
//

#ifndef ROOT_ROBOT2017_H
#define ROOT_ROBOT2017_H

// Robot20162017.h

#ifndef ROBOT_TOUMAI_2015_H_
#define ROBOT_TOUMAI_2015_H_

#include <array>

#include "Commun.h"
#include "RobotLogic.h"
#include "sockets/Socket.h"

// Identifiants des cartes
#include "ConstantesCommunes.h"

#define TARGET_SIMULATEUR
#include "RobotPrincipal/Constantes.h"
#undef TARGET_SIMULATEUR


// Interfaces
#include "cartes/IDeplacement.h"
#include "modules/ModuleContactor.h"
#include "modules/ModuleMove.h"
#include "modules/ModuleServos.h"

using namespace Constantes;

// Note : Ce code n'a pas été concerné par la dernière refonte du simu.
// Il peut donc être un peu impraticable par endroit
// N'hésitez pas à le remodeler à votre convenance !
// -- Louis

class Robot2017 : public RobotLogicTemplate<IDCartesPrincipal::IDCartes, ConstantesPrincipal::CarteInfo, Constantes::RobotColor> {
public:
	using Base = RobotLogicTemplate<IDCartesPrincipal::IDCartes, ConstantesPrincipal::CarteInfo, Constantes::RobotColor>;

	static const Mass MASSE_ROBOT;

	// constructeur
	Robot2017(const std::string& connexion, RobotColor col);

	// destructeur
	virtual ~Robot2017();

	void setPhysicalObject(IPhysicalInstance* object);

	// mise à jour du robot
	virtual void update(Duration elapsed) override;

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
	IPhysicalInstance* _obj = nullptr;

	std::unique_ptr<ModuleMove> _moduleMove = nullptr;
	std::unique_ptr<ModuleContactor> _moduleContactor = nullptr;
	std::unique_ptr<ModuleServos> _moduleServos = nullptr;
	// TODO : rajouter tous les modules

	std::unique_ptr<Socket> _socketSimu = nullptr;
	std::thread _tcpServer;
	std::atomic_bool _alive = {true};

	// Liste des contacteurs, à mettre à jour chaque année
	bool _etatContacteur[6];

	Constantes::RobotColor _color;
};

#endif // Robot20162017


#endif // ROOT_ROBOT2017_H
