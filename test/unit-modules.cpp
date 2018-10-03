//
// Created by terae on 8/28/18.
//

#include "catch.hpp"

#include "../src/robot/Modules/ModuleManager.h"

using namespace Commun;

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

class ModuleTest : public Module<SharedStructTest> {
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
#include "../src/robot/Modules/Servos2019.h"
#undef private

#include <log/Log.h>
TEST_CASE("Servos 2019 Module") {
	SECTION("Non-frame functions' module") {
		Servos2019 my_module(2);
		my_module.add_servo(5, 110_deg, Servos2019::HOLD_ON_BLOCKING);
		my_module.add_servo(2, -119_deg, Servos2019::UNBLOCKING);
		my_module.add_servo(4, 80_deg);

		/*SECTION("Test de la validité d'un servo") {
		    REQUIRE_FALSE(my_module.is_servo_ok(0));
		    REQUIRE(my_module.is_servo_ok(4));
		    REQUIRE_FALSE(my_module.is_servo_ok(6));
		    REQUIRE_FALSE(my_module.is_servo_ok(Servos2019::NB_MAX_SERVOS));
		    REQUIRE_FALSE(my_module.is_servo_ok(42));
		}*/

		CHECK_THROWS_WITH(my_module.add_servo(5, 50_deg), "Double assignation du servo 5 !");
		CHECK_THROWS_WITH(my_module.add_servo(42, 50_deg), "ID du servo trop grand (42 > 8) !");
		CHECK_THROWS_WITH(my_module.add_servo(0, 0_deg), "L'ID 0 des servos est réservé !");
		CHECK(my_module.get_nbr_servos() == 3);
		// CHECK(my_module.get_frame_size() == ?);

		CHECK_THROWS_WITH(my_module.set_position(1, 50_deg), "Numéro du servo demandé invalide : 1");
		my_module.set_position(2, 50.4_deg);

		CHECK_THROWS_WITH(my_module.set_speed(1, 2), "Numéro du servo demandé invalide : 1");
		my_module.set_speed(2, 6);

		CHECK_THROWS_WITH(my_module.read_position(1), "Numéro du servo demandé invalide : 1");
		CHECK(my_module.read_position(2) == -119_deg);

		CHECK_THROWS_WITH(my_module.set_color(1, Servos2019::GREEN), "Numéro du servo demandé invalide : 1");
		my_module.set_color(2, Servos2019::GREEN);

		CHECK_THROWS_WITH(my_module.is_blocking(1), "Numéro du servo demandé invalide : 1");
		CHECK_FALSE(my_module.is_blocking(2));

		CHECK_THROWS_WITH(my_module.is_moving_done(1), "Numéro du servo demandé invalide : 1");
		CHECK_FALSE(my_module.is_moving_done(2));
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
		    CHECK(servo_2.blocking_mode == Servos2019::UNBLOCKING);
		    CHECK(servo_2.color == Servos2019::GREEN);

		    // Ce servo n'a reçu aucune modification depuis sa construction
		    SharedServos2019::Servo2019 servo_5 = s.servos[5];
		    REQUIRE(servo_5.id == 5);
		    CHECK(servo_5.position == 849); // 110_deg
		    CHECK(servo_5.wanted_position == 849);
		    CHECK(servo_5.speed == 30);
		    CHECK_FALSE(servo_5.blocked);
		    CHECK(servo_5.blocking_mode == Servos2019::HOLD_ON_BLOCKING);
		    CHECK(servo_5.color == Servos2019::YELLOW);
		}*/
	}

	SECTION("Frame functions' module") {
		const uint8_t SIZE = 15;

		SECTION("SharedStruct2019 servo_read_frame(const uint8_t *, uint8_t)") {
			SECTION("bad frame format") {
				uint8_t buf[SIZE];
				buf[0] = 3;

				// buf == NULL
				REQUIRE(servo_read_frame(nullptr, SIZE).parsing_failed);

				// on annonce un buffeur vide
				REQUIRE(servo_read_frame(buf, 0).parsing_failed);

				// 3 servos sont déclarés, donc il faut une taille de (1 + 3 * 7 = 22) bits
				REQUIRE(servo_read_frame(buf, SIZE).parsing_failed);

				// l'ID 42 est interdit pour un servo
				buf[0] = 2;
				buf[1] = 42;
				REQUIRE(servo_read_frame(buf, SIZE).parsing_failed);

				// et l'ID 0 veut dire qu'il n'y a pas de servo, donc on économise de la place dans la trame
				buf[1] = 0;
				REQUIRE(servo_read_frame(buf, 1).parsing_failed);
			}

			SECTION("good frame format") {
				uint8_t buf[SIZE];
				buf[0] = 2;

				// id = 1
				buf[1] = 1;
				// position = 0b00111100 11000011
				buf[2] = 0b00111100;
				buf[3] = 0b11000011;
				// wanted_position = 0b01111110 10000001
				buf[4] = 0b01111110;
				buf[5] = 0b10000001;
				// speed = 0b00001111
				buf[6] = 0b00001111;

				// blocked = true, blocking_mode = 0, color = 0b011
				buf[7] = 0b10011;

				// id = 3
				buf[8] = 3;
				// position = 0b00000000 11111111
				buf[9] = 0b00000000;
				buf[10] = 0b11111111;
				// wanted_position = 0b01010101 10101010
				buf[11] = 0b01010101;
				buf[12] = 0b10101010;
				// speed = 0b11110000
				buf[13] = 0b11110000;
				// blocked = false, blocking_mode = 1, color = 0b000
				buf[14] = 0b01000;

				SharedServos2019 s;
				REQUIRE_FALSE((s = servo_read_frame(buf, SIZE)).parsing_failed);
				CHECK(s.servos[2].id == 0);

				CHECK(s.servos[1].id == 1);
				CHECK(s.servos[1].position == 0b00111100'11000011);
				CHECK(s.servos[1].wanted_position == 0b01111110'10000001);
				CHECK(s.servos[1].speed == 0b00001111);
				CHECK(s.servos[1].blocked);
				CHECK(s.servos[1].blocking_mode == 0);
				REQUIRE(s.servos[1].color == 0b011);

				CHECK(s.servos[3].id == 3);
				CHECK(s.servos[3].position == 0b00000000'11111111);
				CHECK(s.servos[3].wanted_position == 0b01010101'10101010);
				CHECK(s.servos[3].speed == 0b11110000);
				CHECK_FALSE(s.servos[3].blocked);
				CHECK(s.servos[3].blocking_mode == 1);
				CHECK(s.servos[3].color == 0b000);

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
				servos->servos[1].id = 1;
				servos->servos[3].id = 3;

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
					servos->servos[1] = {1, 0b00001111'11110000, 0b01010101'10101010, 0b00110011, true, 1, 0b100};
					servos->servos[3] = {3, 0b00000000'00000000, 0b11111111'11111111, 0b00011101, false, 0, 0b010};

					REQUIRE(servo_write_frame(buf, SIZE, servos) == SIZE);
					CHECK(buf[0] == 2);
					CHECK(buf[1] == 1);
					CHECK(buf[2] == 0b00001111);
					CHECK(buf[3] == 0b11110000);
					CHECK(buf[4] == 0b01010101);
					CHECK(buf[5] == 0b10101010);
					CHECK(buf[6] == 0b00110011);
					CHECK(buf[7] == 0b11100);

					CHECK(buf[8] == 3);
					CHECK(buf[9] == 0b00000000);
					CHECK(buf[10] == 0b00000000);
					CHECK(buf[11] == 0b11111111);
					CHECK(buf[12] == 0b11111111);
					CHECK(buf[13] == 0b00011101);
					CHECK(buf[14] == 0b00010);
				}
			}
		}
	}
}

#include "../src/robot/Communication/NamedPipe.h"

TEST_CASE("ModuleManager") {

	SECTION("basic functions") {
		ModuleManager manager;

		REQUIRE_THROWS_WITH(manager.get_module<Servos2019>(), "The module doesn't exist.");
		CHECK_FALSE(manager.has_module<Servos2019>());
		CHECK_FALSE(manager.has_module(8));

		REQUIRE_THROWS_WITH(manager.add_module<ModuleTest>(42), "Impossible to add module n°42 (> 16).");
		CHECK(manager.get_nb_modules() == 0);
		manager.add_module<ModuleTest>(5);
		CHECK(manager.get_nb_modules() == 1);
		CHECK(manager.has_module<ModuleTest>());
		CHECK(manager.has_module(5));
		REQUIRE_THROWS_WITH(manager.add_module<ModuleTest>(5), "Double assignment of the module n°5.");
		REQUIRE_THROWS_WITH(manager.add_module<ModuleTest>(8), "Double assignment of the module type: 10ModuleTest");
		manager.add_module<Servos2019>(6);
		CHECK(manager.get_nb_modules() == 2);

		REQUIRE_THROWS_WITH(manager.get_module_by_id(42), "Impossible to get module n°42 (> 16).");
		REQUIRE_THROWS_WITH(manager.get_module_by_id(8), "The module n°8 doesn't exist.");
		REQUIRE_NOTHROW(manager.get_module_by_id(5));

		// ne compile pas car 'int' n'hérite pas de BaseModule
		// CHECK_NOTHROW(manager.get_module<int>());
		REQUIRE_NOTHROW(manager.get_module<ModuleTest>());
		CHECK(manager.get_module<ModuleTest>().get_id() == 5);
		CHECK(manager.get_module<Servos2019>().get_id() == 6);
	}

	SECTION("Frame manipulation") {
		ModuleManager manager;
		manager.add_module<ModuleTest>(5);
		auto& module_servo = manager.add_module<Servos2019>(15);
		module_servo.add_servo(2, 50_deg);

		SECTION("GlobalFrame write_frame()") {
			auto frame = manager.write_frame();
			const uint8_t wanted_size = static_cast<uint8_t>(6) + manager.get_module<Servos2019>().get_frame_size();
			REQUIRE(frame.getNbDonnees() == wanted_size);
			const uint8_t* array = frame.getDonnees();
			// ID n°5 : ModuleTest  v
			CHECK(array[0] == 0b00001000);
			// ID n°15 : ModuleServos v
			CHECK(array[1] == 0b00000010);
		}

		SECTION("update_all") {
			// TODO
		}
	}
}
