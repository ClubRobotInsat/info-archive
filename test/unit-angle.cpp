//
// Created by abdelsaidt on 11/22/18.
//

#include "catch.hpp"

#include <Commun.h>

#define TEST_SERVOS
#include "../src/robot/Modules/Servos.h"

using namespace PhysicalRobot;

// Tests : cf [la datasheet](https://www.robotshop.com/media/files/pdf/manual-drs-0101.pdf) page 55

TEST_CASE("uint16_t <-> Angle conversions") {

	SECTION("uint16_t to angle") {
		const double eps = .001;

		CHECK((Servos::uint16t_to_angle(1023) - 166.650_deg).toDeg() < eps);

		CHECK((Servos::uint16t_to_angle(0) + 166.650_deg).toDeg() < eps);

		CHECK((Servos::uint16t_to_angle(546) - 11.240_deg).toDeg() < eps);
	}

	SECTION("angle to uint16_t") {
		CHECK(Servos::angle_to_uint16t(-159.8_deg) == 21);
		CHECK(Servos::angle_to_uint16t(-159.4_deg) == 22);
		CHECK(Servos::angle_to_uint16t(-144.4_deg) == 68);
		CHECK(Servos::angle_to_uint16t(-68.8_deg) == 300);
		CHECK(Servos::angle_to_uint16t(-0.1_deg) == 511);
		CHECK(Servos::angle_to_uint16t(0.5_deg) == 512);
		CHECK(Servos::angle_to_uint16t(77_deg) == 747);
		CHECK(Servos::angle_to_uint16t(159.9_deg) == 1002);
	}
}