//
// Created by terae on 17/09/18.
//

#include "unit.cpp"

#include "../src/robot/Communication/ElecCommunicator.h"
#include <type_traits>

TEST_CASE("Communication between info and elec") {
	SECTION("Validity test of the 'parses_frames' helper struct.") {
		struct Ok {
			void read_frame(const GlobalFrame&) {}
			GlobalFrame write_frame() const {
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
			GlobalFrame write_frame(int) const {
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

		CHECK(Commun::parses_frames<Ok>::value);
		CHECK_FALSE(Commun::parses_frames<Nok1>::value);
		CHECK_FALSE(Commun::parses_frames<Nok2>::value);
		CHECK_FALSE(Commun::parses_frames<Nok3>::value);
		CHECK_FALSE(Commun::parses_frames<Nok4>::value);
		CHECK_FALSE(Commun::parses_frames<Nok5>::value);
		CHECK_FALSE(Commun::parses_frames<Nok6>::value);

		CHECK(Commun::parses_frames<Commun::ModuleManager>::value);

		Commun::ElecCommunicator<Ok> c1(std::make_shared<Ok>(), 42);
		// ne compile pas : Nok1 ne correspond pas aux critères de parsing
		// Commun::ElecCommunicator<Nok1> c3(std::make_shared<Nok1>(), 42);
		Commun::ElecCommunicator<Commun::ModuleManager> c2(std::make_shared<Commun::ModuleManager>(), 42);
	}
}
