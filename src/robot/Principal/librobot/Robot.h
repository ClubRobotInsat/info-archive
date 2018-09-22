/* Robot2015.h
 */

#ifndef ROBOT_2015_H
#define ROBOT_2015_H

#include "../../Commun/Robot.h"
#include <RobotPrincipal/Constantes.h>
//#include "Strategie.h"

// Identifiants des cartes
//#include "../../../commun/Commun.h"
//#include "../../../commun/RobotPrincipal/Constantes.h"

//#include "../../Commun/Deplacement.h"
//#include <atomic>
//#include <iostream>
//#include <mutex>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class RobotPrincipal : public Commun::Robot {
public:
	//////////////////
	/// Constantes ///
	//////////////////
	// static const AngularSpeed VITESSE_ASCENSEUR_DEFAUT;
	// static const AngularSpeed VITESSE_RECALAGE_ASCENSEUR;

	// static const Distance RAYON_ROBOT_RECALAGE;
	// static const Distance RAYON_ROBOT_RECALAGE_X;

	// static const float COEFF_FIT_MULT;
	// static const float COEFF_FIT_POW;

public:
	///////////////////
	// Constructeurs //
	///////////////////
	RobotPrincipal(std::shared_ptr<Commun::ModuleManager>, std::vector<std::string> const& args);

	explicit RobotPrincipal(std::vector<std::string> const& args);

	~RobotPrincipal() final = default;

	// Vector2m getPositionTourelle() const override;

	// L'appel est effectué par la classe Strategie lors de sa construction et de sa destruction
	virtual void deactivation() override;

	// StrategiePrincipal& getStrategie();

private:
	virtual void assign_modules() override;
};

#endif
