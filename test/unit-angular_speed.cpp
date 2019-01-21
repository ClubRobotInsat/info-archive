
//
// Created by abdelsaidt on 12/13/18.
//

#include "catch.hpp"

#include <Commun.h>

#define TEST_SERVOS
#include "../src/robot/Modules/Servos.h"

using namespace PhysicalRobot;

TEST_CASE("test angularspeed from unit_t16") {

	SECTION("uint16_t_to_angular_speed") {
		double eps = .1;

		CHECK(std::abs((Servos::uint16_t_to_angular_speed(1023) - 360_deg_s).toDeg_s()) < eps);

		CHECK(std::abs((Servos::uint16_t_to_angular_speed(256) - 90_deg_s).toDeg_s()) < eps);

		CHECK(std::abs((Servos::uint16_t_to_angular_speed(512) - 180.2_deg_s).toDeg_s()) < eps);
	}

	SECTION("angular_speed_to_uint16_t") {
		CHECK(Servos::angular_speed_to_uint16_t(360_deg_s) == 1023);

		CHECK(Servos::angular_speed_to_uint16_t(0_deg_s) == 0);

		CHECK(Servos::angular_speed_to_uint16_t(180_deg_s) == 511);

		CHECK(Servos::angular_speed_to_uint16_t(180.3_deg_s) == 512);
	}
}
