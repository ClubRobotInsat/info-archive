//
// Created by terae on 09/05/19.
//

#include "IATest.h"
#include <AsciiArt.hpp>

int main(int argc, char* argv[]) {
	AsciiArt art(std::cout);
	art.print_chocobot();

	Log::open(argc, argv, false);

	Strategy::IATest strategy;

	strategy.start(1_h);

	return EXIT_SUCCESS;
}

namespace Strategy {
	IATest::IATest() : AbstractStrategy(Constants::RobotColor::Purple) {
		logInfo("Initialization of the test robot");

		auto module_manager = std::make_shared<PhysicalRobot::ModuleManager>();
		auto& servos = module_manager->add_module<PhysicalRobot::Servos>(2);
		module_manager->add_module<PhysicalRobot::IO>(4);
		module_manager->add_module<PhysicalRobot::Pumps>(5);

		servos.add_servo(2);

		std::vector<std::string> communicator_arguments{
		    "ETHERNET", "2", "192.168.1.2", "5002", "52", "4", "192.168.1.4", "5004", "54", "5", "192.168.1.4", "5005", "55"};

		auto physical_robot =
		    std::make_shared<PhysicalRobot::Robot>(module_manager, communicator_arguments, Lidar::LidarType::Any, true);
		add_robot(physical_robot);

		wait_for_tirette();
	}

	void IATest::execute() {
		forward_hand(50_deg);
	}

	void IATest::funny_action() {
		logDebug0("Funny action here!");
	}
} // namespace Strategy
