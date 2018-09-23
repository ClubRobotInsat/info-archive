//
// Created by terae on 23/09/18.
//

#include "Constants.h"

Constants::Constants const GLOBAL_CONSTANTS(INI_FILE_PATH);

namespace Constants {

	PrimaryRobot::PrimaryRobot(Ini& reader) {
		_start_angle = Angle::makeFromDeg(reader["robot.principal"]["angle"]);
		_start_position = {
		    Distance::makeFromMm(reader["robot.principal"]["position_x"]),
		    Distance::makeFromMm(reader["robot.principal"]["position_y"]),
		    Distance::makeFromMm(reader["robot.principal"]["position_z"]),
		};
		auto it = reader.sections.find("robot.principal.modules");
		for(auto p : *it->second) {
			_modules[p.first] = p.second;
		}

		_linear_speed = Speed::makeFromCm_s(reader.get("robot", "linear_speed", 400));
		_angular_speed = AngularSpeed::makeFromRad_s(reader.get("robot", "angular_speed", 4));
		_linear_precision = Distance::makeFromMm(reader.get("robot", "linear_precision", 40));
		_angular_precision = Angle::makeFromDeg(reader.get("robot", "angular_precision", 4));

		_turret_position = {
		    Distance::makeFromMm(reader["robot.principal"]["turret_position_x"]),
		    Distance::makeFromMm(reader["robot.principal"]["turret_position_y"]),
		};

		_radius_rotation = Distance::makeFromMm(reader.get("robot.principal", "radius_rotation", 250));

		_size = {
		    Distance::makeFromMm(reader.get("robot.principal", "size_x", 300)),
		    Distance::makeFromMm(reader.get("robot.principal", "size_y", 300)),
		    Distance::makeFromMm(reader.get("robot.principal", "size_z", 420)),
		};
	}

	Constants::Constants(std::string ini_path) : _reader(ini_path, false) {
		assert(_reader.parse());
		primary = std::unique_ptr<PrimaryRobot>(new PrimaryRobot(_reader));
		_TCPIP_port_simu = _reader.get("constants", "TCPIP_port", 4321);
		_table_size = {
		    Distance::makeFromMm(_reader["constants"]["table_x"]),
		    Distance::makeFromMm(_reader["constants"]["table_y"]),
		    0_m,
		};
		_match_duration = Duration::makeFromS(_reader.get("constants", "match_duration", 100));
	}
} // namespace Constants
