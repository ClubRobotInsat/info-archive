//
// Created by terae on 17/09/18.
//

#include "catch.hpp"

#include "../src/robot/Communication/ElecCommunicator.h"
#include <type_traits>
#include "communication/GlobalFrame.h"

// La variable `_count` est incrémentée à chaque échange
// pour `_count == N`, la trame correspondante est composée d'un octet par valeur entre 0 et N.
class PingPong {
    uint8_t _count = 0;

public:
    PingPong() = default;

    void read_frame(const GlobalFrame& f) {
        logDebug("Inside read_frame; _count = ", static_cast<int>(_count), "; size = ", f.getNbDonnees(), "; f = ", f);
        if (uint16_t size = f.getNbDonnees(); size != _count) {
            //throw std::runtime_error("Size of GlobalFrame does not correspond.");
        }
        auto array = f.getDonnees();
        for (uint8_t i = 0; i < _count; ++i) {
            /*if (array[i] != i) {
                throw std::runtime_error("Frame construction is bad.");
            }*/
        }
        _count++;
    }

    GlobalFrame write_frame(uint8_t) const {
        logDebug("Inside write_frame with _count = ", _count);
        GlobalFrame f;
        for (uint8_t i = 0; i < _count; ++i) {
            f.addByte(i);
        }
        return f;
    }
};

TEST_CASE("Communication between info and elec") {
	SECTION("Validity test of the 'parses_frames' helper struct.") {
		struct Ok {
			void read_frame(const GlobalFrame&) {}
			GlobalFrame write_frame(uint8_t) const {
				return GlobalFrame{};
			}
		};

		// pas bon à cause de 'read_frame'
		struct Nok1 {
			// pas le bon retour
			int read_frame(const GlobalFrame&) {
				return 1;
			}
			GlobalFrame write_frame() const {
				return GlobalFrame{};
			}
		};
		struct Nok2 {
			// pas les bons arguments
			void read_frame(int) {}
			GlobalFrame write_frame() const {
				return GlobalFrame{};
			}
		};
		struct Nok3 {
			// pas le bon nom de fonction
			void other_function_name(const GlobalFrame&) {}
			GlobalFrame write_frame() const {
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
			GlobalFrame write_frame(std::string) const {
				return GlobalFrame{};
			}
		};
		struct Nok6 {
			void read_frame(const GlobalFrame&) {}
			// pas le bon nom
			GlobalFrame other_function_name() const {
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

		Communication::ElecCommunicator<Ok> c1(std::make_shared<Ok>(), 42);
		// ne compile pas : Nok1 ne correspond pas aux critères de parsing
		// Commun::ElecCommunicator<Nok1> c3(std::make_shared<Nok1>(), 42);
		Communication::ElecCommunicator<PhysicalRobot::ModuleManager> c2(std::make_shared<PhysicalRobot::ModuleManager>(), 42);
	}
}
