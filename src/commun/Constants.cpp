//
// Created by terae on 23/09/18.
//

#include "Constants.h"

Constants::Constants const GLOBAL_CONSTANTS(
#include "../.robot.ini.includable"
);

namespace Constants {

	int read_field(IniFile& file, std::string section, std::string field, int default_value) {
		auto it = file[section];
		if(it.member(field)) {
			return it[field].asInt();
		} else {
			return default_value;
		}
	}

	Robot::Robot(IniFile& reader, std::string name) {
		const std::string section = "robot." + name;
		_start_angle = Angle::makeFromDeg(reader[section]["angle"].asDouble());
		_start_position = {
		    Distance::makeFromMm(reader[section]["position_x"].asInt()),
		    Distance::makeFromMm(reader[section]["position_y"].asInt()),
		    Distance::makeFromMm(reader[section]["position_z"].asInt()),
		};
		for(auto it : reader) {
			if(it.first == section + ".modules") {
				for(auto field : it.second) {
					_modules[field.first] = static_cast<uint8_t>(field.second.asInt());
				}
			}
		}

		_linear_speed = Speed::makeFromMm_s(read_field(reader, "robot", "linear_speed", 400));
		_angular_speed = AngularSpeed::makeFromRad_s(read_field(reader, "robot", "angular_speed", 4));
		_linear_precision = Distance::makeFromMm(read_field(reader, "robot", "linear_precision", 40));
		_angular_precision = Angle::makeFromDeg(read_field(reader, "robot", "angular_precision", 4));

		_turret_position = {
		    Distance::makeFromMm(reader[section]["turret_position_x"].asInt()),
		    Distance::makeFromMm(reader[section]["turret_position_y"].asInt()),
		};

		_radius_rotation = Distance::makeFromMm(read_field(reader, section, "radius_rotation", 250));

		_size = {
		    Distance::makeFromMm(read_field(reader, section, "size_x", 300)),
		    Distance::makeFromMm(read_field(reader, section, "size_y", 300)),
		    Distance::makeFromMm(read_field(reader, section, "size_z", 420)),
		};
	}

	Constants::Constants(std::string ini_string) : _reader(IniFile('=', '#')) {
		std::stringstream ss;
		ss << ini_string;
		_reader.decode(ss);

		for(auto it : _reader) {
			if(it.first == "robot.list") {
				for(auto robot : it.second) {
					if(robot.second.asBool()) {
						_robots[robot.first] = std::unique_ptr<Robot>(new Robot(_reader, robot.first));
					}
				}
			}
		}

		_TCPIP_port_simu = static_cast<uint16_t>(read_field(_reader, "constants", "TCPIP_port", 4321));
		_table_size = {
		    Distance::makeFromMm(read_field(_reader, "constants", "table_x", 3000)),
		    Distance::makeFromMm(read_field(_reader, "constants", "table_y", 2000)),
		    0_m,
		};
		_scale_environment = Distance::makeFromMm(read_field(_reader, "constants", "scale_environment", 10));
		_table_env_grid = {
		    static_cast<uint16_t>(_table_size.x / _scale_environment),
		    static_cast<uint16_t>(_table_size.y / _scale_environment),
		};

		_match_duration = Duration::makeFromS(read_field(_reader, "constants", "match_duration", 100));
	}

	const Robot& Constants::operator[](const std::string& name) const {
		auto it = _robots.find(name);
		if(it == _robots.cend()) {
			throw std::runtime_error("Constants of the robot '" + name + "' does not exist.");
		}
		return *it->second;
	}
} // namespace Constants
