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
	IATest::IATest() : AbstractStrategy(Constants::RobotColor::Purple), _id_servo(254) {
		logInfo("Initialization of the test robot");

		_module_manager = std::make_shared<PhysicalRobot::ModuleManager>();
		auto& servos = _module_manager->add_module<PhysicalRobot::Servos>(2);
		auto& io = _module_manager->add_module<PhysicalRobot::IO>(4);
		auto& pumps = _module_manager->add_module<PhysicalRobot::Pumps>(5);

		servos.add_servo(_id_servo);
		// servos.add_servo(2);
		// servos.add_servo(1);

		std::vector<std::string> communicator_arguments{
		    "ETHERNET", "2", "192.168.1.2", "5002", "52", "4", "192.168.1.4", "5004", "54", "5", "192.168.1.4", "5005", "55"};

		auto physical_robot =
		    std::make_shared<PhysicalRobot::Robot>(_module_manager, communicator_arguments, Lidar::LidarType::None, true);
		_robot_manager = add_robot(physical_robot);

		wait_for_tirette();
	}

	void IATest::execute() {
		auto& io = _robot_manager->get_interfacer<Interfacer::IOInterfacer>();

		auto& pumps = _module_manager->get_module<PhysicalRobot::Pumps>();
		// auto& interfacer_pumps = _robot_manager->get_interfacer<Interfacer::PumpsInterfacer>();


		std::cout << "###########################\n## Test of pumps and valves" << std::endl;
		for(PhysicalRobot::Pumps::valve_t i = 0; i < 6; ++i) {
			pumps.activate_valve(i);
			sleep(1_s);
		}
		sleep(1_s);
		pumps.activate_pump(0);
		pumps.activate_pump(1);
		io.reset_buzzer();
		sleep(100_ms);
		io.play_success_sound();
		std::cout << "all valves and pumps activated" << std::endl;

		sleep(2_s);
		for(int i = 0; i < 6; ++i) {
			pumps.deactivate_valve(i);
			sleep(1_s);
		}
		pumps.deactivate_pump(0);
		pumps.deactivate_pump(1);
		io.reset_buzzer();
		sleep(100_ms);
		io.play_error_sound();
		std::cout << "all objects deactivated" << std::endl;

		std::cout << "###########################\n## Test of servos" << std::endl;
		auto& servos = _robot_manager->get_interfacer<Interfacer::ServosInterfacer>();
		for(Angle angle = -200_deg; angle < 200_deg; angle += 20_deg) {
			servos.set_position(_id_servo, angle);
			sleep(20_ms);
		}
	}

	void IATest::funny_action() {
		logDebug0("Funny action here!");
	}
} // namespace Strategy
