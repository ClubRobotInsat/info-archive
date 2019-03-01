//
// Created by abdelsaidt on 11/22/18.
//

#include "catch.hpp"

#include <Commun.h>

// Permets de récupérer des méthodes de conversion de la classe `Servos` normalement privées
#define TEST_SERVOS
#include "../src/robot/Modules/Servos.h"
#undef TEST_SERVOS

using namespace PhysicalRobot;

// Tests : cf [la datasheet](https://www.robotshop.com/media/files/pdf/manual-drs-0101.pdf) page 55

TEST_CASE("Conversions 'uint16_t <-> Angle'", "[integration]") {
	SECTION("uint16_t -> Angle") {
		const double eps = .001;

		CHECK((Servos::uint16t_to_angle(1023) - 166.650_deg).toDeg() < eps);
		CHECK((Servos::uint16t_to_angle(0) + 166.650_deg).toDeg() < eps);
		CHECK((Servos::uint16t_to_angle(546) - 11.240_deg).toDeg() < eps);
	}

	SECTION("Angle -> uint16_t") {
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

// cf [la datasheet](https://www.robotshop.com/media/files/pdf/manual-drs-0101.pdf) page 9 : 60°/0.166s

TEST_CASE("Conversions 'uint16_t <-> AngularSpeed'", "[integration]") {
	SECTION("uint16_t -> AngularSpeed") {
		double eps = .1;

		CHECK(std::abs((Servos::uint16_t_to_angular_speed(1019) - 360_deg_s).toDeg_s()) < eps);
		CHECK(std::abs((Servos::uint16_t_to_angular_speed(255) - 90_deg_s).toDeg_s()) < eps);
		CHECK(std::abs((Servos::uint16_t_to_angular_speed(510) - 180.2_deg_s).toDeg_s()) < eps);
	}

	SECTION("AngularSpeed -> uint16_t") {
		CHECK(Servos::angular_speed_to_uint16_t(360_deg_s) == 1018);
		CHECK(Servos::angular_speed_to_uint16_t(0_deg_s) == 0);
		CHECK(Servos::angular_speed_to_uint16_t(180_deg_s) == 509);
		CHECK(Servos::angular_speed_to_uint16_t(180.3_deg_s) == 510);
	}
}
