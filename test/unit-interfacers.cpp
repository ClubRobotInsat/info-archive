//
// Created by terae on 24/01/19.
//

#include "../src/robot/Strategy/ModuleInterfacers/RobotManager.h"
#include "catch.hpp"

TEST_CASE("Interfacers", "[integration]") {
	using namespace Strategy::Interfacer;

	SECTION("ServosInterfacer") {
		/// Removes a necessary `logWarn` from terminal without disturbing the `catch` outputs
		Log::open(Log::NOTHING, "", false);
		REQUIRE(std::is_same<ServosInterfacer::interfaced_type, PhysicalRobot::Servos>::value);
		REQUIRE(std::is_same<ServosInterfacer::servo_t, uint8_t>::value);

		/*auto get_servo_position = [](ServosInterfacer::servo_t id, uint8_t value) -> Angle {
		    return Angle::makeFromDeg(10 * (id + value));
		};*/

		PhysicalRobot::Servos servos(5);
		ServosInterfacer interfacer1(servos /*, get_servo_position*/);
		CHECK(interfacer1->get_id() == 5);

		auto manager = std::make_shared<PhysicalRobot::ModuleManager>();
		auto& s = manager->add_module<PhysicalRobot::Servos>(6);
		s.add_servo(42);
		auto robot = std::make_shared<PhysicalRobot::Robot>(manager, std::vector{"NULL"s}, Lidar::None);
		ServosInterfacer interfacer2(robot /*, get_servo_position*/);
		CHECK(interfacer2->get_nbr_servos() == 1);
		CHECK(interfacer2.set_position(42, 50_deg) == ActionResult::TIMEOUT);
		interfacer2.set_offset(42, 18_deg);
		CHECK(interfacer2.get_offset(42) == 18_deg);
		ServosInterfacer interfacer3(*robot /*, get_servo_position*/);
		CHECK(interfacer3->get_nbr_servos() == 1);
		Log::close(Log::NOTHING);
	}

	SECTION("AvoidanceInterfacer") {
		REQUIRE(std::is_same<AvoidanceInterfacer::interfaced_type, void>::value);

		auto manager = std::make_shared<PhysicalRobot::ModuleManager>();
		auto& nav = manager->add_module<PhysicalRobot::Navigation>(1);
		auto robot = std::make_shared<PhysicalRobot::Robot>(manager, std::vector{"NULL"s}, Lidar::None);

		Distance sx = 40_cm;
		Distance sy = 30_cm;
		Environment env({300, 200}, 1_cm, sy, (sqrt(sx * sx + sy * sy) / 2) * 1.2, Vector2m(0_m, 1_m));

		AvoidanceInterfacer avoidance2(robot, env);
		CHECK(avoidance2.get_adversary_positions().empty());

		avoidance2.set_adversary_detection_angle(45_deg);
		nav.set_coordinates(repere::Coordinates({0.5_m, 1_m}, 135_deg));

		SECTION("Adversary in front") {
			avoidance2.add_fake_adversary_position({0.3_m, 1.3_m});
			CHECK(avoidance2.adversary_detected(50_cm, PhysicalRobot::SensAdvance::Forward));
			CHECK_FALSE(avoidance2.adversary_detected(50_cm, PhysicalRobot::SensAdvance::Backward));
		}

		SECTION("Adversary in back") {
			avoidance2.add_fake_adversary_position({0.8_m, 0.8_m});
			CHECK(avoidance2.adversary_detected(50_cm, PhysicalRobot::SensAdvance::Backward));
			CHECK_FALSE(avoidance2.adversary_detected(50_cm, PhysicalRobot::SensAdvance::Forward));
		}

		SECTION("Adversaries on sides") {
			avoidance2.add_fake_adversary_position({0.6_m, 1.3_m});
			avoidance2.add_fake_adversary_position({0.2_m, 0.9_m});
			CHECK_FALSE(avoidance2.adversary_detected(50_cm, PhysicalRobot::SensAdvance::Backward));
			CHECK_FALSE(avoidance2.adversary_detected(50_cm, PhysicalRobot::SensAdvance::Forward));
			CHECK(avoidance2.adversary_detected(50_cm));
		}
	}

	SECTION("NavigationInterfacer") {
		REQUIRE(std::is_same<NavigationInterfacer::interfaced_type, PhysicalRobot::Navigation>::value);

		auto manager = std::make_shared<PhysicalRobot::ModuleManager>();
		manager->add_module<PhysicalRobot::Navigation>(2);
		auto robot = std::make_shared<PhysicalRobot::Robot>(manager, std::vector{"NULL"s}, Lidar::None);

		Distance sx = 40_cm;
		Distance sy = 30_cm;
		Environment env({300, 200}, 1_cm, sy, (sqrt(sx * sx + sy * sy) / 2) * 1.2, Vector2m(0_m, 1_m));
		AvoidanceInterfacer avoidance(robot, env);

		NavigationInterfacer navigation(robot, env, avoidance);
		CHECK(navigation.get_linear_speed() == GLOBAL_CONSTANTS()["default"].get_linear_speed());
		CHECK(navigation.get_angular_speed() == GLOBAL_CONSTANTS()["default"].get_angular_speed());
		CHECK(navigation.get_linear_accuracy() == GLOBAL_CONSTANTS()["default"].get_linear_accuracy());
		CHECK(navigation.get_angular_accuracy() == GLOBAL_CONSTANTS()["default"].get_angular_accuracy());

		CHECK(navigation.optimal_rotation_sens({0_deg}, {179_deg}) == PhysicalRobot::SensRotation::Trigo);
		CHECK(navigation.optimal_rotation_sens({0_deg}, {-179_deg}) == PhysicalRobot::SensRotation::Clockwise);
	}

	SECTION("RobotManager") {
		auto module_manager = std::make_shared<PhysicalRobot::ModuleManager>();
		auto robot = std::make_shared<PhysicalRobot::Robot>(module_manager, std::vector{"NULL"s}, Lidar::None);
		RobotManager manager(robot);
		REQUIRE(manager.get_robot() == robot);

		CHECK_FALSE(manager.has_interfacer<ServosInterfacer>());
		REQUIRE_THROWS_WITH(manager.get_interfacer<ServosInterfacer>(), "The interfacer doesn't exist.");

		REQUIRE_THROWS_WITH(manager.add_interfacer<ServosInterfacer>(), "The module doesn't exist.");
		module_manager->add_module<PhysicalRobot::Servos>(5);
		REQUIRE_NOTHROW(manager.add_interfacer<ServosInterfacer>());
		REQUIRE_NOTHROW(manager.get_interfacer<ServosInterfacer>());
		CHECK(manager.has_interfacer<ServosInterfacer>());

		auto& servos = manager.get_interfacer<ServosInterfacer>();
		REQUIRE_NOTHROW(servos->add_servo(8));

		Distance sx = 40_cm;
		Distance sy = 30_cm;
		Environment env({300, 200}, 1_cm, sy, (sqrt(sx * sx + sy * sy) / 2) * 1.2, Vector2m(0_m, 1_m));
		REQUIRE_NOTHROW(manager.add_interfacer<AvoidanceInterfacer>(env));

		module_manager->add_module<PhysicalRobot::Navigation>(8);
		REQUIRE_NOTHROW(manager.add_interfacer<NavigationInterfacer>(env, manager.get_interfacer<AvoidanceInterfacer>()));

		CHECK(manager.size() == 3);
	}
}
