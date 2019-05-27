//
// Created by terae on 8/28/18.
//

#include "catch.hpp"

#include <log/Log.h>

#include "../src/robot/Modules/ModuleManager.h"
#include "TestUtils.h"

using TestUtils::ModuleTest;

TEST_CASE("Basic module") {
	SECTION("Simple tests.") {
		ModuleTest my_module(5);
		CHECK(my_module.get_id() == 5);
		CHECK(my_module.get_a_value() == 1);
		CHECK(my_module.get_b_value() == 2);
		CHECK(my_module.name == "ModuleTest");
	}

	SECTION("Frames manipulation.") {
		SECTION("void update(const GlobalFrame &f)") {
			GlobalFrame f;
			f.addBytes({'{', '\"', 'a', '\"', ':', '1', '2', ',', '\"', 'b', '\"', ':', '3', '4', '}'});

			ModuleTest my_module(42);
			my_module.update(f);
			CHECK(my_module.get_a_value() == 12);
			CHECK(my_module.get_b_value() == 34);
		}

		SECTION("GlobalFrame make_frame() const") {
			ModuleTest my_module(42);
			auto frames = my_module.make_frames();
			REQUIRE(frames.size() == 1);
			GlobalFrame frame = frames[0];
			REQUIRE(frame.getNbDonnees() == 13);
			std::string msg(frame.getDonnees(), frame.getDonnees() + frame.getNbDonnees());
			CHECK(msg == "{\"a\":1,\"b\":2}");
		}
	}
}

#include "../src/robot/Modules/Servos.h"

TEST_CASE("Servos' Module", "[integration]") {
	PhysicalRobot::Servos my_module(2);

	SECTION("Non-frame functions' module") {
		my_module.add_servo(5, PhysicalRobot::BlockingMode::HoldOnBlock);
		my_module.add_servo(2, PhysicalRobot::BlockingMode::Unblocking);
		my_module.add_servo(4);

		REQUIRE_THROWS_WITH(my_module.add_servo(PhysicalRobot::Servos::ID_RESERVED_SERVOS), "ID du servo trop grand (255 > 254) !");
		REQUIRE_THROWS_WITH(my_module.add_servo(5), "Double assignation du servo 5 !");
		REQUIRE_NOTHROW(my_module.add_servo(42));
		REQUIRE_NOTHROW(my_module.add_servo(0));
		CHECK(my_module.get_nbr_servos() == 4);
		CHECK(my_module.name == "Servos");

		REQUIRE_THROWS_WITH(my_module.set_position(0, 50_deg), "Numéro du servo demandé invalide : 0");
		REQUIRE_THROWS_WITH(my_module.set_position(1, 50_deg), "Numéro du servo demandé invalide : 1");
		my_module.set_position(2, 50.4_deg);
		// Servo 2 commandé en position
		CHECK_FALSE(my_module.is_moving_done(2));

		REQUIRE_THROWS_WITH(my_module.set_speed(0, 2_deg_s), "Numéro du servo demandé invalide : 0");
		REQUIRE_THROWS_WITH(my_module.set_speed(1, 2_deg_s), "Numéro du servo demandé invalide : 1");
		my_module.set_speed(2, 6_deg_s, PhysicalRobot::Rotation::CounterClockwise);

		REQUIRE_THROWS_WITH(my_module.read_position(0), "Numéro du servo demandé invalide : 0");
		REQUIRE_THROWS_WITH(my_module.read_position(1), "Numéro du servo demandé invalide : 1");

		REQUIRE_THROWS_WITH(my_module.set_color(0, PhysicalRobot::Color::Green), "Numéro du servo demandé invalide : 0");
		REQUIRE_THROWS_WITH(my_module.set_color(1, PhysicalRobot::Color::Green), "Numéro du servo demandé invalide : 1");
		my_module.set_color(2, PhysicalRobot::Color::Green);

		REQUIRE_THROWS_WITH(my_module.set_blocking_mode(1, PhysicalRobot::BlockingMode::HoldOnBlock),
		                    "Numéro du servo demandé invalide : 1");
		my_module.set_blocking_mode(2, PhysicalRobot::BlockingMode::HoldOnBlock);

		REQUIRE_THROWS_WITH(my_module.get_blocking_mode(0), "Numéro du servo demandé invalide : 0");
		REQUIRE_THROWS_WITH(my_module.get_blocking_mode(1), "Numéro du servo demandé invalide : 1");
		CHECK(my_module.get_blocking_mode(2) == PhysicalRobot::BlockingMode::HoldOnBlock);

		REQUIRE_THROWS_WITH(my_module.is_blocking(0), "Numéro du servo demandé invalide : 0");
		REQUIRE_THROWS_WITH(my_module.is_blocking(1), "Numéro du servo demandé invalide : 1");
		CHECK_FALSE(my_module.is_blocking(2));

		REQUIRE_THROWS_WITH(my_module.is_moving_done(0), "Numéro du servo demandé invalide : 0");
		REQUIRE_THROWS_WITH(my_module.is_moving_done(1), "Numéro du servo demandé invalide : 1");
		// Servo 2 commandé en vitesse
		CHECK(my_module.is_moving_done(2));
		CHECK(my_module.is_moving_done(4));
	}

	SECTION("Frame functions' module") {
		my_module.add_servo(12);
		JSON json;
		json["id"] = 12;
		json["known_position"] = 546;
		json["blocked"] = true;
		json["control"] = "Speed";
		json["data"] = 0;
		json["rotation"] = "Clockwise";
		json["mode"] = "Ublocking";
		json["color"] = "Blue";


		std::stringstream ss;
		ss << json;
		const std::string msg = ss.str();
		my_module.update(GlobalFrame(std::vector<uint8_t>(msg.cbegin(), msg.cend())));
		CHECK((my_module.read_position(12) - 11.240_deg).toDeg() < .001);
		CHECK(my_module.is_blocking(12));
	}
}

#include "../src/robot/Modules/IO.h"

TEST_CASE("IO Module", "[integration]") {
	PhysicalRobot::IO my_module(2);
	CHECK(my_module.read_tirette() == PhysicalRobot::TriggerState::Waiting);

	const std::string msg = "{\"tirette\":\"Triggered\"}";
	my_module.update(GlobalFrame(std::vector<uint8_t>(msg.cbegin(), msg.cend())));
	CHECK(my_module.read_tirette() == PhysicalRobot::TriggerState::Triggered);
}

#include "../src/robot/Modules/Navigation.h"

TEST_CASE("Navigation Module", "[integration]") {
	PhysicalRobot::Navigation my_module(1);

	SECTION("Initialization") {
		auto starting_point = my_module.get_coordinates();
		CHECK(starting_point.getX().toM() == Approx(0));
		CHECK(starting_point.getY().toM() == Approx(0));
		CHECK(starting_point.getAngle().toRad() == Approx(0));
	}
}

#include "../src/robot/Communication/NamedPipe.h"

TEST_CASE("ModuleManager", "[integration]") {
	SECTION("basic functions") {
		PhysicalRobot::ModuleManager manager;

		REQUIRE_THROWS_WITH(manager.get_module<PhysicalRobot::Servos>(), "The module doesn't exist.");
		CHECK_FALSE(manager.has_module<PhysicalRobot::Servos>());
		CHECK_FALSE(manager.has_module(8));

		REQUIRE_THROWS_WITH(manager.add_module<ModuleTest>(42), "Impossible to add module n°42 (> 16).");
		CHECK(manager.get_nb_modules() == 0);
		manager.add_module<ModuleTest>(5);
		CHECK(manager.get_nb_modules() == 1);
		CHECK(manager.has_module<ModuleTest>());
		CHECK(manager.has_module(5));
		REQUIRE_THROWS_WITH(manager.add_module<ModuleTest>(5), "Double assignment of the module n°5.");
		REQUIRE_THROWS_WITH(manager.add_module<ModuleTest>(8), "Double assignment of the module type: N9TestUtils10ModuleTestE");
		manager.add_module<PhysicalRobot::Servos>(6);
		CHECK(manager.get_nb_modules() == 2);

		REQUIRE_THROWS_WITH(manager.get_module_by_id(42), "Impossible to get module n°42 (> 16).");
		REQUIRE_THROWS_WITH(manager.get_module_by_id(8), "The module n°8 doesn't exist.");
		REQUIRE_NOTHROW(manager.get_module_by_id(5));

		// ne compile pas car 'int' n'hérite pas de BaseModule
		// CHECK_NOTHROW(manager.get_module<int>());
		REQUIRE_NOTHROW(manager.get_module<ModuleTest>());
		CHECK(manager.get_module<ModuleTest>().get_id() == 5);
		CHECK(manager.get_module<PhysicalRobot::Servos>().get_id() == 6);

		std::vector<uint8_t> list_modules = {5, 6};
		CHECK(manager.get_list_modules() == list_modules);
	}

	SECTION("Frame manipulation") {
		SECTION("GlobalFrame write_frame()") {
			PhysicalRobot::ModuleManager manager;
			SECTION("Test") {
				auto& module_test = manager.add_module<ModuleTest>(5);

				// Le module manager n'a aucune modification à apporter côté élec, et le timer n'a pas encore expiré
				REQUIRE(manager.write_frame().empty());
				module_test.set_a_value(2);
				REQUIRE_FALSE(manager.write_frame().empty());

				module_test.set_b_value(3);
				auto frames_test = manager.write_frame();
				REQUIRE(frames_test.size() == 1);

				GlobalFrame frame_test = frames_test[0];
				// ID
				REQUIRE(frame_test.getDonnees()[0] == module_test.get_id());

				std::string msg(frame_test.getDonnees() + 1, frame_test.getDonnees() + frame_test.getNbDonnees());
				CHECK(msg == "{\"a\":2,\"b\":3}");
			}

			SECTION("Servos") {
				auto& module_servos = manager.add_module<PhysicalRobot::Servos>(15);
				module_servos.add_servo(254);
				module_servos.set_speed(254, 500_mrad_s);

				auto frames_servos = manager.write_frame();
				REQUIRE(frames_servos.size() == 1);

				GlobalFrame frame_servos = frames_servos[0];

				// ID
				REQUIRE(frame_servos.getDonnees()[0] == module_servos.get_id());

				std::string msg(frame_servos.getDonnees() + 1, frame_servos.getDonnees() + frame_servos.getNbDonnees());

				CHECK(msg == "{\"blocked\":false,\"color\":\"Yellow\",\"control\":\"Speed\",\"data\":81,\"id\":254,"
				             "\"known_position\":511,\"mode\":\"Unblocking\",\"rotation\":\"CounterClockwise\"}");
			}

			SECTION("Navigation") {
				auto& module_nav = manager.add_module<PhysicalRobot::Navigation>(15);

				SECTION("No commands") {
					auto frames_nav = manager.write_frame();
					REQUIRE(frames_nav.empty());
				}

				SECTION("Simple commands") {
					module_nav.forward(5_m, PhysicalRobot::SensAdvance::Forward);
					auto frames_nav = manager.write_frame();
					REQUIRE(frames_nav.size() == 1);

					GlobalFrame frame_nav = frames_nav[0];
					REQUIRE(frame_nav.getDonnees()[0] == module_nav.get_id());

					INFO(std::string(frame_nav.getDonnees() + 1, frame_nav.getDonnees() + frame_nav.getNbDonnees()));
					JSON json =
					    nlohmann::json::parse(frame_nav.getDonnees() + 1, frame_nav.getDonnees() + frame_nav.getNbDonnees());
					CHECK(std::string("GoForward") == json["command"]);


					// check that manager.write_frame() flushes frame buffer
					frames_nav = manager.write_frame();
					CHECK(frames_nav.empty());
				}
			}
		}

		SECTION("read_frame") {
			PhysicalRobot::ModuleManager manager;
			manager.add_module<ModuleTest>(5);
			auto& module_servo = manager.add_module<PhysicalRobot::Servos>(15);
			module_servo.add_servo(2);

			REQUIRE_THROWS_WITH(manager.read_frame({}), "Frame does not contain the module's id.");
			// la trame est drop vu que le module 13 n'est pas initialisé
			Log::open(Log::NOTHING, "", false);
			REQUIRE_NOTHROW(manager.read_frame({13}), "The module n°13 isn't initialized.");
			Log::close(Log::NOTHING);
			// TODO: some tests with a good frame
		}
	}
}
