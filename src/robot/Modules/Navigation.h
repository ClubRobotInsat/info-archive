//
// Created by terae on 16/09/18.
//

#ifndef ROOT_MODULEMOVING_H
#define ROOT_MODULEMOVING_H

#include "MathToolbox/Repere.h"
#include "Module.hpp"

#include <atomic>

namespace PhysicalRobot {
	ENUM_CLASS_NS(PhysicalRobot, MovingCommand, GoForward, GoBackward, TurnRelative, TurnAbsolute, DoNothing, EmergencyStop, Stop);
	ENUM_CLASS_NS(PhysicalRobot, SensAdvance, Backward, Forward);
	ENUM_CLASS_NS(PhysicalRobot, SensRotation, Clockwise, Trigo);

	class Navigation final : public Module {
	public:
		explicit Navigation(uint8_t id) : Module(id, "Navigation") {
			_args_cmd[0] = 0;
			_args_cmd[1] = 0;
		}

		void forward(Distance distance, SensAdvance);

		void turn_relative(Angle angle);

		void turn_absolute(Angle angle, SensRotation);

		/** Arrête le robot. Le robot met à jour sa consigne pour qu'elle
		 * corresponde à sa position actuelle. A la difference de emergency_stop(),
		 * la commande donnée au moteurs du robot peut ne pas être nulle. */
		void stop();

		/** Arrêt d'urgence. Tant qu'il reste dans cet état, le robot fournit
		 * une commande nulle aux moteurs. */
		void emergency_stop();

		bool is_physically_blocked() const;

		repere::Coordinates get_coordinates() const;

		repere::Position get_position() const;

		repere::Orientation get_orientation() const;

		Distance get_left_wheel_distance() const;

		Distance get_right_wheel_distance() const;

		void set_coordinates(const repere::Coordinates&);

		const repere::Repere& get_reference() const;

		/** Définit la vitesse maximale du robot lorsqu'il avance ou recule. */
		void update_linear_speed(Speed);

		/** Définit la vitesse angulaire maximale du robot lorsqu'il tourne
		 * sur lui-même. */
		void update_angular_speed(AngularSpeed);

		/** La précision longitutinale indique au robot à partir de quel
		 * moment il peut déterminer que son déplacement longitudinal est
		 * terminé. */
		void update_linear_accuracy(Distance accuracy);

		/** La précision angulaire indique au robot à partir de quel
		 * moment il peut déterminer que son déplacement angulaire est
		 * terminé. */
		void update_angular_accuracy(Angle accuracy);

		bool is_moving_done() const;

		bool is_precision_reached() const;

		void set_linear_asserv_enabled(bool activated);

		void set_angular_asserv_enabled(bool activated);

		void set_asserv_enabled(bool activated);

	protected:
		std::vector<JSON> generate_list_jsons() const override;

		void message_processing(const JSON&) override;

		void deactivation() override;

	private:
		static constexpr const repere::Repere& REFERENCE = repere::ABSOLUTE_REFERENCE;

		/// x, y, angle
		repere::Coordinates _coords;
		/// Distance parcourue par la roue gauche
		Distance _left_dist;
		/// Distance parcourue par la roue droite
		Distance _right_dist;

		Speed _linear_speed;
		AngularSpeed _angular_speed;
		Distance _linear_accuracy;
		Angle _angular_accuracy;

		std::atomic_bool _blocked = false;
		std::atomic_bool _asserv_lin;
		std::atomic_bool _asserv_ang;
		std::atomic_bool _leds;
		std::atomic_bool _reset;

		MovingCommand _command = MovingCommand::DoNothing;
		uint16_t _args_cmd[2];
		uint16_t _counter = 0;
		std::atomic_bool _moving_done;

		void set_command(MovingCommand command);
	};
} // namespace PhysicalRobot

#endif // ROOT_MODULEMOVING_H
