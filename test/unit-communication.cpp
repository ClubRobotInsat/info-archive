//
// Created by terae on 17/09/18.
//

#include "catch.hpp"

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

TEST_CASE("Global Frame") {
	GlobalFrame frame{0xFF, 0x8, 0x3, 0xD};

	SECTION("get raw data array") {
		CHECK(frame.getDonnees()[2] == 0x3);
	}

	SECTION("get one byte of data") {
		CHECK(frame.getNbDonnees() == 4);
		CHECK(frame.getDonnee(2) == 0x3);
	}

	SECTION("get data bit by bit") {
		CHECK(frame.getDonneeBool(2, 1));
		CHECK_FALSE(frame.getDonneeBool(2, 2));
	}

	SECTION("Getters trigger an exception when data id is too high") {
		CHECK_THROWS_AS(frame.getDonnee(4), GlobalFrame::ErreurNumeroDonneeTropGrand);
		CHECK_THROWS_AS(frame.getDonneeBool(4, 1), GlobalFrame::ErreurNumeroDonneeTropGrand);
		CHECK_THROWS_AS(frame.getDonneeBool(1, 8), GlobalFrame::ErreurNumeroBitTropGrand);
	}

	SECTION("set bit by bit") {
		frame.set(2, 2, true);
		CHECK(frame.getDonnee(2) == 0x7);
		CHECK(frame.getDonneeBool(2, 2));
	}

	SECTION("set data to one byte only") {
		frame.setDonnee(0xFD);
		CHECK(frame.getNbDonnees() == 1);
		CHECK(frame.getDonnee(0) == 0xFD);
	}

	SECTION("set array") {
		uint8_t array[] = {0x1, 0x2, 0x6};
		frame.setDonnees(3, array);
		CHECK(frame.getNbDonnees() == 3);
		CHECK(frame.getDonnee(2) == 0x6);
	}

	SECTION("add one byte") {
		frame.addByte(0x1);
		CHECK(frame.getNbDonnees() == 5);
		CHECK(frame.getDonnee(4) == 0x1);
		CHECK(frame.getDonnee(0) == 0xFF);
	}

	SECTION("add several bytes (initializer list version)") {
		frame.addBytes({0x1, 0x2, 0x6});
		CHECK(frame.getNbDonnees() == 7);
		CHECK(frame.getDonnee(4) == 0x1);
		CHECK(frame.getDonnee(6) == 0x6);
		CHECK(frame.getDonnee(0) == 0xFF);
	}

	SECTION("add several bytes (array version)") {
		uint8_t array[] = {0x1, 0x2, 0x6};
		frame.addBytes(3, array);
		CHECK(frame.getNbDonnees() == 7);
		CHECK(frame.getDonnee(4) == 0x1);
		CHECK(frame.getDonnee(6) == 0x6);
		CHECK(frame.getDonnee(0) == 0xFF);
	}

	SECTION("GlobalFrame can and can only contain as much bytes as specified "
		 "in GlobalFrame::DONNEES_TRAME_MAX") {
		uint8_t array[GlobalFrame::DONNEES_TRAME_MAX];
		GlobalFrame specialFrame;

		CHECK_NOTHROW(specialFrame.addBytes(GlobalFrame::DONNEES_TRAME_MAX, array));
		CHECK_THROWS_AS(specialFrame.addBytes(1, array), GlobalFrame::ErreurTropDeDonnees);
	}

	// TODO à tester :
	// - méthode générique "addDonnees"
}
