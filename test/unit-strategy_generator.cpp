//
// Created by terae on 4/6/18.
//

#include "../src/robot/Principal/librobot/StrategyGenerator/MagicStrategy.h"
#include "catch.hpp"

TEST_CASE("StrategyGenerator") {
	using namespace StrategyGenerator;
	SECTION("DecisionalTree") {}

	SECTION("Action") {
		SECTION("get_execution_time") {
			auto generator = [](Duration execution_time, Vector2m start_position) -> Action {
				return Action(execution_time, 0, start_position, 0_deg, {} /*, nullptr*/);
			};

			CHECK(generator(20_s, {0_m, 0_m}).get_execution_time({1.2_m, 0_m}) == 28_s);
		}
	}

	SECTION("Element") {}

	SECTION("MagicStrategy") {}

	SECTION("Table") {}
}