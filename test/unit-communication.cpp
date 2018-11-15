//
// Created by terae on 17/09/18.
//

#include "catch.hpp"

#include "../src/robot/Communication/Communicator.h"
#include "../src/robot/Modules/ModuleManager.h"
#include "communication/GlobalFrame.h"

TEST_CASE("ParsingClassChecker") {
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

template <Communication::SerialProtocolType T>
void symetric_serial_test(Communication::SerialProtocol<T>& rx,
                          Communication::SerialProtocol<T>& tx,
                          const GlobalFrame& frame,
                          std::function<void(Duration)> stop_execution_after,
                          std::atomic_bool& running_execution) {
	REQUIRE(rx.protocol == T);
	REQUIRE(tx.protocol == T);
	std::thread t_send([&]() {
		REQUIRE_NOTHROW(tx.send_frame(frame));
		sleep(100_ms);
		tx.send_frame({});
	});

	std::thread t(stop_execution_after, 90_ms);
	StopWatch sw;
	REQUIRE_THROWS_AS(rx.recv_frame(running_execution, [frame](const GlobalFrame& f) { REQUIRE(frame == f); }),
	                  typename Communication::SerialProtocol<T>::ReceptionAborted);
	CHECK(sw.getElapsedTime() >= 100_ms);
	t_send.join();
	t.join();
}

TEST_CASE("Serial Protocols") {
	std::atomic_bool running_execution = true;
	auto stop_execution_after = [&running_execution](Duration delay) {
		sleep(delay);
		running_execution.exchange(false);
	};

	const GlobalFrame frame{0x5, 0xA4};

	SECTION("Local") {
		asio::io_service io_service;
		// listen for new connection
		asio::ip::tcp::acceptor acceptor(io_service, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 4321));
		asio::ip::tcp::socket srv_socket(io_service);

		std::thread srv([&]() {
			acceptor.accept(srv_socket);
			asio::streambuf buf;
			// FIXME : l'appel à `asio::read_some` ne compile pas
			// asio::read_until(socket, buf, std::to_string(0xED));
			// REQUIRE(asio::buffer_cast<const char*>(buf.data()) == std::to_string(0x2468ED));
		});

		Communication::protocol_local p;
		CHECK(p.protocol == Communication::SerialProtocolType::SERIAL_LOCAL);
		p.send_frame({0x24, 0x68, 0xED});

		srv.join();
	}

	SECTION("NullCommunicator") {
		running_execution.exchange(true);

		Communication::protocol_null p;
		REQUIRE(p.protocol == Communication::SerialProtocolType::SERIAL_NULL);

		REQUIRE_NOTHROW(p.send_frame(frame));
		std::thread t(stop_execution_after, 20_ms);
		StopWatch sw;
		REQUIRE_THROWS_AS(p.recv_frame(running_execution, [](const GlobalFrame&) {}), Communication::protocol_null::ReceptionAborted);
		CHECK(sw.getElapsedTime() >= 20_ms);
		t.join();
	}

	SECTION("PIPES") {
		running_execution.exchange(true);

		Communication::protocol_pipes rx("/tmp/write.pipe", "/tmp/read.pipe");
		Communication::protocol_pipes tx("/tmp/read.pipe", "/tmp/write.pipe");

		symetric_serial_test(rx, tx, frame, stop_execution_after, running_execution);
	}

	SECTION("RS232") {
		running_execution.exchange(true);
		// TODO
	}

	SECTION("TCPIP") {
		running_execution.exchange(true);
		// TODO
	}

	SECTION("UDP") {
		SECTION("Bad initialization") {
			// Pas de privilège suffisant pour binder le port 10 en réception
			REQUIRE_THROWS_WITH(Communication::protocol_udp("127.0.0.1", 10, 80),
			                    "Failed to bind the receiving socket with 127.0.0.1:10.");

			REQUIRE_NOTHROW(Communication::protocol_udp("localhost", 1234, 80));

			// Multiple use of the port 1234
			Communication::protocol_udp udp("localhost", 1234, 80);
			REQUIRE_THROWS_WITH(Communication::protocol_udp("localhost", 1234, 80),
			                    "Failed to bind the receiving socket with 127.0.0.1:1234.");
		}

		SECTION("Simple communication") {
			// Initialisation d'une communication one-to-one
			const std::string IP_ADDRESS = "127.0.0.1";
			const uint16_t SENDER_PORT = 1234;
			const uint16_t RECVER_PORT = 40000;

			Communication::protocol_udp sender(IP_ADDRESS, SENDER_PORT, RECVER_PORT);
			Communication::protocol_udp recver(IP_ADDRESS, RECVER_PORT, SENDER_PORT);

			running_execution.exchange(true);
			symetric_serial_test(sender, recver, frame, stop_execution_after, running_execution);
		}
	}
}

TEST_CASE("Multi Serial Protocols") {
	std::atomic_bool running_execution = true;
	auto stop_execution_after = [&running_execution](Duration delay) {
		sleep(delay);
		running_execution.exchange(false);
	};

	SECTION("Ethernet") {
		Communication::protocol_ethernet client({{5, "127.0.0.1", 5555, 40005}, {3, "127.0.0.1", 3333, 40003}});
		Communication::protocol_ethernet server({{5, "127.0.0.1", 40005, 5555}, {3, "127.0.0.1", 40003, 3333}});

		REQUIRE_THROWS_WITH(client.send_frame({2}), "Impossible to send a frame to the module n°2: the serial connexion doesn't exist.");

		const GlobalFrame frame_3{3, 0xDE, 0xAD, 0xBE, 0xEF};
		const GlobalFrame frame_5{5, 0x21, 0x43, 0x65, 0x87, 0x90};

		std::thread t_send([&]() {
			// Envoi sur la liaison UDP 3
			REQUIRE_NOTHROW(client.send_frame(frame_3));
			// Envoi sur la liaison UDP 5
			REQUIRE_NOTHROW(client.send_frame(frame_5));

			// Le serveur doit s'arrêter après 90ms ; il est en attente bloquante sur tous ses threads de réception
			// et l'envoi d'un message sur le seul thread `id 3` permets de la down
			sleep(100_ms);
			client.send_frame({3});
		});

		std::thread t(stop_execution_after, 90_ms);
		StopWatch sw;
		REQUIRE_THROWS_AS(server.recv_frame(running_execution,
		                                    [frame_3, frame_5](const GlobalFrame& f) {
			                                    if(f.getDonnee(0) == 3) {
				                                    REQUIRE(f == frame_3);
			                                    } else if(f.getDonnee(0) == 5) {
				                                    REQUIRE(f == frame_5);
			                                    } else {
				                                    FAIL("Unknown module id.");
			                                    }
		                                    }),
		                  Communication::protocol_udp::ReceptionAborted);
		CHECK(sw.getElapsedTime() >= 100_ms);

		t_send.join();
		t.join();
	}
}
