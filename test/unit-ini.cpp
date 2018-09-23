//
// Created by terae on 23/09/18.
//

#include "unit.cpp"

#include "../third_parties/iniparser/INI.h"

TEST_CASE(".ini file") {
	INI<std::string, std::string, int> reader("../src/robot.ini", true);

	CHECK(reader.get("robot.principal.modules", "moving", -1) == 1);
	CHECK(reader.get("robot.principal.modules", "servos", -1) == 2);
	CHECK(reader.get("robot.principal.modules", "inexistant", -1) == -1);
	reader["robot.principal.modules"]["other"] = 6;
	// reader.save();

	for(auto i : reader.sections) {
		std::cout << "section: " << i.first << std::endl;
		for(auto j : *i.second)
			std::cout << "    > " << j.first << " = " << j.second << std::endl;
		std::cout << std::endl;
	}
}
