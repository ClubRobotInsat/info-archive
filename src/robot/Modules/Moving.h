//
// Created by terae on 16/09/18.
//

#ifndef ROOT_MODULEMOVING_H
#define ROOT_MODULEMOVING_H

#include "MathToolbox/Repere.h"
#include "Module.hpp"

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

	class Moving final : public Module<SharedMoving2019> {
	public:
		explicit Moving2019(uint8_t id);

		Speed get_linear_speed() const;
		AngularSpeed get_angular_speed() const;

		void set_linear_speed(Speed);
		void set_angular_speed(AngularSpeed);

		Coordonnees get_coordonnees() const;

		const Repere& get_reference() const {
			return REFERENCE;
		}

		// TODO
		uint8_t get_frame_size() const override;

	private:
		SharedMoving2019 generate_shared() const override;
		void message_processing(const SharedMoving2019&) override;

		void deactivation() override;

		static constexpr const Repere& REFERENCE = ABSOLUTE_REFERENCE;
		Coordonnees _coords;

		mutable std::mutex _mutex_speed;
		Speed _linear_speed;
		AngularSpeed _angular_speed;
	};
} // namespace PhysicalRobot


#endif // ROOT_MODULEMOVING_H
