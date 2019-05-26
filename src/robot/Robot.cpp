#include "Robot.h"
#include "../Lidar/filtre.h"

#include <Constants.h>
#include <log/Log.h>

namespace PhysicalRobot {

	// Le robot n'est pas initialisé à partir de `src/robot.ini`
	// L'utilisateur doit donc fournir un ModuleManager non vierge s'il veut un robot fonctionnel
	Robot::Robot(std::shared_ptr<ModuleManager> module_manager, const std::vector<std::string>& args, Lidar::LidarType lidar, bool debug_active)
	        : Robot(std::move(module_manager), "default", lidar, debug_active) {
		_communicator = std::make_unique<Communication::Communicator<ModuleManager>>(_module_manager);
		_communicator->connect(args);
	}

	// Le robot est initialisé à partir de `src/robot.ini` dans la section `[robot.<name>]`
	Robot::Robot(std::string name, bool debug_active)
	        : Robot(std::move(name), GLOBAL_CONSTANTS()[name].get_lidar_type(), debug_active) {}

	Robot::Robot(std::string name, Lidar::LidarType lidar, bool debug_active)
	        : Robot(std::make_shared<ModuleManager>(), name, lidar, debug_active) {
		_communicator = std::make_unique<Communication::Communicator<ModuleManager>>(_module_manager);
		_communicator->connect(GLOBAL_CONSTANTS()[name]);
	}

	Robot::Robot(std::string name, const std::vector<std::string>& args, Lidar::LidarType lidar, bool debug_active)
	        : Robot(std::make_shared<ModuleManager>(), name, lidar, debug_active) {
		_communicator = std::make_unique<Communication::Communicator<ModuleManager>>(_module_manager);
		_communicator->connect(args);
	}

	Robot::Robot(std::string name, const std::vector<std::string>& args, bool debug_active)
	        : Robot(name, args, GLOBAL_CONSTANTS()[name].get_lidar_type(), debug_active) {}

	/// Initialise le robot à partir des arguments passés au programme.
	Robot::Robot(std::shared_ptr<ModuleManager> module_manager, std::string name, Lidar::LidarType lidar, bool debug_active)
	        : name(std::move(name)), _module_manager(std::move(module_manager)), _debug_active(debug_active) {
		assign_modules();

		try {
			_lidar = Lidar::open_lidar(lidar);
		} catch(std::runtime_error&) {
			logWarn("Impossible to open the lidar.");
			_lidar = nullptr;
		}
	}

	/// Finalise le robot
	Robot::~Robot() {
		deactivation();
	}

	std::type_index Robot::get_communication_protocol_type() const {
		return _communicator->get_protocol_type();
	}

	Lidar::LidarType Robot::get_lidar_type() const {
		if(_lidar == nullptr) {
			return Lidar::None;
		}
		return _lidar->type;
	}

	bool Robot::has_lidar() const {
		return get_lidar_type() != Lidar::None;
	}

	std::optional<FrameLidar> Robot::get_lidar_frame() const {
		if(has_lidar()) {
			return Filtre().get_frame(_lidar->get_frame());
		}
		return std::nullopt;
	}

	void Robot::set_debug(bool debug) {
		_debug_active = debug;
		if(_communicator != nullptr) {
			_communicator->set_debug(debug);
		}
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
		// Un robot 'default' est un robot dont l'initialisation se fait à partir d'un ModuleManager directement
		// sans passer par les constantes introduites dans `src/robot.ini`
		if(name == "default") {
			return;
		}

		for(const auto& module : GLOBAL_CONSTANTS()[name].get_modules()) {
			if(module.first == "navigation") {
				_module_manager->add_module<Navigation>(module.second);
			} else if(module.first == "servos") {
				_module_manager->add_module<Servos>(module.second);
			} else if(module.first == "motors") {
				_module_manager->add_module<Motors>(module.second);
			} else if(module.first == "io") {
				_module_manager->add_module<IO>(module.second);
			} else if(module.first == "pumps") {
				_module_manager->add_module<Pumps>(module.second);
			} else if(module.first == "navigation_parameters") {
				_module_manager->add_module<NavigationParameters>(module.second);
			} else {
				throw std::runtime_error("The module named '" + module.first + "' (ID: " + std::to_string(module.second) +
				                         ") isn't known for the robot '" + name + "'.");
			}
		}
	}
} // namespace PhysicalRobot
