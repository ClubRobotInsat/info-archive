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
		explicit Navigation(uint8_t id) : Module(id, "Navigation") {}

		void forward(Distance distance, SensAdvance);

		void turn_relative(Angle angle);

		void turn_absolute(Angle angle, SensRotation);

		void stop();

		bool is_physically_blocked() const;

		repere::Coordinates get_coordinates() const;

		repere::Position get_position() const;

		repere::Orientation get_orientation() const;

		const repere::Repere& get_reference() const;

		void update_linear_speed(Speed);

		void update_angular_speed(AngularSpeed);

		void update_linear_accuracy(Distance precision);

		void update_angular_accuracy(Angle precision);

		bool is_moving_done() const;

		bool is_precision_reached() const;

	protected:
		std::vector<JSON> generate_list_jsons() const override;

		void message_processing(const JSON&) override;

		void deactivation() override;

	private:
		static constexpr const repere::Repere& REFERENCE = repere::ABSOLUTE_REFERENCE;

		/// x, y, angle
		repere::Coordinates _coords;

		std::atomic_bool _blocked;
		std::atomic_bool _asserv_on_off;
		std::atomic_bool _leds;
		std::atomic_bool _reset;

		MovingCommand _command;
		uint16_t _args_cmd[2];
		uint16_t _counter;
		std::atomic_bool _moving_done;


		void set_command(MovingCommand command);

		uint16_t distance_to_u16(Distance distance) const;

		Distance u16_to_distance(uint16_t) const;

		uint16_t angle_to_u16(Angle angle) const;

		Angle u16_to_angle(uint16_t) const;
	};
} // namespace PhysicalRobot


#endif // ROOT_MODULEMOVING_H
