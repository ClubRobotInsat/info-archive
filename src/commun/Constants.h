//
// Created by terae on 23/09/18.
//

#ifndef ROOT_CONSTANTES_H
#define ROOT_CONSTANTES_H

#include "Enum/Enum.h"
#include <ini/src/IniFile.hpp>

#include "Commun.h"
#include "Json.h"
#include "MathToolbox/Repere.h"

namespace Constants {
	using namespace ini;

	ENUM_CLASS_NS(Constants,
	              RobotColor,
	              Orange,
	              Green,
	              // Valeur d'initialisation, qui permet de déctecter si la lecture du capteur couleur a déconné.
	              Undef)

	class Constants;
	class Robot {
		friend class Constants;
		explicit Robot(IniFile& reader, std::string name);

		Vector3m _start_position;
		Angle _start_angle;
		// Association de chaque nom de module à son ID
		std::map<std::string, uint8_t> _modules;
		Speed _linear_speed;
		AngularSpeed _angular_speed; // unité arbitraire décidée en élec
		Distance _linear_precision;
		Angle _angular_precision;
		Vector2m _turret_position;
		Distance _radius_rotation;
		Vector3m _size;

	public:
		inline Vector3m get_start_position() const {
			return _start_position;
		}

		inline Angle get_start_angle() const {
			return _start_angle;
		}

		inline std::map<std::string, uint8_t> get_modules() const {
			return _modules;
		}

		inline Speed get_linear_speed() const {
			return _linear_speed;
		}

		inline AngularSpeed get_angular_speed() const {
			return _angular_speed;
		}

		inline Distance get_linear_precision() const {
			return _linear_precision;
		}

		inline Angle get_angular_precision() const {
			return _angular_precision;
		}

		// POSITION_TOURELLE = { décalage avant, décalage vers la droite }
		inline Vector2m get_turret_position() const {
			return _turret_position;
		}

		inline Distance get_radius_rotation() const {
			return _radius_rotation;
		}

		inline Vector3m get_size() const {
			return _size;
		}
	};

	class Constants {
		std::map<std::string, std::unique_ptr<Robot>> _robots;

	public:
		explicit Constants(std::string ini_string);

		const Robot& operator[](const std::string& name) const;

		inline Duration get_match_duration() const {
			return _match_duration;
		}

		inline uint16_t get_default_TCPIP_port() const {
			return _TCPIP_port_simu;
		}
		inline Vector3m get_table_size() const {
			return _table_size;
		}

		const repere::Repere REFERENCE_ASTAR =
		    repere::Repere({0_m, 0_m}, repere::Multiplicateur::SENS_POSITIF, repere::Multiplicateur::SENS_POSITIF);

		// Convertir la table du fichier texte en un objet JSON à la pré-compilation
		const JSON TABLE_2018 = nlohmann::json::parse(
#include ".table_2018.json.includable"
		);

	private:
		uint16_t _TCPIP_port_simu;
		Vector3m _table_size;
		Duration _match_duration;

		IniFile _reader;
	};
} // namespace Constants

inline Constants::RobotColor operator!(Constants::RobotColor const& c) {
	if(c == Constants::RobotColor::Orange)
		return Constants::RobotColor::Green;
	else if(c == Constants::RobotColor::Green)
		return Constants::RobotColor::Orange;

	return c;
}

extern Constants::Constants const GLOBAL_CONSTANTS;

#endif // ROOT_CONSTANTES_H
