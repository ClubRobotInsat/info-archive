//
// Created by terae on 24/01/19.
//

#include "../src/robot/Strategy/ModuleInterfacers/GlobalManager.h"
#include "catch.hpp"

TEST_CASE("Interfacers") {
	using namespace Strategy::Interfacer;

	SECTION("ServosManager") {
		/// Removes a necessary `logWarn` from terminal without disturbing the `catch` outputs
		Log::open(Log::NOTHING, "", false);
		REQUIRE(std::is_same<ServosManager::interfaced_type, PhysicalRobot::Servos>::value);
		REQUIRE(std::is_same<ServosManager::servo_t, uint8_t>::value);

		/*auto get_servo_position = [](ServosManager::servo_t id, uint8_t value) -> Angle {
		    return Angle::makeFromDeg(10 * (id + value));
		};*/

		PhysicalRobot::Servos servos(5);
		ServosManager interfacer1(servos /*, get_servo_position*/);
		CHECK(interfacer1->get_id() == 5);

		auto manager = std::make_shared<PhysicalRobot::ModuleManager>();
		auto& s = manager->add_module<PhysicalRobot::Servos>(6);
		s.add_servo(42);
		auto robot = std::make_shared<PhysicalRobot::Robot>(manager, std::vector<std::string>({"ehCoucou", "NULL"}), Lidar::None);
		ServosManager interfacer2(robot /*, get_servo_position*/);
		CHECK(interfacer2->get_nbr_servos() == 1);
		CHECK(interfacer2.set_position(42, 50_deg) == ActionResult::TIMEOUT);
		interfacer2.set_offset(42, 18_deg);
		CHECK(interfacer2.get_offset(42) == 18_deg);
		ServosManager interfacer3(*robot /*, get_servo_position*/);
		CHECK(interfacer3->get_nbr_servos() == 1);
		Log::close(Log::NOTHING);
	}

	SECTION("Avoidance") {
		REQUIRE(std::is_same<Avoidance::interfaced_type, void>::value);

		auto manager = std::make_shared<PhysicalRobot::ModuleManager>();
		auto robot = std::make_shared<PhysicalRobot::Robot>(manager, std::vector<std::string>({"ehCoucou", "NULL"}), Lidar::None);

		Distance sx = 40_cm;
		Distance sy = 30_cm;
		Environment env({300, 200}, 1_cm, sy, (sqrt(sx * sx + sy * sy) / 2) * 1.2, Vector2m(0_m, 1_m));

		Avoidance avoidance2(robot, env);
		CHECK(avoidance2.get_adversary_positions().empty());
	}

	SECTION("GlobalManager") {
		auto module_manager = std::make_shared<PhysicalRobot::ModuleManager>();
		auto robot =
		    std::make_shared<PhysicalRobot::Robot>(module_manager, std::vector<std::string>({"ehCoucou", "NULL"}), Lidar::None);
		GlobalManager manager(robot);
		REQUIRE(manager.get_robot() == robot);

		CHECK_FALSE(manager.has_interfacer<ServosManager>());
		REQUIRE_THROWS_WITH(manager.get_interfacer<ServosManager>(), "The interfacer doesn't exist.");

		REQUIRE_THROWS_WITH(manager.add_interfacer<ServosManager>(), "The module doesn't exist.");
		module_manager->add_module<PhysicalRobot::Servos>(5);
		REQUIRE_NOTHROW(manager.add_interfacer<ServosManager>());
		REQUIRE_NOTHROW(manager.get_interfacer<ServosManager>());
		CHECK(manager.has_interfacer<ServosManager>());

		auto& servos = manager.get_interfacer<ServosManager>();
		REQUIRE_NOTHROW(servos->add_servo(8));

		Distance sx = 40_cm;
		Distance sy = 30_cm;
		Environment env({300, 200}, 1_cm, sy, (sqrt(sx * sx + sy * sy) / 2) * 1.2, Vector2m(0_m, 1_m));
		REQUIRE_NOTHROW(manager.add_interfacer<Avoidance>(env));

		CHECK(manager.size() == 2);
	}
}
