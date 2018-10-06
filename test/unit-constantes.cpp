//
// Created by terae on 01/10/18.
//

#include "catch.hpp"

#include <Constants.h>

TEST_CASE("Global constants") {
	const std::string INI{"[constants]\n"
	                      "TCPIP_port=4242\n"
	                      "match_duration=100\n"
	                      "table_x=1500\n"
	                      "table_y=1000\n"
	                      "scale_environment=5\n"

	                      "[robot]\n"
	                      "linear_speed=376\n"
	                      "linear_precision=26\n"
	                      "angular_speed=10\n"
	                      "angular_precision=6\n"

	                      "[robot.list]\n"
	                      "bot=true\n"
	                      "iron-man=false\n"

	                      "[robot.bot]\n"
	                      "position_x=4\n"
	                      "position_y=42\n"
	                      "position_z=420\n"
	                      "size_x=10\n"
	                      "size_y=20\n"
	                      "size_z=30\n"
	                      "angle=57\n"
	                      "turret_position_x=-30\n"
	                      "turret_position_y=20\n"
	                      "radius_rotation=60\n"

	                      "[robot.bot.modules]\n"
	                      "moving=5\n"
	                      "servos=14"};

	Constants::Constants cst(INI);
	CHECK(cst.get_default_TCPIP_port() == 4242);
	CHECK(cst.get_match_duration() == 100_s);
	CHECK(cst.get_table_size() == Vector3m{1.5_m, 1_m, 0_m});
	CHECK(cst.get_scale_environment() == 5_mm);
	CHECK(cst.get_table_env_grid() == Vector2u16{300, 200});
	REQUIRE_THROWS_WITH(cst["iron-man"], "Constants of the robot 'iron-man' does not exist.");

	REQUIRE_NOTHROW(cst["bot"]);
	auto& bot = cst["bot"];
	CHECK(bot.get_linear_precision() == 2.6_cm);
	CHECK(bot.get_linear_speed() == 37.6_cm_s);
	CHECK(bot.get_angular_precision() == 6_deg);
	CHECK(bot.get_angular_speed() == 10_rad_s);
	CHECK(bot.get_radius_rotation() == 6_cm);
	CHECK(bot.get_start_angle() == 57_deg);
	CHECK(bot.get_start_position() == Vector3m{4_mm, 42_mm, 42_cm});
	CHECK(bot.get_turret_position() == Vector2m{-3_cm, 2_cm});
	CHECK(bot.get_size() == Vector3m{1_cm, 2_cm, 3_cm});

	REQUIRE(bot.get_modules().size() == 2);
	CHECK(bot.get_modules()["moving"] == 5);
	CHECK(bot.get_modules()["servos"] == 14);
}