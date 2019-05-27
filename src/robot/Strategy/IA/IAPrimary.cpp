//
// Created by terae on 13/02/19.
//

#include "IAPrimary.h"
#include "../PetriLab/src/Primary.h"
#include <AsciiArt.hpp>
#include <petrilab/Cpp/PetriLab.h>

int main(int argc, char* argv[]) {
	AsciiArt art(std::cout);
	art.print_patatohm();

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

	IAPrimary::IAPrimary(const IAArguments& args) : IAOneRobot(args, "primary") {
		init_strategy();
	}

	std::unique_ptr<Petri::PetriDynamicLib> IAPrimary::generate_petrilab_lib() {
		return Petri::Generated::Primary::createLib(".");
	}

	void IAPrimary::funny_action() {
		logDebug0("Funny action here for the primary robot!");
	}

} // namespace Strategy
