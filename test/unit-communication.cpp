//
// Created by terae on 17/09/18.
//

#include "catch.hpp"

#include "../src/robot/Communication/Communicator.h"
#include "../src/robot/Modules/ModuleManager.h"
#include "communication/GlobalFrame.h"

TEST_CASE("Communication between info and elec") {
	SECTION("Validity test of the 'parses_frames' helper struct.") {
		struct Ok {
			void read_frame(const GlobalFrame&) {}
			std::optional<GlobalFrame> write_frame() const {
				return GlobalFrame{};
			}
		};

		// pas bon à cause de 'read_frame'
		struct Nok1 {
			// pas le bon retour
			int read_frame(const GlobalFrame&) {
				return 1;
			}
			std::optional<GlobalFrame> write_frame() const {
				return GlobalFrame{};
			}
		};
		struct Nok2 {
			// pas les bons arguments
			void read_frame(int) {}
			std::optional<GlobalFrame> write_frame() const {
				return GlobalFrame{};
			}
		};
		struct Nok3 {
			// pas le bon nom de fonction
			void other_function_name(const GlobalFrame&) {}
			std::optional<GlobalFrame> write_frame() const {
				return GlobalFrame{};
			}
		};

		// pas bon à cause de 'write_frame'
		struct Nok4 {
			void read_frame(const GlobalFrame&) {}
			// pas le bon retour
			int write_frame() const {
				return 1;
			}
		};
		struct Nok5 {
			void read_frame(const GlobalFrame&) {}
			// pas les bons arguments
			std::optional<GlobalFrame> write_frame(std::string) const {
				return GlobalFrame{};
			}
		};
		struct Nok6 {
			void read_frame(const GlobalFrame&) {}
			// pas le bon nom
			std::optional<GlobalFrame> other_function_name() const {
				return GlobalFrame{};
			}
		};

		CHECK(Communication::parses_frames<Ok>::value);
		CHECK_FALSE(Communication::parses_frames<Nok1>::value);
		CHECK_FALSE(Communication::parses_frames<Nok2>::value);
		CHECK_FALSE(Communication::parses_frames<Nok3>::value);
		CHECK_FALSE(Communication::parses_frames<Nok4>::value);
		CHECK_FALSE(Communication::parses_frames<Nok5>::value);
		CHECK_FALSE(Communication::parses_frames<Nok6>::value);

		CHECK(Communication::parses_frames<PhysicalRobot::ModuleManager>::value);

		Communication::Communicator<Ok> c1(std::make_shared<Ok>());
		Communication::Communicator<PhysicalRobot::ModuleManager> c2(std::make_shared<PhysicalRobot::ModuleManager>());

		Communication::Communicator<Nok1> c3(std::make_shared<Nok1>());
		// ne compile pas : Nok1 ne correspond pas aux critères de parsing
		// c3.connect({});
	}
}

TEST_CASE("UDP connection") {
	SECTION("Bad initialization") {
		// Pas de privilège suffisant pour binder le port 10 en réception
		REQUIRE_THROWS_WITH(Communication::UDP("127.0.0.1", 10, 80), "Failed to bind the receiving socket with 127.0.0.1:10.");

		REQUIRE_NOTHROW(Communication::UDP("localhost", 1234, 80));

		// Multiple use of the port 1234
		Communication::UDP udp("localhost", 1234, 80);
		REQUIRE_THROWS_WITH(Communication::UDP("localhost", 1234, 80), "Failed to bind the receiving socket with 127.0.0.1:1234.");
	}

	SECTION("Simple communication") {
		// Initialisation d'une communication one-to-one
		const std::string IP_ADDRESS = "127.0.0.1";
		const uint16_t SENDER_PORT = 1234;
		const uint16_t RECVER_PORT = 40000;

		Communication::UDP sender(IP_ADDRESS, SENDER_PORT, RECVER_PORT);
		Communication::UDP recver(IP_ADDRESS, RECVER_PORT, SENDER_PORT);

		const uint8_t BUF_SIZE = 10;
		uint8_t buf_write[BUF_SIZE];
		for(uint8_t i = 0; i < BUF_SIZE; ++i) {
			buf_write[i] = i;
		}

		// Simulation de l'envoi de données
		std::thread t_send([&sender, buf_write]() {
			sender.write_byte(0x42);
			sender.write_bytes(buf_write, BUF_SIZE);
		});

		// Un seul octet transmis
		REQUIRE(recver.read_byte() == 0x42);

		// Un tableau d'octets transmis
		uint8_t buf_read[BUF_SIZE];
		recver.read_bytes(buf_read, BUF_SIZE);
		REQUIRE(std::equal(std::begin(buf_write), std::end(buf_write), std::begin(buf_read)));

		t_send.join();
	}
}

TEST_CASE("Ethernet communication") {
	using namespace PhysicalRobot;
	using namespace Communication;

	// TODO
}