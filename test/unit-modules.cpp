//
// Created by terae on 8/28/18.
//

#include "catch.hpp"

#include <log/Log.h>

#include "../src/robot/Modules/ModuleManager.h"

class ModuleTest : public PhysicalRobot::Module {
public:
	explicit ModuleTest(uint8_t id) : Module(id), _a(1), _b(2) {}

	// Accesseurs pour les tests
	inline uint8_t get_a_value() const {
		return _a;
	}
	inline uint8_t get_b_value() const {
		return _b;
	}
	void set_a_value(int a) {
		_a = a;
		_state_changed.exchange(true);
	}
	void set_b_value(int b) {
		_b = b;
		_state_changed.exchange(true);
	}

private:
	std::vector<JSON> generate_list_jsons() const override {
		JSON j;
		j["a"] = _a.load();
		j["b"] = _b.load();

		return {j};
	}

	void message_processing(const JSON& j) override {
		_a.exchange(j["a"]);
		_b.exchange(j["b"]);
	}

	void deactivation() override {}

	std::atomic_uint8_t _a, _b;
};

TEST_CASE("Basic module") {

	SECTION("Simple tests.") {
		ModuleTest my_module(5);
		CHECK(my_module.get_id() == 5);
		CHECK(my_module.get_a_value() == 1);
		CHECK(my_module.get_b_value() == 2);
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

TEST_CASE("Servos' Module") {
	SECTION("Non-frame functions' module") {
		PhysicalRobot::Servos my_module(2);
		my_module.add_servo(5, PhysicalRobot::BlockingMode::HoldOnBlock);
		my_module.add_servo(2, PhysicalRobot::BlockingMode::Unblocking);
		my_module.add_servo(4);

		REQUIRE_THROWS_WITH(my_module.add_servo(5), "Double assignation du servo 5 !");
		REQUIRE_NOTHROW(my_module.add_servo(42));
		REQUIRE_THROWS_WITH(my_module.add_servo(0), "L'ID 0 des servos est réservé !");
		CHECK(my_module.get_nbr_servos() == 4);

		REQUIRE_THROWS_WITH(my_module.set_position(1, 50_deg), "Numéro du servo demandé invalide : 1");
		my_module.set_position(2, 50.4_deg);
		// Servo 2 commandé en position
		CHECK_FALSE(my_module.is_moving_done(2));

		REQUIRE_THROWS_WITH(my_module.set_speed(1, 2_deg_s), "Numéro du servo demandé invalide : 1");
		my_module.set_speed(2, 6_deg_s, PhysicalRobot::Rotation::CounterClockwise);

		REQUIRE_THROWS_WITH(my_module.read_position(1), "Numéro du servo demandé invalide : 1");

		REQUIRE_THROWS_WITH(my_module.set_color(1, PhysicalRobot::Color::Green), "Numéro du servo demandé invalide : 1");
		my_module.set_color(2, PhysicalRobot::Color::Green);

		REQUIRE_THROWS_WITH(my_module.is_blocking(1), "Numéro du servo demandé invalide : 1");
		CHECK_FALSE(my_module.is_blocking(2));

		REQUIRE_THROWS_WITH(my_module.is_moving_done(1), "Numéro du servo demandé invalide : 1");
		// Servo 2 commandé en vitesse
		CHECK(my_module.is_moving_done(2));
		CHECK(my_module.is_moving_done(4));
	}
}


#include "../src/robot/Modules/Navigation.h"

TEST_CASE("Navigation Module") {
    PhysicalRobot::Navigation my_module(1);

    SECTION("Initialization") {
        auto starting_point = my_module.get_coordinates();
        CHECK(starting_point.getX().toM() == Approx(0));
        CHECK(starting_point.getY().toM() == Approx(0));
        CHECK(starting_point.getAngle().toRad() == Approx(0));
    }
}


#include "../src/robot/Communication/NamedPipe.h"

TEST_CASE("ModuleManager") {

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
		REQUIRE_THROWS_WITH(manager.add_module<ModuleTest>(8), "Double assignment of the module type: 10ModuleTest");
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
		}

		SECTION("read_frame") {
			PhysicalRobot::ModuleManager manager;
			manager.add_module<ModuleTest>(5);
			auto& module_servo = manager.add_module<PhysicalRobot::Servos>(15);
			module_servo.add_servo(2);

			REQUIRE_THROWS_WITH(manager.read_frame({}), "Frame does not contain the module's id.");
			// la trame est drop vu que le module 13 n'est pas initialisé
			REQUIRE_NOTHROW(manager.read_frame({13}), "The module n°13 isn't initialized.");
			// TODO: some tests with a good frame
		}
	}
}
