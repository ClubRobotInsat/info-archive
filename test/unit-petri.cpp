//
// Created by terae on 07/03/19.
//

#include "TestUtils.h"
#include "catch.hpp"

TEST_CASE("PetriLab") {
	SECTION("Initialization") {
		TestUtils::StrategyTest s;
		s.debug_init();
		REQUIRE(s.get_robot_names().size() == 1);
	}
}