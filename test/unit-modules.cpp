//
// Created by terae on 8/28/18.
//

#include "catch.hpp"

#include "../src/robot/Modules/ModuleManager.h"

struct SharedStructTest {
	uint8_t a, b;
};
SharedStructTest test_read_frame(const uint8_t* frame, uint8_t size) {
	if(size == 2) {
		SharedStructTest s = {frame[0], frame[1]};
		return s;
	}
	throw std::runtime_error("Invalid frame format.");
}

uint8_t test_write_frame(uint8_t* buf, uint8_t buf_size, const SharedStructTest* obj) {
	if(buf_size >= 2) {
		buf[0] = obj->a;
		buf[1] = obj->b;
		return 2;
	}

	return 0;
}

class ModuleTest : public PhysicalRobot::Module<SharedStructTest> {
public:
	explicit ModuleTest(uint8_t id) : Module(id, test_read_frame, test_write_frame), _a(1), _b(2) {}

	inline uint8_t get_frame_size() const override {
		return 2;
	}

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
	SharedStructTest generate_shared() const override {
		SharedStructTest s = {_a, _b};
		return s;
	}

	void message_processing(const SharedStructTest& s) override {
		_a.exchange(s.a);
		_b.exchange(s.b);
	}

	void deactivation() override {}

	std::atomic_uint8_t _a, _b;
};

#include <log/Log.h>
TEST_CASE("Basic module") {

	SECTION("Simple tests.") {
		ModuleTest my_module(5);
		CHECK(my_module.get_id() == 5);
		CHECK(my_module.get_frame_size() == 2);
		CHECK(my_module.get_a_value() == 1);
		CHECK(my_module.get_b_value() == 2);
	}

	SECTION("Frames manipulation.") {
		SECTION("void update(const GlobalFrame &f)") {
			GlobalFrame f;
			f.addByte(12);
			f.addByte(34);

			ModuleTest my_module(42);
			my_module.update(f);
			CHECK(my_module.get_a_value() == 12);
			CHECK(my_module.get_b_value() == 34);
		}

		SECTION("GlobalFrame make_frame() const") {
			ModuleTest my_module(42);
			auto frame = my_module.make_frame();
			REQUIRE(frame.getNbDonnees() == 2);
			CHECK(frame.getDonnees()[0] == 1);
			CHECK(frame.getDonnees()[1] == 2);
		}
	}
}


// Ce define très moche permets d'accéder aux fonctions et variables cachées des objets (TESTS uniquement)
#define private public
#include "../src/robot/Modules/Servos.h"
#undef private

#include <log/Log.h>
TEST_CASE("Servos 2019 Module") {
	SECTION("Non-frame functions' module") {
		PhysicalRobot::Servos my_module(2);
		my_module.add_servo(5, PhysicalRobot::Servos::HOLD_ON_BLOCKING);
		my_module.add_servo(2, PhysicalRobot::Servos::UNBLOCKING);
		my_module.add_servo(4);

		REQUIRE_THROWS_WITH(my_module.add_servo(5), "Double assignation du servo 5 !");
		REQUIRE_NOTHROW(my_module.add_servo(42));
		REQUIRE_THROWS_WITH(my_module.add_servo(0), "L'ID 0 des servos est réservé !");
		CHECK(my_module.get_nbr_servos() == 4);
		CHECK(my_module.get_frame_size() == 25);

		REQUIRE_THROWS_WITH(my_module.set_position(1, 50_deg), "Numéro du servo demandé invalide : 1");
		my_module.set_position(2, 50.4_deg);
		// Servo 2 commandé en position
		CHECK_FALSE(my_module.is_moving_done(2));

		REQUIRE_THROWS_WITH(my_module.set_speed(1, 2), "Numéro du servo demandé invalide : 1");
		my_module.set_speed(2, 6);

		REQUIRE_THROWS_WITH(my_module.read_position(1), "Numéro du servo demandé invalide : 1");
		CHECK(my_module.read_position(2) == -119_deg);

		REQUIRE_THROWS_WITH(my_module.set_color(1, PhysicalRobot::Servos::GREEN), "Numéro du servo demandé invalide : 1");
		my_module.set_color(2, PhysicalRobot::Servos::GREEN);

		REQUIRE_THROWS_WITH(my_module.is_blocking(1), "Numéro du servo demandé invalide : 1");
		CHECK_FALSE(my_module.is_blocking(2));

		REQUIRE_THROWS_WITH(my_module.is_moving_done(1), "Numéro du servo demandé invalide : 1");
		// Servo 2 commandé en vitesse
		CHECK(my_module.is_moving_done(2));
		CHECK(my_module.is_moving_done(4));

		/*SECTION("Vérification de la structure générée") {
		    SharedServos2019 s = my_module.generate_shared();
		    CHECK(s.servos[7].id == 0);

		    // Ce servo a subi bcp de modifications au-dessus
		    SharedServos2019::Servo2019 servo_2 = s.servos[2];
		    REQUIRE(servo_2.id == 2);
		    CHECK(servo_2.position == 146);        // -119_deg
		    CHECK(servo_2.wanted_position == 666); // 55.4_deg
		    CHECK(servo_2.speed == 6);
		    CHECK_FALSE(servo_2.blocked);
		    CHECK(servo_2.blocking_mode == Servos::UNBLOCKING);
		    CHECK(servo_2.color == Servos::GREEN);

		    // Ce servo n'a reçu aucune modification depuis sa construction
		    SharedServos2019::Servo2019 servo_5 = s.servos[5];
		    REQUIRE(servo_5.id == 5);
		    CHECK(servo_5.position == 849); // 110_deg
		    CHECK(servo_5.wanted_position == 849);
		    CHECK(servo_5.speed == 30);
		    CHECK_FALSE(servo_5.blocked);
		    CHECK(servo_5.blocking_mode == Servos::HOLD_ON_BLOCKING);
		    CHECK(servo_5.color == Servos::YELLOW);
		}*/
	}

	SECTION("Frame functions' module") {
		const uint8_t SIZE = 13;

		SECTION("SharedStruct2019 servo_read_frame(const uint8_t *, uint8_t)") {
			SECTION("bad frame format") {
				uint8_t buf[SIZE];
				buf[0] = 3;
				buf[7] = 5;

				// buf == NULL
				REQUIRE(servo_read_frame(nullptr, SIZE).parsing_failed);

				// on annonce un buffeur vide
				REQUIRE(servo_read_frame(buf, 0).parsing_failed);

				// 3 servos sont déclarés, donc il faut une taille de (1 + 3 * 6 = 19) bits
				REQUIRE(servo_read_frame(buf, SIZE).parsing_failed);

				buf[0] = 2;
				buf[1] = 42;
				REQUIRE_FALSE(servo_read_frame(buf, SIZE).parsing_failed);

				// l'ID 0 veut dire qu'il n'y a pas de servo, donc on économise de la place dans la trame
				buf[1] = 0;
				REQUIRE(servo_read_frame(buf, SIZE).parsing_failed);

				// Les servos instantiés doivent être adjacents en début du buffeur.
				buf[0] = 1;
				buf[1] = 0;
				REQUIRE(servo_read_frame(buf, 7).parsing_failed);
			}

			SECTION("too many servos") {
				uint8_t buf[55];
				buf[0] = 9;
				// IDs des 9 servos
				buf[1] = 1;
				buf[7] = 2;
				buf[13] = 3;
				buf[19] = 4;
				buf[25] = 5;
				buf[31] = 6;
				buf[37] = 7;
				buf[43] = 8;
				buf[49] = 9;
				REQUIRE(servo_read_frame(buf, 55).parsing_failed);
			}

			SECTION("good frame format") {
				uint8_t buf[SIZE];
				buf[0] = 2;

				// id = 1
				buf[1] = 1;
				// position = 0b00111100 11000011
				buf[2] = 0b00111100;
				buf[3] = 0b11000011;
				// command = 0b01111110 10000001
				buf[4] = 0b01111110;
				buf[5] = 0b10000001;

				// command = POSITION, blocked = true, blocking_mode = 0, color = 0b011
				buf[6] = 0b010011;

				// id = 3
				buf[7] = 3;
				// position = 0b00000000 11111111
				buf[8] = 0b00000000;
				buf[9] = 0b11111111;
				// command = 0b01010101 10101010
				buf[10] = 0b01010101;
				buf[11] = 0b10101010;
				// command = SPEED, blocked = false, blocking_mode = 1, color = 0b000
				buf[12] = 0b101000;

				SharedServos2019 s;
				REQUIRE_FALSE((s = servo_read_frame(buf, SIZE)).parsing_failed);
				CHECK(s.servos[2].id == 0);

				CHECK(s.servos[0].id == 1);
				CHECK(s.servos[0].position == 0b00111100'11000011);
				CHECK(s.servos[0].command == 0b01111110'10000001);
				CHECK(s.servos[0].command_type == 0);
				CHECK(s.servos[0].blocked);
				CHECK(s.servos[0].blocking_mode == 0);
				REQUIRE(s.servos[0].color == 0b011);

				CHECK(s.servos[1].id == 3);
				CHECK(s.servos[1].position == 0b00000000'11111111);
				CHECK(s.servos[1].command == 0b01010101'10101010);
				CHECK(s.servos[1].command_type == 1);
				CHECK_FALSE(s.servos[1].blocked);
				CHECK(s.servos[1].blocking_mode == 1);
				CHECK(s.servos[1].color == 0b000);

				buf[0] = 0;
				REQUIRE_FALSE((s = servo_read_frame(buf, 1)).parsing_failed);
				CHECK(s.servos[0].id == 0);
				CHECK(s.servos[2].id == 0);
			}
		}

		SECTION("uint8_t servo_write_frame(uint8_t *, uint8_t, const SharedServos2019 *)") {
			SECTION("bad arguments") {
				uint8_t buf[SIZE];
				auto servos = new SharedServos2019();
				for(int i = 0; i < 8; ++i) {
					servos->servos[i].id = 0;
				}
				servos->servos[0].id = 1;
				servos->servos[1].id = 3;
				servos->nb_servos = 2;

				// buf == NULL
				REQUIRE(servo_write_frame(nullptr, SIZE, servos) == 0);

				// on annonce un buffeur vide
				REQUIRE(servo_write_frame(buf, 0, servos) == 0);

				// 2 servos sont déclarés, donc il faut une taille de (1 + 2 * 7 = 15) bits
				REQUIRE(servo_write_frame(buf, 3, servos) == 0);

				// servos == NULL
				REQUIRE(servo_write_frame(buf, SIZE, nullptr) == 0);
			}

			SECTION("good arguments") {
				SECTION("empty") {
					uint8_t buf[1];
					auto servos = new SharedServos2019();
					for(int i = 0; i < 8; ++i) {
						servos->servos[i].id = 0;
					}
					REQUIRE(servo_write_frame(buf, 1, servos) == 1);
					CHECK(buf[0] == 0);
				}

				SECTION("2 servos declared") {
					uint8_t buf[SIZE];
					auto servos = new SharedServos2019();
					for(int i = 0; i < 8; ++i) {
						servos->servos[i].id = 0;
					}
					//                   id position             command        c_type blocked mode color
					servos->servos[0] = {1, 0b00001111'11110000, 0b01010101'10101010, 0, true, 1, 0b100};
					servos->servos[1] = {3, 0b00000000'00000000, 0b11111111'11111111, 1, false, 0, 0b010};
					servos->nb_servos = 2;

					REQUIRE(servo_write_frame(buf, SIZE, servos) == SIZE);
					CHECK(buf[0] == 2);
					CHECK(buf[1] == 1);
					CHECK(buf[2] == 0b00001111);
					CHECK(buf[3] == 0b11110000);
					CHECK(buf[4] == 0b01010101);
					CHECK(buf[5] == 0b10101010);
					CHECK(buf[6] == 0b011100);

					CHECK(buf[7] == 3);
					CHECK(buf[8] == 0b00000000);
					CHECK(buf[9] == 0b00000000);
					CHECK(buf[10] == 0b11111111);
					CHECK(buf[11] == 0b11111111);
					CHECK(buf[12] == 0b100010);
				}
			}
		}
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
				REQUIRE(manager.write_frame() == std::nullopt);
				module_test.set_a_value(2);
				REQUIRE(manager.write_frame() != std::nullopt);

				module_test.set_b_value(3);
				auto frame_test = manager.write_frame().value();
				REQUIRE(frame_test.getNbDonnees() == 1 + module_test.get_frame_size());
				const uint8_t* array_test = frame_test.getDonnees();
				// ID
				CHECK(array_test[0] == module_test.get_id());
				// a
				CHECK(array_test[1] == 2);
				// b
				CHECK(array_test[2] == 3);
			}

			SECTION("Servos") {
				auto& module_servo = manager.add_module<PhysicalRobot::Servos>(15);
				module_servo.add_servo(2);
				module_servo.set_position(2, 90_deg);

				auto frame_servos = manager.write_frame().value();
				REQUIRE(frame_servos.getNbDonnees() == 1 + module_servo.get_frame_size());
				const uint8_t* array_servos = frame_servos.getDonnees();
				// ID
				REQUIRE(array_servos[0] == module_servo.get_id());
				CHECK(array_servos[1] == module_servo.get_nbr_servos());
				CHECK(array_servos[2] == module_servo.make_frame().getDonnee(1));
			}
		}

		SECTION("read_frame") {
			PhysicalRobot::ModuleManager manager;
			manager.add_module<ModuleTest>(5);
			auto& module_servo = manager.add_module<PhysicalRobot::Servos>(15);
			module_servo.add_servo(2);

			REQUIRE_THROWS_WITH(manager.read_frame({}), "Frame does not contain the module's id.");
			REQUIRE_THROWS_WITH(manager.read_frame({13}), "The module n°13 isn't initialized.");
			REQUIRE_THROWS_WITH(manager.read_frame({5}), "The size of the module n°5 does not correspond to the theory (0 != 2).");
			REQUIRE_THROWS_WITH(manager.read_frame({5, 0x1, 0x2, 0x3}),
			                    "The size of the module n°5 does not correspond to the theory (3 != 2).");
			REQUIRE_NOTHROW(manager.read_frame({5, 0, 0}));
		}
	}
}
