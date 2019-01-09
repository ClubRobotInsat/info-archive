//
// Created by terae on 16/09/18.
//

#ifndef ROOT_MODULEMOVING_H
#define ROOT_MODULEMOVING_H

#include "MathToolbox/Repere.h"
#include "Module.hpp"

#include <atomic>

enum class SensAvance : uint8_t { Arriere = 0, Avant = 1 };

enum class SensRotation : uint8_t { Horaire = 0, Trigo = 1 };

inline std::ostream& operator<<(std::ostream& os, const SensAvance& sens) {
	os << (sens == SensAvance::Avant ? "avant" : "arrière");
	return os;
}

inline std::ostream& operator<<(std::ostream& os, const SensRotation& sens) {
	os << (sens == SensRotation::Trigo ? "trigo" : "horaire");
	return os;
}

using namespace repere;

namespace PhysicalRobot {
	ENUM_CLASS_NS(PhysicalRobot, MovingCommand, GoForward, GoBackward, TurnRelative, TurnAbsolute, DoNothing, EmergencyStop, Stop);

	class Moving final : public Module {
	public:
		explicit Moving(uint8_t id) : Module(id) {}

		void forward(Distance distance);

		void backward(Distance distance);

		void stop();

		Coordinates get_coordinates() const;

		const Repere& get_reference() const {
			return REFERENCE;
		}

	protected:
		std::vector<JSON> generate_list_jsons() const override;

		void message_processing(const JSON&) override;

		void deactivation() override;

	private:
		static constexpr const Repere& REFERENCE = ABSOLUTE_REFERENCE;

		/// x, y, angle
		Coordinates _coords;

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
