
#include "../../src/robot/Robot.h"
#include "json.hpp"

using namespace PhysicalRobot;

int main() {
	auto m = std::make_shared<ModuleManager>();

	// TODO check ID
	auto& navigation = m->add_module<Navigation>(1);

	// TODO check address
	Robot robot(m, {"prog_moving", "ETHERNET", "1", "192.168.1.1", "5001", "51"}, Lidar::None);

	std::cout << "forward(10_cm)" << std::endl;
	navigation.forward(10_cm, SensAdvance::Forward);

	sleep(1_s);
	std::cout << navigation.get_coordinates() << std::endl;

	getchar();
	std::cout << "backward(10_cm)" << std::endl;
	// navigation.backward(10_cm);
	navigation.turn_absolute(20_deg, SensRotation::Trigo);
	sleep(1_s);

	std::cout << "Is the robot blocked ? " << navigation.is_physically_blocked() << std::endl;
	std::cout << "stop" << std::endl;
	navigation.stop();

	getchar();

	robot.deactivation();

	logInfo("End of the sequence test.");

	return EXIT_SUCCESS;
}
