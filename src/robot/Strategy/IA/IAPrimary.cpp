//
// Created by terae on 13/02/19.
//

#include "IAPrimary.h"
#include "../PetriLab/src/Primary.h"
#include <AsciiArt.hpp>
#include <petrilab/Cpp/PetriLab.h>

int main(int argc, char* argv[]) {
	AsciiArt art(std::cout);
	art.print_chocobot();

	Log::open(argc, argv, false);
	Strategy::IAArguments args(argc, argv);

	if(args.should_exit()) {
		exit(1);
	}

	Strategy::IAPrimary strategy(args);

	strategy.start(strategy.debug_mode ? 1_h : GLOBAL_CONSTANTS().get_match_duration());

	return EXIT_SUCCESS;
}

namespace Strategy {
	IAPrimary::IAPrimary(const IAArguments& args) : AbstractStrategy(args.get_color(), "primary") {
		if(debug_mode) {
			logInfo("Initialization of the physical robot '", name, "'");
			args.print_parsed_arguments();
		}

		std::shared_ptr<PhysicalRobot::Robot> physical_robot;

		if(args.is_running_on_simulator()) {
			physical_robot =
			    std::make_shared<PhysicalRobot::Robot>(name, std::vector({"IAPrimary"s, "UDP"s, "localhost"s, "5001"s, "5101"s}), true);
		} else {
			physical_robot = std::make_shared<PhysicalRobot::Robot>(name, true);
		}
		add_robot(physical_robot);

		_petrilab = Petri::Generated::Primary::createLib(".");
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

	std::shared_ptr<Interfacer::RobotManager> IAPrimary::get_robot() const {
		return _interfacers.find(name)->second;
	}

	void IAPrimary::execute() {
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

	void IAPrimary::funny_action() {
		logDebug0("Funny action here!");
	}

	ActionResult IAPrimary::launch_petrilab(bool debug) {
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
