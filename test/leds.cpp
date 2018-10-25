//
// Created by terae on 25/10/18.
//

#include "../src/robot/Robot.h"

using namespace PhysicalRobot;

int main() {
	auto m = std::make_shared<ModuleManager>();
	auto& led = m->add_module<LED2019>(4);

	Robot robot(m, {"prog_servos", "RS232", "/dev/ttyACM0"});

	bool on_off = true;
	for(;;) {
		led.set_state(on_off);
		on_off = !on_off;
		sleep(1_s);
	}

	return EXIT_SUCCESS;
}
