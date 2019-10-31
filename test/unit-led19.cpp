//
// Created by abdelsaidt on 10/10/19.
//

#include "../src/robot/Modules/LED.h"
#include "../src/robot/Robot.h"
#include "../src/robot/Strategy/ModuleInterfacers/RobotManager.h"
#include "catch.hpp"


TEST_CASE("Led test on/off", "[integration]") {

	auto manager = std::make_shared<PhysicalRobot::ModuleManager>();
	SECTION("Mode manual") {
		auto& ref_led = manager->add_module<PhysicalRobot::LED>(0);
		CHECK(ref_led.get_mode() == PhysicalRobot::LEDMode::Manual);
		PhysicalRobot::IOState init_state = ref_led.get_state();
		int odd_n = 7, even_n = 8, trg = 0;

		while(even_n-- > 0) {
			trg = 1 - trg;
			if(trg)
				ref_led.on();
			else
				ref_led.off();
		}
		CHECK(ref_led.get_state() == init_state);

		while(odd_n-- > 0) {
			trg = 1 - trg;
			if(trg)
				ref_led.on();
			else
				ref_led.off();
		}
		ref_led.trigger();
		CHECK(ref_led.get_state() == init_state);
		// Log::close(Log::NOTHING);
	}

	SECTION("Mode auto") {
		auto& ref_led = manager->add_module<PhysicalRobot::LED>(0, 1);
		CHECK(ref_led.get_mode() == PhysicalRobot::LEDMode::Auto);
		REQUIRE_NOTHROW(ref_led.start());
		std::cout << "LED started" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(8));
		REQUIRE_NOTHROW(ref_led.stop());
		std::cout << "LED stopped" << std::endl;
		// Log::close(Log::NOTHING);
	}
}