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

#ifdef TEST_SERVOS
#define SERVOS_TEST_ACCESS public
#else
#define SERVOS_TEST_ACCESS private
#endif

namespace PhysicalRobot {
	ENUM_CLASS_NS(PhysicalRobot, BlockingMode, Unblocking, HoldOnBlock);
	ENUM_CLASS_NS(PhysicalRobot, Color, Black, Red, Green, Yellow, Blue, Magenta, Cyan, White);
	ENUM_CLASS_NS(PhysicalRobot, Rotation, Clockwise, CounterClockwise);
	ENUM_CLASS_NS(PhysicalRobot, CommandType, Position, Speed);

	class Servos final : public Module {
	public:
		using servo_t = uint8_t;

		static const servo_t ID_MAX_SERVOS = std::numeric_limits<uint8_t>::max();

		explicit Servos(servo_t id) : Module(id, "Servos") {}

		void add_servo(servo_t id, BlockingMode = BlockingMode::Unblocking);

		uint8_t get_nbr_servos() const;

		explicit Servos(servo_t id) : Module(id) {}

		void set_position(servo_t, Angle);

		void set_speed(servo_t servo, AngularSpeed speed, Rotation = Rotation::CounterClockwise);

		Angle read_position(servo_t) const;

		void set_color(servo_t, Color);

		void set_blocking_mode(servo_t, BlockingMode);

		BlockingMode get_blocking_mode(servo_t);

		bool is_blocking(servo_t) const;

		bool is_moving_done(servo_t) const;

	private:
		// Retourne l'index associé au mapping du servo `id`. Si l'`id` est mauvais, retourne NB_MAX_SERVOS.
		uint8_t get_index_of(servo_t) const;

		std::vector<JSON> generate_list_jsons() const override;
		void message_processing(const JSON&) override;

		void deactivation() override;

		SERVOS_TEST_ACCESS : static uint16_t angle_to_uint16t(Angle);

		static Angle uint16t_to_angle(uint16_t pos);

		static AngularSpeed uint16_t_to_angular_speed(uint16_t pos);

		static uint16_t angular_speed_to_uint16_t(AngularSpeed as);

	private:
		static constexpr AngularSpeed MAX_SPEED = 361.44_deg_s; // datasheet p9: 60°/0.166s

		struct Servo {
			using CommandPosition = Angle;
			using CommandSpeed = std::pair<AngularSpeed, Rotation>;

			// Par défaut, un servo est commandé en vitesse (0 rad/s).
			Servo(servo_t id, BlockingMode mode)
			        : id(static_cast<servo_t>(id > 0 ? id : throw std::runtime_error("ID equals to 0.")))
			        , position(0_deg)
			        , command(CommandSpeed(0_deg_s, Rotation::CounterClockwise))
			        , command_type(CommandType::Speed)
			        , blocked(false)
			        , blocking_mode(mode)
			        , color(Color::Yellow) {}

			const servo_t id;

			Angle position;
			std::variant<CommandPosition, CommandSpeed> command;
			std::atomic<CommandType> command_type;
			std::atomic_bool blocked;
			BlockingMode blocking_mode;
			std::atomic<Color> color;
		};

		std::unique_ptr<Servo> _servos[ID_MAX_SERVOS];
	};
} // namespace PhysicalRobot


#endif // ROOT_SERVOS_H
