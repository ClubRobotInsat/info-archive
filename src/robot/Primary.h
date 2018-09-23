/*
 * Cette classe instantie un `Robot` en le construisant à partir des variables fournies dans le `robot.ini`
 */

#ifndef ROBOT_2015_H
#define ROBOT_2015_H

#include "Commun/Robot.h"

class PrimaryRobot : public Commun::Robot {
public:
	//////////////////
	/// Constantes ///
	//////////////////
	// static const AngularSpeed VITESSE_ASCENSEUR_DEFAUT;
	// static const AngularSpeed VITESSE_RECALAGE_ASCENSEUR;

public:
	///////////////////
	// Constructeurs //
	///////////////////
	PrimaryRobot(std::shared_ptr<Commun::ModuleManager>, std::vector<std::string> const& args);

	explicit PrimaryRobot(std::vector<std::string> const& args);

	~PrimaryRobot() final = default;

	// L'appel est effectué par la classe Strategie lors de sa construction et de sa destruction
	void deactivation() override;

private:
	// Pour le moment, cette fonction lève une exception dès qu'il y a une erreur (pratique pour le debug)
	void assign_modules() override;
};

#endif
