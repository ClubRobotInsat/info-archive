//
// Created by terae on 23/09/18.
//

#ifndef ROOT_CONSTANTS_H
#define ROOT_CONSTANTS_H

#include "Enum/Enum.h"
#include <ini/src/IniFile.hpp>

#include "../Lidar/Driver/lidar.h"
#include "Commun.h"
#include "Json.h"
#include "MathToolbox/Repere.h"
#include "resources/EmbeddedFiles.h"

namespace Constants {
	using namespace ini;

	ENUM_CLASS_NS(Constants,
	              RobotColor,
	              Purple,
	              Yellow,
	              // Valeur d'initialisation, qui permet de déctecter si la lecture du capteur couleur a déconné.
	              Undef)

	inline Constants::RobotColor string_to_color(std::string str) {
		std::transform(str.cbegin(), str.cend(), str.begin(), ::tolower);
		for(auto color : getEnumValues<Constants::RobotColor>()) {
			std::string str_color(toString(color));
			std::transform(str_color.cbegin(), str_color.cend(), str_color.begin(), ::tolower);
			if(str == str_color) {
				return color;
			}
		}
		return Constants::RobotColor::Undef;
	}

	class Constants;
	class RobotInitializationData {
		friend class Constants;
		RobotInitializationData(IniFile& reader, std::string name);

		// Les attributs `optional` correspondent à des valeurs qui ne peuvent pas être entrées par défaut
		std::optional<Vector3m> _start_position;
		std::optional<Angle> _start_angle;

		// Association de chaque nom de module à son ID
		std::map<std::string, uint8_t> _modules;
		Speed _linear_speed;
		AngularSpeed _angular_speed; // unité arbitraire décidée en élec
		Distance _linear_accuracy;
		Angle _angular_accuracy;
		Angle _angle_adversary_detection; // demi-zone de détection
		Vector2m _turret_position;
		Distance _radius_rotation;
		Vector3m _size;
		Lidar::LidarType _lidar_type;
		std::string _protocol_type;
		std::map<std::string, std::string> _communication_arguments;

		std::vector<uint8_t> _list_id_servos;

	public:
		inline Vector3m get_start_position() const {
			return _start_position.value();
		}

		inline Angle get_start_angle() const {
			return _start_angle.value();
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

		inline Distance get_linear_accuracy() const {
			return _linear_accuracy;
		}

		inline Angle get_angular_accuracy() const {
			return _angular_accuracy;
		}

		inline Angle get_angle_adversary_detection() const {
			return _angle_adversary_detection;
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

		inline Lidar::LidarType get_lidar_type() const {
			return _lidar_type;
		}

		inline std::string get_protocol_type() const {
			return _protocol_type;
		}

		inline std::map<std::string, std::string> get_communication_arguments() const {
			return _communication_arguments;
		}

		inline std::vector<uint8_t> get_list_id_servos() const {
			return _list_id_servos;
		}
	};

	class Constants {
		std::map<std::string, std::unique_ptr<RobotInitializationData>> _robots;

	public:
		explicit Constants(const std::string& ini_string);

		const RobotInitializationData& operator[](const std::string& name) const;

		inline Duration get_match_duration() const {
			return _match_duration;
		}

		inline uint16_t get_default_TCPIP_port() const {
			return _TCPIP_port_simu;
		}
		inline Vector3m get_table_size() const {
			return _table_size;
		}
		// Échelle pour créer la discrétisation de l'environnement
		inline Distance get_scale_environment() const {
			return _scale_environment;
		}
		// Nombre de points pour discrétiser la table dans l'environnement du robot (utilisé par l'A*)
		inline Vector2u16 get_table_env_grid() const {
			return _table_env_grid;
		}

		const repere::Repere REFERENCE_ASTAR =
		    repere::Repere({0_m, 0_m}, repere::Multiplier::SENS_POSITIVE, repere::Multiplier::SENS_POSITIVE);

		const repere::Repere& get_reference(RobotColor color) const {
			switch(color) {
				case RobotColor::Yellow:
					return REFERENCE_YELLOW;
				case RobotColor::Purple:
					return REFERENCE_PURPLE;
				default:
					return REFERENCE_ASTAR;
			}
		}

		/// True si les mouvements du robot doivent être inversés quand il a la
		/// couleur passée en paramètres.
		bool is_inverted(RobotColor color) const {
			return color == RobotColor::Purple;
		}

		inline const Duration& get_default_communication_delay() const {
			return _communication_delay;
		}

		// Période maximale pour le renvoi de l'état d'un module
		inline const Duration& get_frame_period() const {
			return _frame_period;
		}

		// Convertir la table du fichier texte en un objet JSON à la pré-compilation

		const JSON& TABLE_2020() const {
			static JSON TABLE_2020 = nlohmann::json::parse(EmbeddedFiles::readText("table_2020.json"));
			return TABLE_2020;
		}

		inline Duration get_lidar_actualization_period() const {
			return _lidar_actualization_period;
		}

		inline Distance get_threshold_adversary_detection() const {
			return _threshold_adversary_detection;
		}

	private:

	    std::string configFolderPath;
		uint16_t _TCPIP_port_simu;
		Vector3m _table_size;

		Distance _scale_environment;
		Vector2u16 _table_env_grid;

		Duration _match_duration;

		// Durée de temporisation entre l'envoi de deux messages successifs sur le médium de communication
		Duration _communication_delay;
		Duration _frame_period;

		Duration _lidar_actualization_period;

		Distance _threshold_adversary_detection;

		IniFile _reader;

		const repere::Repere REFERENCE_YELLOW =
		    repere::Repere({0_m, 0_m}, repere::Multiplier::SENS_POSITIVE, repere::Multiplier::SENS_POSITIVE);

		const repere::Repere REFERENCE_PURPLE =
		    repere::Repere({3_m, 0_m}, repere::Multiplier::SENS_NEGATIVE, repere::Multiplier::SENS_POSITIVE);
	};
} // namespace Constants

inline Constants::RobotColor operator!(Constants::RobotColor const& c) {
	using Constants::RobotColor;

	switch(c) {
		case RobotColor::Purple:
			return RobotColor::Yellow;
		case RobotColor::Yellow:
			return RobotColor::Purple;
		default:
			return RobotColor::Undef;
	}
}

const Constants::Constants& GLOBAL_CONSTANTS();

#endif // ROOT_CONSTANTS_H
