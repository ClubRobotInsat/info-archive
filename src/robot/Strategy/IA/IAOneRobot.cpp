//
// Created by terae on 23/05/19.
//

#include "IAOneRobot.h"
#include <AsciiArt.hpp>
#include <petrilab/Cpp/PetriLab.h>

namespace Strategy {
	IAOneRobot::IAOneRobot(const IAArguments& args, std::string robot_name)
	        : AbstractStrategy(args.get_color()), name(std::move(robot_name)) {
		if(debug_mode) {
			logInfo("Initialization of the physical robot '", name, "'");
			args.print_parsed_arguments();
		}

		std::shared_ptr<PhysicalRobot::Robot> physical_robot;

		if(args.is_running_on_simulator()) {
			physical_robot =
			    std::make_shared<PhysicalRobot::Robot>(name, std::vector({"IAOneRobot"s, "UDP"s, "localhost"s, "5001"s, "5101"s}), true);
		} else {
			physical_robot = std::make_shared<PhysicalRobot::Robot>(name, true);
		}
		add_robot(physical_robot);
	}

	std::shared_ptr<Interfacer::RobotManager> IAOneRobot::get_robot() const {
		return _interfacers.find(name)->second;
	}

	std::string IAOneRobot::get_robot_name() const {
		return name;
	}

	void IAOneRobot::init_strategy() {
		_petrilab = generate_petrilab_lib();
		_petrilab->load();

		logDebug0("Tirette");
		if(!debug_mode) {
			if(get_robot()->get_robot()->get_communication_protocol_type() == typeid(Communication::protocol_local)) {
				// Attente en mode simu
				std::cout << "Local mode: press a key." << std::endl;
				getchar();
			} else {
				wait_for_tirette();
			}
		}
	}

	void IAOneRobot::execute() {
		if(!debug_mode) {
			_points_printer = std::thread([this]() {
				AsciiArt art(std::cout);

				while(true) {
					art.print_chocobot();
					sleep(2_s);
					art.print_number(static_cast<std::size_t>(get_points()));
					sleep(2_s);
					art.print_string("INSA", AsciiArt::COLOR_RED, AsciiArt::COLOR_RED);
					sleep(2_s);
				}
			});
		}

		launch_petrilab(debug_mode);
	}

	void IAOneRobot::funny_action() {
		logDebug0("Funny action here for the robot ", name, "!");
	}

	ActionResult IAOneRobot::launch_petrilab(bool debug) {
		if(debug) {
			Petri::DebugServer debug_session(*_petrilab);
			debug_session.start();
			debug_session.join();
		} else {
			auto ia = _petrilab->createPetriNet();
			ia->run();

			// Garde fou pour les 100s de match
			while(get_left_time() > 0_ms) {
				sleep(50_ms);
			}

			ia->stop();
		}
		return ActionResult::SUCCESS;
	}
} // namespace Strategy
