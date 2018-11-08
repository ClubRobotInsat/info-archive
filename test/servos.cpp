//
// Created by terae on 14/10/18.
//

#include "../src/robot/Robot.h"

using namespace PhysicalRobot;

int main() {
	const uint8_t ID_SERVO = 254;
	auto m = std::make_shared<ModuleManager>();

	auto& servos = m->add_module<Servos>(5);
	servos.add_servo(ID_SERVO, 0_deg);
	servos.set_color(ID_SERVO, Servos::CYAN);

	Robot robot(m, {"prog_servos", "RS232", "/dev/ttyACM0"});
	for(int count = 0; count < 10; count++) {
		const Angle command = count * 15_deg;
		std::cout << "set_position_servo_254(" << command << ")." << std::endl;
		servos.set_position(ID_SERVO, command);
		sleep(1_s);
	}

	return EXIT_SUCCESS;
}
