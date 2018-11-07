//
// Created by terae on 17/09/18.
//

#include "catch.hpp"

#include "../src/robot/Communication/Communicator.h"
#include "../src/robot/Modules/ModuleManager.h"
#include "communication/GlobalFrame.h"
#include <type_traits>

// La variable `_count` est incrémentée à chaque échange
// pour `_count == N`, la trame correspondante est composée d'un octet par valeur entre 0 et N.
class PingPong {
	uint8_t _count = 0;

public:
	PingPong() = default;

	void read_frame(const GlobalFrame& f) {
		logDebug("Inside read_frame; _count = ", static_cast<int>(_count), "; size = ", f.getNbDonnees(), "; f = ", f);
		if(uint16_t size = f.getNbDonnees(); size != _count) {
			// throw std::runtime_error("Size of GlobalFrame does not correspond.");
		}
		auto array = f.getDonnees();
		for(uint8_t i = 0; i < _count; ++i) {
			/*if (array[i] != i) {
			    throw std::runtime_error("Frame construction is bad.");
			}*/
		}
		_count++;
	}

	GlobalFrame write_frame(uint8_t) const {
		logDebug("Inside write_frame with _count = ", _count);
		GlobalFrame f;
		for(uint8_t i = 0; i < _count; ++i) {
			f.addByte(i);
		}
		return f;
	}
};

TEST_CASE("Communication between info and elec in serial connections") {
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

	/*SECTION("Connection") {

	    auto test = []() {
	        std::atomic_bool stop = false;

	        auto listener = [&stop]() {
	            auto can = std::make_unique<Communication::CAN>(
	                    std::make_unique<Communication::NamedPipe>("/tmp/write.pipe", "/tmp/read.pipe"));
	            PingPong parser;
	            while (!stop) {
	                try {
	                    can->envoyerTrame(parser.write_frame(), true);
	                } catch (std::runtime_error &e) {
	                    logError("Échec de l'envoi de l'état du robot par le module manager !!");
	                    logError("Exception rencontrée : ", e.what());
	                }

	                try {
	                    GlobalFrame frame = {};
	                    can->recevoirTrameBloquant();
	                    parser.read_frame(frame);
	                } catch (std::runtime_error &e) {
	                    logError("Échec de la mise à jour du module manager !!");
	                    logError("Exception rencontrée : ", e.what());
	                }
	            }
	        };

	        Communication::ElecCommunicator<PingPong> communicator(std::make_shared<PingPong>(), 1234);
	        communicator.set_modules_initialized();
	        communicator.connect({"prog", "PIPES"});

	        auto thread = std::thread(listener);

	        sleep(5_s);
	        stop = true;
	        communicator.disconnect();
	        thread.join();
	    };

	    CHECK_NOTHROW(test());
	}*/
}

TEST_CASE("Ethernet") {}