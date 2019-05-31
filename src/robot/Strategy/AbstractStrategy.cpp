//
// Created by terae on 06/10/18.
//

#include <log/Log.h>

#include "AbstractStrategy.h"

#include "resources/EmbeddedFiles.h"

extern void init_petri_avoidance(std::shared_ptr<Strategy::Interfacer::RobotManager> manager,
                                 std::shared_ptr<std::atomic<Distance>> actuator_offset);
extern void init_petri_io(std::shared_ptr<Strategy::Interfacer::RobotManager> manager);
extern void init_petri_navigation(std::shared_ptr<Strategy::Interfacer::RobotManager> manager, Constants::RobotColor color);
extern void init_petri_pumps_primary(std::shared_ptr<Strategy::Interfacer::RobotManager> manager, Constants::RobotColor color);
extern void init_petri_pumps_secondary(std::shared_ptr<Strategy::Interfacer::RobotManager> manager);
extern void init_petri_servos_primary(std::shared_ptr<Strategy::Interfacer::RobotManager> manager,
                                      Constants::RobotColor color,
                                      std::shared_ptr<std::atomic<Distance>> actuators_offset);
extern void init_petri_servos_secondary(std::shared_ptr<Strategy::Interfacer::RobotManager> manager, Constants::RobotColor color);
extern void init_petri_utils(Strategy::AbstractStrategy& strategy);

namespace Strategy {

	AbstractStrategy::AbstractStrategy(Constants::RobotColor color, std::string name) : _color(color), _nb_points(0) {
		_actuator_offset = std::make_shared<std::atomic<Distance>>(0_m);
		if(_color == Constants::RobotColor::Undef) {
			throw std::runtime_error("The strategy color is undefined.");
		}
		setThreadName("Main");

		Distance sx = GLOBAL_CONSTANTS()["default"].get_size().x;
		Distance sy = GLOBAL_CONSTANTS()["default"].get_size().y;
		_env = std::make_unique<Environment>(GLOBAL_CONSTANTS().get_table_env_grid(),
		                                     GLOBAL_CONSTANTS().get_scale_environment(),
		                                     std::min(sx, sy) * 1.15,
		                                     (sqrt(sx * sx + sy * sy) / 2) * 1.35,
		                                     Vector2m{0_m, 0_m});

		this->create_environment();

		try {
			logInfo("Loading Petri::Utils...");
			init_petri_utils(*this);
		} catch(std::exception const& e) {
			logError("Impossible to initialize PetriLab: ", e.what());
		}
		logInfo("name: ", name);

		set_points(0);
	}

	void AbstractStrategy::create_environment() {
		this->_env->loadFromJSON(GLOBAL_CONSTANTS().TABLE_2019());
	}

	void AbstractStrategy::start(Duration match_duration) {
		_total_duration_match = match_duration;

		logDebug9("Couleur du robot : ", this->get_color());
		logDebug("Début du match !");
		logDebug("Durée: ", match_duration);
		this->reset_timer();

		_execution = std::thread(std::bind(&AbstractStrategy::exec, this));

		while(get_left_time() > 0_s) {
			sleep(100_ms);
		}

		stop();

		logDebug("FIN DU MATCH !");
		logDebug("Funny action");
		this->funny_action();

		// Arrêt du thread d'exécution
		auto handle = _execution.native_handle();
		_execution.detach();
		pthread_cancel(handle);
		pthread_join(handle, nullptr);

		stop();
	}

	void AbstractStrategy::stop() {
		for(auto& [name, manager] : _interfacers) {
			manager->get_robot()->deactivation();
		}
	}

	Constants::RobotColor AbstractStrategy::get_color() const {
		return _color;
	}

	const repere::Repere& AbstractStrategy::get_reference() const {
		return GLOBAL_CONSTANTS().get_reference(_color);
	}

	Duration AbstractStrategy::get_left_time() const {
		// logDebug6("Il reste :", (_dureeTotaleMatch - _chronoMatch.getElapsedTime()).toS());
		return _total_duration_match - get_time();
	}

	Duration AbstractStrategy::get_time() const {
		return _chrono_match.getElapsedTime();
	}

	void AbstractStrategy::reset_timer() {
		_chrono_match.reset();
	}

	Environment& AbstractStrategy::get_environment() const {
		return *_env;
	}

	std::shared_ptr<Interfacer::RobotManager> AbstractStrategy::add_robot(std::shared_ptr<PhysicalRobot::Robot> robot) {
		auto manager = std::make_shared<Interfacer::RobotManager>(robot);
		// Interfacer::AvoidanceInterfacer
		if(!robot->has_lidar()) {
			logWarn("No lidar found. Avoidance may not work correctly");
		}
		if(debug_mode) {
			logInfo("Insertion of an Interfacer::AvoidanceInterfacer inside the robot '" + robot->name + "'");
		}

		logInfo("name: ", robot->name);
		auto& avoidance =
		    manager->add_interfacer<Interfacer::AvoidanceInterfacer>(get_environment(),
		                                                             GLOBAL_CONSTANTS()[robot->name].get_turret_position());

		// Interfacer::NavigationInterfacer
		if(manager->get_robot()->has_module<PhysicalRobot::Navigation>()) {
			if(debug_mode) {
				logInfo("Insertion of an Interfacer::NavigationInterfacer inside the robot '" + robot->name + "'");
			}
			manager->add_interfacer<Interfacer::NavigationInterfacer>(get_environment(), avoidance);
		}
		// Interfacer::IOInterfacer
		if(manager->get_robot()->has_module<PhysicalRobot::IO>()) {
			if(debug_mode) {
				logInfo("Insertion of an Interfacer::IOInterfacer inside the robot '" + robot->name + "'");
			}
			manager->add_interfacer<Interfacer::IOInterfacer>();
		}
		// Interfacer::CaptorsInterfacerSecondary
		if(manager->get_robot()->has_module<PhysicalRobot::Captors>() && robot->name == "secondary") {
			if(debug_mode) {
				logInfo("Inserion of an Interfacer::CaptorsInterfacerSecondary inside the robot '" + robot->name + "'");
			}
			manager->add_interfacer<Interfacer::CaptorsInterfacerSecondary>();
		}
		// Interfacer::PumpsInterfacerSecondary
		if(manager->get_robot()->has_module<PhysicalRobot::Pumps>() && robot->name == "secondary" &&
		   manager->has_interfacer<Interfacer::CaptorsInterfacerSecondary>()) {
			if(debug_mode) {
				logInfo("Insertion of an Interfacer::PumpsInterfacerSecondary inside the robot '" + robot->name + "'");
			}
			manager->add_interfacer<Interfacer::PumpsInterfacerSecondary>(
			    manager->get_interfacer<Interfacer::CaptorsInterfacerSecondary>());
		}
		// Interfacer::PumpsInterfacerPrimary
		else if(manager->get_robot()->has_module<PhysicalRobot::Pumps>()) {
			if(debug_mode) {
				logInfo("Insertion of an Interfacer::PumpsInterfacerPrimary inside the robot '" + robot->name + "'");
			}
			manager->add_interfacer<Interfacer::PumpsInterfacerPrimary>();
		}
		// Interfacer::ServosInterfacer
		if(manager->get_robot()->has_module<PhysicalRobot::Servos>()) {
			const auto list_id = GLOBAL_CONSTANTS()[robot->name].get_list_id_servos();
			if(debug_mode) {
				logInfo("Insertion of an Interfacer::ServosInterfacer inside the robot '" + robot->name + "' with " +
				        std::to_string(list_id.size()) + " servos");
			}
			auto& servos = manager->add_interfacer<Interfacer::ServosInterfacer>();
			for(uint8_t id : list_id) {
				servos->add_servo(id);
			}
		}

		return add_manager(manager);
	}

	std::shared_ptr<Interfacer::RobotManager> AbstractStrategy::add_manager(std::shared_ptr<Interfacer::RobotManager> manager) {
		try {
			// Interfacer::AvoidanceInterfacer
			if(manager->has_interfacer<Interfacer::AvoidanceInterfacer>()) {
				if(debug_mode) {
					logInfo("Insertion of PetriLab::AvoidanceInterfacer functionalities associated with the robot '" +
					        manager->get_robot()->name + "'");
				}
				init_petri_avoidance(manager, _actuator_offset);
			}
			// Interfacer::IOInterfacer
			if(manager->has_interfacer<Interfacer::IOInterfacer>()) {
				if(debug_mode) {
					logInfo("Insertion of PetriLab::IO functionalities associated with the robot '" +
					        manager->get_robot()->name + "'");
				}
				init_petri_io(manager);
			}
			// Interfacer::NavigationInterfacer
			if(manager->has_interfacer<Interfacer::NavigationInterfacer>()) {
				if(debug_mode) {
					logInfo("Insertion of PetriLab::Navigation functionalities associated with the robot '" +
					        manager->get_robot()->name + "'");
				}
				init_petri_navigation(manager, _color);
			}
			// Interfacer::PumpsInterfacerSecondary
			if(manager->has_interfacer<Interfacer::PumpsInterfacerSecondary>()) {
				if(debug_mode) {
					logInfo("Insertion of PetriLab::PumpsSecondary functionalities associated with the robot '" +
					        manager->get_robot()->name + "'");
				}
				init_petri_pumps_secondary(manager);
			}
			// Interfacer::PumpsInterfacerPrimary
			if(manager->has_interfacer<Interfacer::PumpsInterfacerPrimary>()) {
				if(debug_mode) {
					logInfo("Insertion of PetriLab::PumpsPrimary functionalities associated with the robot '" +
					        manager->get_robot()->name + "'");
				}
				init_petri_pumps_primary(manager, _color);
			}
			// Interfacer::ServosInterfacer
			if(manager->has_interfacer<Interfacer::ServosInterfacer>()) {
				if(debug_mode) {
					logInfo("Insertion of PetriLab::Servos functionalities associated with the robot '" +
					        manager->get_robot()->name + "'");
				}
				if(manager->get_robot()->name == "secondary") {
					init_petri_servos_secondary(manager, _color);
				} else {
					init_petri_servos_primary(manager, _color, _actuator_offset);
				}
			}
		} catch(std::exception const& e) {
			logError("Impossible to initialize PetriLab: ", e.what());
		}

		_interfacers[manager->get_robot()->name] = manager;

		logInfo("name: ", manager->get_robot()->name);

		return manager;
	}

	std::shared_ptr<Interfacer::RobotManager> AbstractStrategy::get_robot(const std::string& name) {
		auto it = _interfacers.find(name);
		if(it != _interfacers.cend()) {
			return it->second;
		}

		return nullptr;
	}

	std::vector<std::string> AbstractStrategy::get_robot_names() const {
		std::vector<std::string> result;
		std::transform(_interfacers.cbegin(), _interfacers.cend(), std::back_inserter(result), [](const auto& it) {
			return it.first;
		});
		return result;
	}

	void AbstractStrategy::wait_for_tirette() const {
		for(auto& [name, manager] : _interfacers) {
			auto& io = manager->get_interfacer<Interfacer::IOInterfacer>();
			if(!io.is_tirette_triggered()) {
				logInfo("Waiting for the insertion of the tirette on robot '", name, "'.");
				io.wait_insertion_tirette();
			}
		}

		for(auto i : _interfacers) {
			auto& io = i.second->get_interfacer<Interfacer::IOInterfacer>();
			io.wait_deletion_tirette();
		}
	}

	int AbstractStrategy::get_points() const {
		return _nb_points;
	}

	int AbstractStrategy::add_points(int n) {
		return _nb_points += n;
	}

	int AbstractStrategy::set_points(int n) {
		_nb_points = n;
		return _nb_points;
	}

	void AbstractStrategy::exec() {
		setThreadName("Strategy");
		// Autoriser l'interruption du thread à la volée
		// Pas propre (mélange std::thread et pthread, mais surtout pas de libération des ressources (locks,
		// allocations…)
		// !!!)
		pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, nullptr);

		this->execute();
	}

} // namespace Strategy
