//
// Created by terae on 24/01/19.
//

#include "../src/robot/Strategy/AbstractStrategy.h"
#include "catch.hpp"

static bool strategy_has_run;
static bool deactivation_called;

namespace Strat {
	class StrategyTest final : public Strategy::AbstractStrategy {
	public:
		StrategyTest(Constants::RobotColor color = Constants::RobotColor::Yellow) : AbstractStrategy(color) {}

	private:
		void execute() override {
			strategy_has_run = true;
		}
	};

	class ModuleTest final : public PhysicalRobot::Module {
	public:
		explicit ModuleTest(uint8_t id) : Module(id, "Test") {}

		void deactivation() override {
			deactivation_called = true;
		}

	private:
		std::vector<JSON> generate_list_jsons() const override {
			return {};
		}

		void message_processing(const JSON&) override {}
	};
} // namespace Strat

TEST_CASE("Strategy", "[integration]") {
	SECTION("Robot color") {
		using Constants::RobotColor;

		Strat::StrategyTest s1(RobotColor::Yellow);
		CHECK(s1.get_color() == RobotColor::Yellow);
		CHECK(s1.get_reference() == GLOBAL_CONSTANTS().get_reference(RobotColor::Yellow));

		Strat::StrategyTest s2(RobotColor::Purple);
		CHECK(s2.get_color() == RobotColor::Purple);
		CHECK(s2.get_reference() == GLOBAL_CONSTANTS().get_reference(RobotColor::Purple));

		CHECK_THROWS_WITH(Strat::StrategyTest(RobotColor::Undef), "The strategy color is undefined.");
	}

	SECTION("Points") {
		Strat::StrategyTest s;
		CHECK(s.get_points() == 0);

		CHECK(s.add_points(15) == 15);
		CHECK(s.get_points() == 15);

		CHECK(s.set_points(50) == 50);
		CHECK(s.get_points() == 50);
	}

	SECTION("Interfacer::RobotManager") {
		Strat::StrategyTest s;
		CHECK(s.get_robot_names().empty());

		auto modules = std::make_shared<PhysicalRobot::ModuleManager>();
		modules->add_module<Strat::ModuleTest>(0);
		auto r1 = s.add_robot(std::make_shared<PhysicalRobot::Robot>(modules, std::vector({"NULL"s}), Lidar::None));
		deactivation_called = false;
		s.stop();
		REQUIRE(deactivation_called);
		CHECK(s.get_robot_names().size() == 1);

		const std::string NAME = "primary";
		auto r2 = s.add_robot(std::make_shared<PhysicalRobot::Robot>(NAME, std::vector({"NULL"s}), Lidar::None));
		REQUIRE(s.get_robot_names().size() == 2);
		CHECK(s.get_robot_names()[1] == NAME);
		CHECK(r2 == s.get_robot(NAME));
	}

	SECTION("Time") {
		StopWatch chrono;
		Strat::StrategyTest s;
		sleep(100_ms);
		CHECK(chrono.getElapsedTime() - s.get_time() < 20_us);

		strategy_has_run = false;
		chrono.reset();
		s.start(1_s);
		auto duration = chrono.getElapsedTime();
		CHECK(duration > 1_s);
		CHECK(duration < 1.1_s + 20_us);
		CHECK(strategy_has_run);
	}
}
