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
 *
 * Un servo-moteur peut être contrôlé soit en vitesse soit en angle
 */

#ifndef ROOT_SERVOS_H
#define ROOT_SERVOS_H

#include "Module.hpp"
#include <variant>

namespace PhysicalRobot {

	class Servos final : public Module<SharedServos2019> {
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

		using servo_t = uint8_t;

		static const servo_t ID_MAX_SERVOS = std::numeric_limits<uint8_t>::max();

		void add_servo(servo_t, BlockingMode = UNBLOCKING);

		uint8_t get_nbr_servos() const;

		explicit Servos(servo_t id) : Module(id, servo_read_frame, servo_write_frame) {}

		uint8_t get_frame_size() const override;

		void set_position(servo_t, Angle);

		void set_speed(servo_t, uint16_t speed);

		Angle read_position(servo_t) const;

		void set_color(servo_t, Color);

		void set_blocking_mode(servo_t, BlockingMode);

		BlockingMode get_blocking_mode(servo_t);

		bool is_blocking(servo_t) const;

		bool is_moving_done(servo_t) const;

		static uint16_t angle_to_uint16t(Angle);

		static Angle uint16t_to_angle(uint16_t pos);

	private:
		// Retourne l'index associé au mapping du servo `id`. Si l'`id` est mauvais, retourne NB_MAX_SERVOS.
		uint8_t get_index_of(servo_t) const;

		SharedServos2019 generate_shared() const override;
		void message_processing(const SharedServos2019&) override;

		void deactivation() override;

		struct Servo {
			enum CommandType { POSITION = 0, SPEED = 1 };

			// Par défaut, un servo est commandé en vitesse (0 rad/s).
			Servo(servo_t id, BlockingMode mode)
			        : id(static_cast<servo_t>(id > 0 ? id : throw std::runtime_error("ID equals to 0.")))
			        , position(0_deg)
			        , command(0)
			        , command_type(CommandType::SPEED)
			        , blocked(false)
			        , blocking_mode(mode)
			        , color(YELLOW) {}

			const servo_t id;

			Angle position;
			std::variant<Angle, uint16_t> command;
			std::atomic<CommandType> command_type;
			std::atomic_bool blocked;
			BlockingMode blocking_mode;
			std::atomic<Color> color;
		};

		std::unique_ptr<Servo> _servos[ID_MAX_SERVOS];
	};
} // namespace PhysicalRobot


#endif // ROOT_SERVOS_H
