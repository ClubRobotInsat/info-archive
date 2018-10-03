//
// Created by terae on 17/09/18.
//

#include "catch.hpp"

#include "../src/robot/Robot.h"
#include <Constants.h>

TEST_CASE("Initialization of a common robot.") {
	auto m = std::make_shared<Commun::ModuleManager>();

	auto& servos = m->add_module<Commun::Servos2019>(2);
	servos.add_servo(5, 120_deg);
	servos.add_servo(6, 50_deg, Commun::Servos2019::BlockingMode::HOLD_ON_BLOCKING);

	Commun::Robot robot(m, {"ehCoucou", "NULL"});

	CHECK(robot.get_module<Commun::Servos2019>().get_nbr_servos() == 2);

	// robot.get_communicator().disconnect();

	SECTION("Robot Principal") {
		Commun::Robot r{"primary", {"ehCoucou", "NULL"}};
	}
}
