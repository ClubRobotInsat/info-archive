//
// Created by terae on 07/03/19.
//

#include "PetriSample.h"
#include "TestUtils.h"
#include "catch.hpp"

TEST_CASE("PetriLab") {
	Log::open(Log::Output::NOTHING, "", false);
	SECTION("Initialization") {
		TestUtils::StrategyTest s;
		s.debug_init();
		REQUIRE(s.get_robot_names().size() == 1);
	}

	SECTION("Execution") {
		TestUtils::StrategyTest s;
		s.debug_init();

		REQUIRE_FALSE(s.is_execute_called());
		StopWatch chrono;
		s.start(1.5_s);
		Duration time_stamp = chrono.getElapsedTime();
		CHECK(time_stamp >= 1.5_s);
		CHECK(time_stamp <= 1.6_s);
		REQUIRE(s.is_execute_called());
		// L'action a  bien été appelée dans PetriLab
		CHECK(test_bool());
	}
	Log::close(Log::Output::NOTHING);
}
