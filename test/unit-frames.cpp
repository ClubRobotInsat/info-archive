#include "catch.hpp"

#include "communication/GlobalFrame.h"

TEST_CASE("Global Frame", "[integration]") {
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

	SECTION("operator+=") {
		GlobalFrame f{12, 23, 48};
		frame += f;
		CHECK(frame.getNbDonnees() == 7);
		CHECK(frame.getDonnee(0) == 0xFF);
		CHECK(frame.getDonnee(3) == 0xD);
		CHECK(frame.getDonnee(6) == 48);
	}

	// TODO à tester :
	// - méthode générique "addDonnees"
}
