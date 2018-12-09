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
	ENUM_CLASS_NS(PhysicalRobot, BlockingMode, Unblocking, HoldOnBlock);
	ENUM_CLASS_NS(PhysicalRobot, Color, Black, Red, Green, Yellow, Blue, Magenta, Cyan, White);
	ENUM_CLASS_NS(PhysicalRobot, Rotation, Clockwise, CounterClockwise);
	ENUM_CLASS_NS(PhysicalRobot, CommandType, Position, Speed);

	class Servos final : public Module {
	public:
		static const uint8_t ID_MAX_SERVOS = std::numeric_limits<uint8_t>::max();

		void add_servo(uint8_t id, BlockingMode = BlockingMode::Unblocking);

		uint8_t get_nbr_servos() const;

		explicit Servos(uint8_t id) : Module(id) {}

		void set_position(uint8_t servo, Angle);

		void set_speed(uint8_t servo, uint16_t speed, Rotation = Rotation::CounterClockwise);

		Angle read_position(uint8_t servo) const;

		void set_color(uint8_t servo, Color);

		void set_blocking_mode(uint8_t servo, BlockingMode);

		bool is_blocking(uint8_t servo) const;

		bool is_moving_done(uint8_t servo) const;

		static uint16_t angle_to_uint16t(Angle);

		static Angle uint16t_to_angle(uint16_t pos);

	private:
		// Retourne l'index associé au mapping du servo `id`. Si l'`id` est mauvais, retourne NB_MAX_SERVOS.
		uint8_t get_index_of(uint8_t id) const;

		std::vector<JSON> generate_list_jsons() const override;
		void message_processing(const JSON&) override;

		void deactivation() override;

		struct Servo {
			using CommandPosition = Angle;
			using CommandSpeed = std::pair<uint16_t, Rotation>;

			// Par défaut, un servo est commandé en vitesse (0 rad/s).
			Servo(uint8_t id, BlockingMode mode)
			        : id(static_cast<uint8_t>(id > 0 ? id : throw std::runtime_error("ID equals to 0.")))
			        , position(0_deg)
			        , command(CommandSpeed(0, Rotation::CounterClockwise))
			        , command_type(CommandType::Speed)
			        , blocked(false)
			        , blocking_mode(mode)
			        , color(Color::Yellow) {}

			const uint8_t id;

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
