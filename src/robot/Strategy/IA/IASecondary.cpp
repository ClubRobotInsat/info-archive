//
// Created by terae on 20/05/19.
//

#include "IASecondary.h"
#include "../PetriLab/src/Secondary.h"
#include <petrilab/Cpp/PetriLab.h>

int main(int argc, char* argv[]) {
	Log::open(argc, argv, false);
	Strategy::IAArguments args(argc, argv);

	if(args.should_exit()) {
		exit(1);
	}

	Strategy::IASecondary strategy(args);

	strategy.start(strategy.debug_mode ? 1_h : GLOBAL_CONSTANTS().get_match_duration());

	return EXIT_SUCCESS;
}

namespace Strategy {
	IASecondary::IASecondary(const IAArguments& args) : IAOneRobot(args, "secondary") {
		init_strategy();
	}

	std::unique_ptr<Petri::PetriDynamicLib> IASecondary::generate_petrilab_lib() {
		return Petri::Generated::Secondary::createLib(".");
	}

	void IASecondary::funny_action() {
		logDebug0("Funny action here for the secondary robot!");
	}

} // namespace Strategy
