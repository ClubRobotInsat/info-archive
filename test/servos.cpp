//
// Created by terae on 14/10/18.
//

#include "../src/robot/Robot.h"

using namespace PhysicalRobot;

int main() {
	const uint8_t ID_SERVO = 254;
	auto m = std::make_shared<ModuleManager>();

	auto& servos = m->add_module<Servos2019>(8);
	servos.add_servo(ID_SERVO, 0_deg);
	servos.set_color(ID_SERVO, Servos2019::CYAN);

	Robot robot(m, {"prog_servos", "RS232", "/dev/ttyUSB0"});
	for(int count = 0; count < 10; count++) {
		servos.set_position(ID_SERVO, count * 15_deg);
		sleep(1_s);
	}

	return EXIT_SUCCESS;
}
