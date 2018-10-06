//
// Created by terae on 9/10/18.
//

#ifndef ROOT_MODULEMOTEURS2019_H
#define ROOT_MODULEMOTEURS2019_H

#include "SharedWithRust.h"

#include "Module.hpp"

namespace PhysicalRobot {

	class Motors2019 final : public Module<SharedMotors2019> {
	public:
		enum RotatingDirection : uint8_t { SCHEDULE = 0, TRIGONOMETRIC = 1 };

		static const int NB_MAX_CONTROLLED_MOTORS = MAX_CONTROLLED_MOTORS;
		static const int NB_MAX_UNCONTROLLED_MOTORS = MAX_UNCONTROLLED_MOTORS;
		static const int NB_MAX_BRUSHLESS = MAX_BRUSHLESS;

		explicit Motors2019(uint8_t id) : Module(id, motor_read_frame, motor_write_frame) {}

		uint8_t get_frame_size() const override;

		void add_controlled_motor(uint8_t id, RotatingDirection);
		void add_uncontrolled_motor(uint8_t id);
		void add_brushless(uint8_t id);

		uint8_t get_nbr_controlled() const;
		uint8_t get_nbr_uncontrolled() const;
		uint8_t get_nbr_brushless() const;

		void position_angle(uint8_t motor, Angle);

		void position_turns(uint8_t motor, uint8_t nb_turns, RotatingDirection);

		bool is_position_finished(uint8_t motor) const;

		void activate_uncontrolled_motor(uint8_t motor, RotatingDirection);

		void deactivate_uncontrolled_motor(uint8_t motor);

		void activate_brushless(uint8_t motor);

		void deactivate_brushless(uint8_t motor);

	private:
		// Retourne 'true' si le moteur associé à 'id' a été déclaré précédemment
		bool test_is_controlled_ok(uint8_t id) const;
		bool test_is_uncontrolled_ok(uint8_t id) const;
		bool test_is_brushless_ok(uint8_t id) const;

		SharedMotors2019 generate_shared() const override;
		void message_processing(const SharedMotors2019&) override;

		void deactivation() override;

		struct ControlledMotor {
			ControlledMotor(uint8_t id, RotatingDirection rotation)
			        : id(id), wanted_position(0_deg), wanted_nb_turns(0), finished(true), rotation(rotation) {}

			const uint8_t id;
			std::atomic<Angle> wanted_position;
			std::atomic_uint8_t wanted_nb_turns;
			std::atomic_bool finished;
			std::atomic<RotatingDirection> rotation;
		};

		struct UncontrolledMotor {
			UncontrolledMotor(uint8_t id) : id(id), on_off(false) {}

			const uint8_t id;
			std::atomic_bool on_off;
			std::atomic<RotatingDirection> rotation;
		};

		struct Brushless {
			Brushless(uint8_t id) : id(id), on_off(false) {}

			const uint8_t id;
			std::atomic_bool on_off;
		};

		std::unique_ptr<ControlledMotor> _controlled[NB_MAX_CONTROLLED_MOTORS];
		std::unique_ptr<UncontrolledMotor> _uncontrolled[NB_MAX_UNCONTROLLED_MOTORS];
		std::unique_ptr<Brushless> _brushless[NB_MAX_BRUSHLESS];
	};
} // namespace PhysicalRobot

#endif // ROOT_MODULEMOTEURS2019_H
