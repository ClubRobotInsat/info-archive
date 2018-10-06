#include "Robot.h"
#include <Constants.h>

namespace PhysicalRobot {

	// Le robot n'est pas initialisé à partir de `src/robot.ini`
	// L'utilisateur doit donc fournir un ModuleManager non vierge s'il veut un robot fonctionnel
	Robot::Robot(std::shared_ptr<ModuleManager> module_manager, std::vector<std::string> const& args)
	        : Robot(std::move(module_manager), "guest", args) {}

	// Le robot est initialisé à partir de `src/robot.ini` dans la section `[robot.<name>]`
	Robot::Robot(std::string name, std::vector<std::string> const& args)
	        : Robot(std::make_shared<ModuleManager>(), std::move(name), args) {}

	/// Initialise le robot à partir des arguments passes au programme.
	Robot::Robot(std::shared_ptr<ModuleManager> module_manager, std::string name, std::vector<std::string> const& args)
	        : _module_manager(std::move(module_manager)), _name(std::move(name)) {
		_communicator =
		    std::make_unique<Communication::ElecCommunicator<ModuleManager>>(_module_manager,
		                                                                     GLOBAL_CONSTANTS.get_default_TCPIP_port());
		_communicator->connect(args);

		// Après avoir créé tous les modules, on dit au communicateur qu'il peut exécuter son thread de communication
		assign_modules();
		_communicator->set_modules_initialized();
	}

	/// Finalise le robot
	Robot::~Robot() {
		deactivation();
	}

	void Robot::deactivation() {
		logInfo("Deactivation of the robot '" + _name + "'.");

		_module_manager->deactivation();

		/*this->getCarte<DEPLACEMENT>().activerEnvoiAuto(false);
		this->getCarte<DEPLACEMENT>().arretUrgence();

		this->getCarte<DEPLACEMENT>().activerAsservissement(CarteInfo<DEPLACEMENT>::typeCarte::POSITION, false);
		this->getCarte<DEPLACEMENT>().activerAsservissement(CarteInfo<DEPLACEMENT>::typeCarte::ANGLE, false);
		this->getCarte<DEPLACEMENT>().activerAsservissement(CarteInfo<DEPLACEMENT>::typeCarte::DROITE, false);
		this->getCarte<DEPLACEMENT>().activerAsservissement(CarteInfo<DEPLACEMENT>::typeCarte::GAUCHE, false);*/

		// this->getCarte<EVITEMENT>().desactiverEnvoiMesureAdvAuto();

		// getStrategie().getMecaManager().couperMeca();
	}

	void Robot::assign_modules() {
		// Un robot 'guest' est un robot dont l'initialisation se fait à partir d'un ModuleManager directement
		// sans passer par les constantes introduites dans `src/robot.ini`
		if(_name == "guest") {
			return;
		}

		for(auto module : GLOBAL_CONSTANTS[_name].get_modules()) {
			if(module.first == "moving") {
				_module_manager->add_module<Moving2019>(module.second);
			} else if(module.first == "servos") {
				// TODO : voir comment récupérer les servos à ajouter (`robot.ini` ou fichier .JSON ?)
				_module_manager->add_module<Servos2019>(module.second);
			} else if(module.first == "motors") {
				_module_manager->add_module<Motors2019>(module.second);
			} else if(module.first == "io") {
				_module_manager->add_module<IO2019>(module.second);
			} else if(module.first == "avoidance") {
				auto& avoidance = _module_manager->add_module<Avoidance2019>(module.second);
				avoidance.set_position_turret(GLOBAL_CONSTANTS[_name].get_turret_position());
			} else {
				throw std::runtime_error("The module named '" + module.first + "' (ID: " + std::to_string(module.second) +
				                         ") isn't known for the robot '" + _name + "'.");
			}
		}
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
} // namespace PhysicalRobot
