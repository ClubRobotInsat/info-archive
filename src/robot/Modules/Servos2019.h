/**
 * Représentation d'un servo-moteur depuis l'année 2019
 *
 * Le manuel des servos moteurs se trouve [ici](https://www.robotshop.com/media/files/pdf/manual-drs-0101.pdf)
 * Pour un soucis de simplicité dans le code C, l'ID 0 des servos n'est pas addressable,
 * il est considéré comme non existant (pbs de tableaux statiques en C, tout ça...)
 *
 * L'interface fournie est non bloquante mais thread-safe :
 * on peut demander au servo X d'aller à l'angle A, puis faire autre chose
 * et l'élec s'occupe d'amener le servo à la bonne position pendant un temps indéterminé
 * Ce principe s'applique aussi pour tous les autres setteurs
 */

#ifndef ROOT_SERVOS2019_H
#define ROOT_SERVOS2019_H

#include "SharedWithRust.h"

#include "Module.hpp"

namespace PhysicalRobot {

	class Servos2019 final : public Module<SharedServos2019> {
	public:
		enum BlockingMode : uint8_t { HOLD_ON_BLOCKING = 1, UNBLOCKING = 0 };

		enum Color : uint8_t {
			BLACK = 0x00,
			RED = 0x01,
			GREEN = 0x02,
			YELLOW = 0x03,
			BLUE = 0x04,
			MAGENTA = 0x05,
			CYAN = 0x06,
			WHITE = 0x07,

			NBR
		};

		static const int NB_MAX_SERVOS = MAX_SERVOS;

		void add_servo(uint8_t id, Angle start_position, BlockingMode = UNBLOCKING);

		uint8_t get_nbr_servos() const;

		explicit Servos2019(uint8_t id) : Module(id, servo_read_frame, servo_write_frame) {}

		uint8_t get_frame_size() const override;

		void set_position(uint8_t servo, Angle);

		void set_speed(uint8_t servo, uint8_t speed);

		Angle read_position(uint8_t servo) const;

		void set_color(uint8_t servo, Color);

		void set_blocking_mode(uint8_t servo, BlockingMode);

		bool is_blocking(uint8_t servo) const;

		bool is_moving_done(uint8_t servo) const;

	private:
		// Retourne 'true' si le servo associé à 'id' a été déclaré précédemment
		bool is_servo_ok(uint8_t id) const;

		SharedServos2019 generate_shared() const override;
		void message_processing(const SharedServos2019&) override;

		void deactivation() override {}

		struct Servo {
			Servo(uint8_t id, Angle start_position, BlockingMode mode)
			        : id(id)
			        , position(start_position)
			        , wanted_position(start_position)
			        , speed(30)
			        , // TODO : quelle valeur pour la vitesse ?
			        blocked(false)
			        , blocking_mode(mode)
			        , color(YELLOW) {}

			const uint8_t id;

			Angle position;
			Angle wanted_position;
			std::atomic_uint8_t speed;
			std::atomic_bool blocked;
			BlockingMode blocking_mode;
			std::atomic<Color> color;
		};

		std::unique_ptr<Servo> _servos[NB_MAX_SERVOS];
	};
} // namespace PhysicalRobot


#endif // ROOT_SERVOS2019_H
