//
// Created by terae on 17/09/18.
//

#ifndef ROOT_AVOIDANCE2019_H
#define ROOT_AVOIDANCE2019_H

#include "SharedWithRust.h"

#include "Module.hpp"

namespace Commun {

	class Avoidance2019 final : public Module<SharedAvoidance2019> {
	public:
		explicit Avoidance2019(uint8_t id);

		Angle get_angle_detection_adversary() const;

		/**
		 * Fonction permettant de modifier l'angle de détection de l'adversaire
		 * Attention: il faut donner la taille de la demi-zone
		 *
		 * ex: pour avoir un cone de +/- 0.25_PI, donner 0.25_PI
		 * default : +/- 0.25_PI
		 */
		void set_angle_detection_adversary(Angle angle);

		void set_position_turret(Vector2m position);

		Vector2m get_position_turret() const;

		bool is_adversary_detected() const;

		inline uint8_t get_frame_size() const override;

	private:
		SharedAvoidance2019 generate_shared() const override;
		void message_processing(const SharedAvoidance2019&) override;

		void deactivation() override {}

		std::atomic<Angle> _angle_detection_adversary;
		Vector2m _position_turret;
		std::atomic_bool _adversary_detected;
	};
} // namespace Commun


#endif // ROOT_AVOIDANCE2019_H
