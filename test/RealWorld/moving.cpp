
#include "../../src/robot/Robot.h"
#include "json.hpp"

using namespace PhysicalRobot;

int main() {
	auto m = std::make_shared<ModuleManager>();

	// TODO check ID
	auto& navigation = m->add_module<Navigation>(1);

	// TODO check address
	Robot robot(m, {"prog_moving", "UDP", "127.0.0.1", "5000", "1234"}, Lidar::None);

	std::cout << "forward(5_cm)" << std::endl;
	navigation.forward(5_cm, SensAdvance::Forward);

	getchar();
	std::cout << "forward(30_cm)" << std::endl;
	navigation.forward(30_cm, SensAdvance::Forward);

	getchar();
	std::cout << "Is the robot blocked ? " << navigation.is_physically_blocked() << std::endl;
	std::cout << "stop" << std::endl;
	navigation.stop();

	robot.deactivation();

	logInfo("End of the sequence test.");

	return EXIT_SUCCESS;
}
