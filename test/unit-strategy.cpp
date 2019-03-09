//
// Created by terae on 24/01/19.
//

#include "../src/robot/Strategy/AbstractStrategy.h"
#include "TestUtils.h"
#include "catch.hpp"

TEST_CASE("Strategy", "[integration]") {
	SECTION("Robot color") {
		using Constants::RobotColor;

		TestUtils::StrategyTest s1(RobotColor::Yellow);
		CHECK(s1.get_color() == RobotColor::Yellow);
		CHECK(s1.get_reference() == GLOBAL_CONSTANTS().get_reference(RobotColor::Yellow));

		TestUtils::StrategyTest s2(RobotColor::Purple);
		CHECK(s2.get_color() == RobotColor::Purple);
		CHECK(s2.get_reference() == GLOBAL_CONSTANTS().get_reference(RobotColor::Purple));

		CHECK_THROWS_WITH(TestUtils::StrategyTest(RobotColor::Undef), "The strategy color is undefined.");
	}

	SECTION("Points") {
		TestUtils::StrategyTest s;
		CHECK(s.get_points() == 0);

		CHECK(s.add_points(15) == 15);
		CHECK(s.get_points() == 15);

		CHECK(s.set_points(50) == 50);
		CHECK(s.get_points() == 50);
	}

	SECTION("Interfacer::RobotManager") {
		TestUtils::StrategyTest s;
		CHECK(s.get_robot_names().empty());

		auto modules = std::make_shared<PhysicalRobot::ModuleManager>();
		modules->add_module<TestUtils::ModuleTest>(0);
		auto r1 = s.add_robot(std::make_shared<PhysicalRobot::Robot>(modules, std::vector({"NULL"s}), Lidar::None));
		REQUIRE_FALSE(modules->get_module<TestUtils::ModuleTest>().is_deactivation_called());
		s.stop();
		REQUIRE(modules->get_module<TestUtils::ModuleTest>().is_deactivation_called());
		CHECK(s.get_robot_names().size() == 1);

		const std::string NAME = "primary";
		auto r2 = s.add_robot(std::make_shared<PhysicalRobot::Robot>(NAME, std::vector({"NULL"s}), Lidar::None));
		REQUIRE(s.get_robot_names().size() == 2);
		CHECK(s.get_robot_names()[1] == NAME);
		CHECK(r2 == s.get_robot(NAME));
	}

	SECTION("Time") {
		StopWatch chrono;
		TestUtils::StrategyTest s;
		sleep(100_ms);
		CHECK(chrono.getElapsedTime() - s.get_time() < 20_us);

		chrono.reset();
		s.start(1_s);
		auto duration = chrono.getElapsedTime();
		CHECK(duration > 1_s);
		CHECK(duration < 1.1_s + 20_us);
		CHECK(s.is_execute_called());
	}
}
