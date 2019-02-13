//
// Created by terae on 13/02/19.
//

#include "IAPrimary.h"
#include "../PetriLab/src/Template.h"
#include <AsciiArt.hpp>
#include <petrilab/Cpp/PetriLab.h>

int main(int argc, char* argv[]) {
	AsciiArt art(std::cout);
	art.print_chocobot();

	Log::open(argc, argv, false);

	Strategy::IAPrimary strategy(Constants::RobotColor::Yellow);

	strategy.start(GLOBAL_CONSTANTS().get_match_duration());

	return EXIT_SUCCESS;
}

namespace Strategy {
	IAPrimary::IAPrimary(Constants::RobotColor color) : AbstractStrategy(color), name("primary") {
		if(debug_mode) {
			logInfo("Initialization of the physical robot '", name, "'");
		}

		auto physical_robot =
		    std::make_shared<PhysicalRobot::Robot>(name, std::vector({"IAPrimary"s, "ETERNET"s, "1"s, "192.168.0.222"s, "50000"s, "51"s}));
		add_robot(physical_robot);

		// TODO: create the 'IAPrimary' network
		_petrilab = Petri::Generated::Template::createLib(".");
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

	std::string IAPrimary::get_robot_name() const {
		return name;
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
