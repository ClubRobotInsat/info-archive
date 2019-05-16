
#include "../../src/robot/Modules/NavigationParameters.h"
#include "../../src/robot/Robot.h"
#include "../../src/robot/Strategy/ModuleInterfacers/NavigationInterfacer.h"
#include "../../src/robot/Strategy/ModuleInterfacers/RobotManager.h"
#include "json.hpp"

using namespace PhysicalRobot;
using namespace Strategy::Interfacer;

int main(int argc, char** argv) {
	std::string target("simu");
	std::string action("move_to");
	if(argc > 1) {
		target = argv[1];
	}
	if(argc > 2) {
		action = argv[2];
	}

	auto& constants = GLOBAL_CONSTANTS()["primary"];


	// Initialization

	auto m = std::make_shared<ModuleManager>();
	auto& nav_module = m->add_module<Navigation>(1);

	if(action == "parameters") {
		m->add_module<NavigationParameters>(10);
	}

	std::shared_ptr<Robot> robot;

	logDebug4("Target: ", target);
	if(target == "simu") {
		robot = std::make_shared<Robot>(m, std::vector<std::string>{"prog_moving", "UDP", "localhost", "5001", "5101"}, Lidar::None);
	} else if(target == "robot") {
		robot = std::make_shared<Robot>(m,
		                                std::vector<std::string>{"prog_moving", "ETHERNET", "1", "192.168.1.1", "5001", "51"},
		                                Lidar::None);
	} else {
		std::cout << "Available targets: simu, robot" << std::endl;
		exit(-1);
	}
	RobotManager manager(robot);

	Distance sx = 40_cm;
	Distance sy = 30_cm;
	Environment env({300, 200}, 1_cm, sy, (sqrt(sx * sx + sy * sy) / 2) * 1.2, Vector2m(0_m, 1_m));
	env.loadFromJSON(GLOBAL_CONSTANTS().TABLE_2018());

	auto& avoidance = manager.add_interfacer<AvoidanceInterfacer>(env);
	auto& nav_interfacer = manager.add_interfacer<NavigationInterfacer>(env, avoidance);


	// Tests

	ActionResult result;
	logDebug4("Testing ", action);

	if(action == "module") {
		std::cout << "[TODO] This test uses directly the Navigation module" << std::endl;
	} else if(action == "parameters") {
		std::cout << "-> This test uses the NavigationParameters module" << std::endl
		          << "(type 'q' to quit)" << std::endl;
		auto& nav_params = m->get_module<NavigationParameters>();

		int c = getchar();
		nav_params.set_inter_axial_length(10_cm);

		while(c != 'q') {
			std::cout << "robot position " << nav_module.get_coordinates() << std::endl;
			c = getchar();
		}

		nav_params.set_inter_axial_length(10_cm);
	} else if(action == "interfacer") {
		std::cout << "forward(10_cm)?" << std::endl;
		getchar();
		result = nav_interfacer.forward(10_cm, SensAdvance::Forward);
		std::cout << "Result: " << result << ", coordinates: " << nav_module.get_coordinates() << std::endl;

		std::cout << "turn absolute(20_deg)?" << std::endl;
		getchar();
		nav_interfacer.turn_absolute(20_deg);
		std::cout << "Result: " << result << ", coordinates: " << nav_module.get_coordinates() << std::endl;
	} else if(action == "move_to") {
		Vector2m startPos{constants.get_start_position().x, constants.get_start_position().y};
		Angle startAngle = constants.get_start_angle();
		nav_module.set_coordinates(repere::Coordinates{startPos, startAngle});
		std::cout << "Set coordinates to " << startPos << ", " << startAngle.toDeg() << " deg" << std::endl;

		Vector2m finalPos{2_m, 1_m};
		Angle finalAngle = 0_deg;
		std::cout << "Move to " << finalPos << ", " << finalAngle.toDeg() << " deg?" << std::endl;
		getchar();
		result = nav_interfacer.move_to(repere::Coordinates{finalPos, finalAngle});
		std::cout << "Result: " << result << ", coordinates: " << nav_module.get_coordinates() << std::endl;
	} else {
		std::cout << "Test one of: move_to, interfacer, module, parameters" << std::endl;
	}

	robot->deactivation();

	logInfo("End of the sequence test.");

	return EXIT_SUCCESS;
}
