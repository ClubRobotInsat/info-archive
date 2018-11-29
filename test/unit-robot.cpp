//
// Created by terae on 17/09/18.
//

#include "catch.hpp"

#include "../src/robot/Robot.h"
#include <Constants.h>

TEST_CASE("Initialization of a common robot.") {
	SECTION("ModuleManager initialization") {
		auto m = std::make_shared<PhysicalRobot::ModuleManager>();

		auto& servos = m->add_module<PhysicalRobot::Servos>(2);
		servos.add_servo(5);
		servos.add_servo(6, PhysicalRobot::Servos::BlockingMode::HOLD_ON_BLOCKING);
		servos.set_position(6, 50_deg);

		m->add_module<PhysicalRobot::IO>(5);

		CHECK(m->get_nb_modules() == 2);

		PhysicalRobot::Robot robot(m, {"ehCoucou", "NULL"});

		REQUIRE(robot.name == "guest");
		CHECK(robot.get_module<PhysicalRobot::Servos>().get_nbr_servos() == 2);

		robot.deactivation();
	}

	SECTION(".ini file initialization") {
		PhysicalRobot::Robot robot("primary", {"ehCoucou", "NULL"});
		CHECK(robot.get_module<PhysicalRobot::Servos>().get_id() == 2);
		CHECK(robot.get_module<PhysicalRobot::IO>().get_id() == 5);
	}
}
