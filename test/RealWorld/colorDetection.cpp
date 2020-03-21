
#include "../../src/robot/Modules/NavigationParameters.h"
#include "../../src/robot/Robot.h"
#include "../../src/robot/Strategy/ModuleInterfacers/NavigationInterfacer.h"
#include "../../src/robot/Strategy/ModuleInterfacers/RobotManager.h"
#include "json.hpp"
#include <iostream>

using namespace PhysicalRobot;
using namespace Strategy::Interfacer;


int main(int argc, char** argv) {
	/*
	if(argc > 1) {
	    target = argv[1];
	}
	if(argc > 2) {
	    action = argv[2];
	}
	*/

	// Initialization

	auto m = std::make_shared<ModuleManager>();
	auto& webcam = m->add_module<Webcam>(1);

	/*std::shared_ptr<Robot> robot;

	robot = std::make_shared<Robot>(m,
	        std::vector<std::string>{"webcamProg", "ETHERNET", "1", "192.168.1.1", "5001", "51", "10", "192.168.1.1",
	"5010", "60"}, Lidar::None);

	RobotManager manager(robot);

	auto& avoidance = manager.add_interfacer<AvoidanceInterfacer>(env);
	auto& nav_interfacer = manager.add_interfacer<NavigationInterfacer>(env, avoidance);

	*/
	// Tests

	logDebug4("Testing ");
	char cont = 'y';
	do {
		std::cout << "Launch a detection ? y/n" << std::endl;
		std::cin >> cont;
		if(cont == 'y') {
			std::cout << toString(webcam.detectColor()) << std::endl;
		}

	} while(cont == 'y');

	// robot->deactivation();

	logInfo("End of the test sequence.");

	return EXIT_SUCCESS;
}
