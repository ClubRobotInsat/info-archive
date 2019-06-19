#include "IAParsing.h"

#include <getopt.h>

#include <log/Log.h>

namespace Strategy {

	IAArguments::IAArguments()
	        : _debug_mode{false}, _should_exit{false}, _color{Constants::RobotColor::Yellow}, _simulator{false} {};

	IAArguments::IAArguments(int argc, char** argv) : IAArguments() {
		int arg;
		static struct option long_options[] = {{"color", required_argument, 0, 'c'},
		                                       {"debug", no_argument, 0, 'd'},
		                                       {"simu", no_argument, 0, 's'},
		                                       {"help", no_argument, 0, 'h'},
		                                       {0, 0, 0, 0}};
		_color = Constants::RobotColor::Undef;

		int long_index = 0;
		while((arg = getopt_long(argc, argv, "c:sh", long_options, &long_index)) != -1) {
			switch(arg) {
				case 'c':
					_color = Constants::string_to_color(std::string(optarg));
					break;
				case 's':
					_simulator = true;
					break;
				case 'd':
					_debug_mode = true;
					break;
				case 'h':
				default:
					print_help_and_exit();
					break;
			}
		}

		if(_color == Constants::RobotColor::Undef) {
			logError("Wrong color");
			print_help_and_exit();
		}
	}

	void IAArguments::print_help_and_exit() {
		std::cout << "Usage: --color [purple|yellow] [--simu] [--debug]" << std::endl;
		_should_exit = true;
	}

	void IAArguments::print_parsed_arguments() const {
		logDebug("Robot color is ", toString(_color));
		logDebug(_simulator ? "Connecting to the simulator" : "Connecting to the robot");
	}

} // namespace Strategy
