//
// Created by abdelsaidt on 11/22/18.
//

#include "catch.hpp"

#include "../src/robot/Modules/Servos.h"
#include <Commun.h>

using namespace PhysicalRobot;

TEST_CASE("test angle from unit_t16") {

	double eps = .001;

	CHECK((Servos::uint16t_to_angle(1023) - 166.650_deg).toDeg() < eps);

	CHECK((Servos::uint16t_to_angle(0) + 166.650_deg).toDeg() < eps);

	CHECK((Servos::uint16t_to_angle(546) - 11.240_deg).toDeg() < eps);
}