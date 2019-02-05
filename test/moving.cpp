
#include "../src/robot/Robot.h"
#include "json.hpp"

using namespace PhysicalRobot;

int main() {
	auto m = std::make_shared<ModuleManager>();

	// TODO check ID
	auto& moving = m->add_module<Navigation>(2);

	// TODO check address
	Robot robot(m, {"prog_moving", "UDP", "192.168.0.222", "50000", "51"}, Lidar::None);
	std::cout << "forward(5_cm)" << std::endl;
	moving.forward(5_cm);

	std::cout << "stop" << std::endl;
	moving.stop();

	robot.deactivation();

	logInfo("End of the sequence test.");

	return EXIT_SUCCESS;
}
