//
// Created by terae on 9/10/18.
//

#ifndef ROOT_MODULEMOTEURS_H
#define ROOT_MODULEMOTEURS_H

#include "Module.hpp"

namespace PhysicalRobot {

	class Motors final : public Module {
	public:
		enum RotatingDirection : uint8_t { SCHEDULE = 0, TRIGONOMETRIC = 1 };

		static const uint8_t ID_MAX_CONTROLLED_MOTORS = std::numeric_limits<uint8_t>::max();
		static const uint8_t ID_MAX_UNCONTROLLED_MOTORS = std::numeric_limits<uint8_t>::max();
		static const uint8_t ID_MAX_BRUSHLESS = std::numeric_limits<uint8_t>::max();

		explicit Motors(uint8_t id) : Module(id, "Motors") {}

		void add_controlled_motor(uint8_t id, RotatingDirection);
		void add_uncontrolled_motor(uint8_t id);
		void add_brushless(uint8_t id);

		uint8_t get_nbr_controlled() const;
		uint8_t get_nbr_uncontrolled() const;
		uint8_t get_nbr_brushless() const;

		void set_position_angle(uint8_t motor, Angle);

		void set_position_turns(uint8_t motor, uint8_t nb_turns, RotatingDirection);

		void stop_controlled_motor(uint8_t motor);

		bool is_position_finished(uint8_t motor) const;

		void activate_uncontrolled_motor(uint8_t motor, RotatingDirection);

		void deactivate_uncontrolled_motor(uint8_t motor);

		void activate_brushless(uint8_t motor);

		void deactivate_brushless(uint8_t motor);

	private:
		// Retourne l'index du moteur associé à `id`, lève une exception si le moteur n'a pas été déclaré
		uint8_t test_is_controlled_ok(uint8_t id) const;
		uint8_t test_is_uncontrolled_ok(uint8_t id) const;
		uint8_t test_is_brushless_ok(uint8_t id) const;

		uint8_t get_index_of_controlled(uint8_t id) const;
		uint8_t get_index_of_uncontrolled(uint8_t id) const;
		uint8_t get_index_of_brushless(uint8_t id) const;

		std::vector<JSON> generate_list_jsons() const override;
		void message_processing(const JSON&) override;

		void deactivation() override;

		struct ControlledMotor {
			ControlledMotor(uint8_t id, RotatingDirection rotation)
			        : id(static_cast<uint8_t>(id > 0 ? id : throw std::runtime_error("ID equals to 0.")))
			        , wanted_position(0_deg)
			        , wanted_nb_turns(0)
			        , finished(true)
			        , rotation(rotation) {}

			const uint8_t id;
			std::atomic<Angle> wanted_position;
			std::atomic_uint8_t wanted_nb_turns;
			std::atomic_bool finished;
			std::atomic<RotatingDirection> rotation;
		};

		struct UncontrolledMotor {
			explicit UncontrolledMotor(uint8_t id)
			        : id(static_cast<uint8_t>(id > 0 ? id : throw std::runtime_error("ID equals to 0.")))
			        , on_off(false)
			        , rotation(TRIGONOMETRIC) {}

			const uint8_t id;
			std::atomic_bool on_off;
			std::atomic<RotatingDirection> rotation;
		};

		struct Brushless {
			explicit Brushless(uint8_t id)
			        : id(static_cast<uint8_t>(id > 0 ? id : throw std::runtime_error("ID equals to 0."))), on_off(false) {}

			const uint8_t id;
			std::atomic_bool on_off;
		};

		std::unique_ptr<ControlledMotor> _controlled[ID_MAX_CONTROLLED_MOTORS];
		std::unique_ptr<UncontrolledMotor> _uncontrolled[ID_MAX_UNCONTROLLED_MOTORS];
		std::unique_ptr<Brushless> _brushless[ID_MAX_BRUSHLESS];
	};

} // namespace PhysicalRobot

#endif // ROOT_MODULEMOTEURS_H
