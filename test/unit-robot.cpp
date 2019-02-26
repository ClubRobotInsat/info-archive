//
// Created by terae on 17/09/18.
//

#include "catch.hpp"

#include "../src/robot/Robot.h"
#include <Constants.h>

TEST_CASE("Initialization of a common robot.", "[integration]") {
	SECTION("ModuleManager initialization") {
		auto m = std::make_shared<PhysicalRobot::ModuleManager>();

		auto& servos = m->add_module<PhysicalRobot::Servos>(2);
		servos.add_servo(5);
		servos.add_servo(6, PhysicalRobot::BlockingMode::HoldOnBlock);
		servos.set_position(6, 50_deg);

		m->add_module<PhysicalRobot::IO>(5);

		CHECK(m->get_nb_modules() == 2);

		PhysicalRobot::Robot robot(m, std::vector{"NULL"s}, Lidar::None);

		REQUIRE(robot.name == "guest");
		CHECK(robot.get_module<PhysicalRobot::Servos>().get_nbr_servos() == 2);

		std::map<uint8_t, std::string> list_modules = {{2, "Servos"}, {5, "IO"}};
		CHECK(robot.get_list_modules() == list_modules);

		robot.deactivation();
	}

	SECTION(".ini file initialization") {
		PhysicalRobot::Robot robot("primary", {"NULL"s}, Lidar::None);
		CHECK(robot.get_module<PhysicalRobot::Servos>().get_id() == 2);
		CHECK(robot.get_module<PhysicalRobot::IO>().get_id() == 4);
	}
}
