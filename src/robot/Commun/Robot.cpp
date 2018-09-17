//
//  Robot.cpp
//  Club Robot
//
//  Created by Rémi on 07/06/2015.
//

#include "Robot.h"
//#include "Strategie.h"

namespace Commun {


	/// Initialise le robot a partir des arguments passes au programme.
	Robot::Robot(std::shared_ptr<ModuleManager> module_manager,
	             std::unique_ptr<ConstantesCommunes> constantesCommunes,
	             std::unique_ptr<ConstantesRobot> constantes,
	             std::vector<std::string> const& args)
	        : _module_manager(std::move(module_manager))
	        , _constantesCommunes(std::move(constantesCommunes))
	        , _constantes(std::move(constantes)) {
		_communicator = std::make_unique<ElecCommunicator>(_module_manager, _constantes->getPortTCPIPDefault());
		_communicator->connect(args);
	}

	/// Finalise le robot
	Robot::~Robot() {}

	Moving2019& Robot::get_module_move() const {
		return _module_manager->get_module<Moving2019>();
	}

	// TODO : déplacer ce code dans la partie 'stratégie'
	/*void Robot::wait_for_tirette() const {
	    int state_tirette = 0;
	    setting_up_tirette();

	    // Méthode compliquée d'avant : la tirette ne marchait pas trop
	    while (state_tirette < Robot::NB_RETRY_TIRETTE) {
	        if (is_tirette_pulled()) {
	            ++state_tirette;
	            logDebug5(state_tirette);
	        } else {
	            state_tirette = 0;
	        }
	        sleep(100_ms);
	    }

	    // FIXME : tester si ce code suffit
	    while (!is_tirette_pulled()) {
	        sleep(100_ms);
	    }
	}

	// Utilitaire : attente de mise de la tirette
	void Robot::setting_up_tirette() const {
	    logInfo("Attente que l'on mette la tirette");

	    while (not this->_module_manager->get_module<IO2019>().read_tirette()) {
	        sleep(100_ms);
	    }

	    logInfo("Tirette mise, tirer sur la tirette plz !");
	}

	// Utilitaire : vérification de tirette tirée
	bool Robot::is_tirette_pulled() const {
	    if (not this->_module_manager->get_module<IO2019>().read_tirette()) {
	        logDebug6("Tirette detectée comme tirée");
	        return true;
	    }
	    return false;
	}*/
} // namespace Commun
