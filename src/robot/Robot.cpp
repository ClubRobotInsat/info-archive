#include "Robot.h"
#include <Constants.h>
#include <log/Log.h>

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
	        : name(std::move(name)), _module_manager(std::move(module_manager)), _debug_active(false) {
		assign_modules();

		_communicator = std::make_unique<Communication::Communicator<ModuleManager>>(_module_manager);
		_communicator->connect(args);

		try {
			_lidar = Lidar::open_lidar(Lidar::Any);
		} catch(std::runtime_error&) {
			logWarn("Impossible to open the lidar.");
			_lidar = nullptr;
		}
	}

	/// Finalise le robot
	Robot::~Robot() {
		deactivation();
	}

	void Robot::deactivation() {
		if(_debug_active) {
			logInfo("Deactivation of the robot '" + name + "'.");
		}

		_module_manager->deactivation();
		_communicator->disconnect();

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
		if(name == "guest") {
			return;
		}

		for(auto module : GLOBAL_CONSTANTS()[name].get_modules()) {
			/*if(module.first == "moving") {
			    _module_manager->add_module<Navigation>(module.second);
			} else */
			if(module.first == "servos") {
				// TODO : voir comment récupérer les servos à ajouter (`robot.ini` ou fichier .JSON ?)
				_module_manager->add_module<Servos>(module.second);
			} else if(module.first == "motors") {
				_module_manager->add_module<Motors>(module.second);
			} else if(module.first == "io") {
				_module_manager->add_module<IO>(module.second);
			} else if(module.first == "avoidance") {
				auto& avoidance = _module_manager->add_module<Avoidance>(module.second);
				avoidance.set_position_turret(GLOBAL_CONSTANTS()[name].get_turret_position());
			} else {
				throw std::runtime_error("The module named '" + module.first + "' (ID: " + std::to_string(module.second) +
				                         ") isn't known for the robot '" + name + "'.");
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

	    while (not this->_module_manager->get_module<IO>().read_tirette()) {
	        sleep(100_ms);
	    }

	    logInfo("Tirette mise, tirer sur la tirette plz !");
	}

	// Utilitaire : vérification de tirette tirée
	bool Robot::is_tirette_pulled() const {
	    if (not this->_module_manager->get_module<IO>().read_tirette()) {
	        logDebug6("Tirette detectée comme tirée");
	        return true;
	    }
	    return false;
	}*/
} // namespace PhysicalRobot
