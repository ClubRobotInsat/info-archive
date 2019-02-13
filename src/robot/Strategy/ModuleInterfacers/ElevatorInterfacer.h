//
// Created by terae on 22/01/19.
//

#ifndef ROOT_ELEVATOR_H
#define ROOT_ELEVATOR_H

#include "Interfacer.hpp"

namespace Strategy {
	namespace Interfacer {
		class ElevatorInterfacer final : public AbstractInterfacer {
			static const Duration TIMEOUT;
			/**
			 * Correspond à l'offset de l'ascenseur = ce qui est rajouté de base à tous ses déplacements
			 */
			static const Angle OFFSET;
			/**
			 * Correspond à la hauteur d'un élément porté par les ascenseurs en roulant
			 */
			static const Distance HAUTEUR_AVANCE;

		public:
			using RotatingDirection = PhysicalRobot::Motors::RotatingDirection;

			using interfaced_type = PhysicalRobot::Motors;

			/**
			 * Construit l'ascenseur. Les angles des positions doivent être dans l'ordre croissant
			 * i.e., on commence en position 0, incrémenter va aller en position 1, puis décrémenter va aller en position 0.
			 */
			ElevatorInterfacer(PhysicalRobot::Motors& module, const std::vector<Angle>& positions, uint8_t id_motor);
			ElevatorInterfacer(std::shared_ptr<PhysicalRobot::Robot> robot, const std::vector<Angle>& positions, uint8_t id_motor);
			ElevatorInterfacer(PhysicalRobot::Robot& robot, const std::vector<Angle>& positions, uint8_t id_motor);

			virtual ~ElevatorInterfacer() = default;

			/**
			 * Balance une exception en cas de dépassement de bornes.
			 */
			ActionResult increment();
			ActionResult decrement();

			std::size_t position() const {
				return _position;
			}

			bool is_moving() {
				bool moves = true;
				if(_mutex_moves.try_lock()) {
					_mutex_moves.unlock();
					moves = false;
				}
				return moves;
			}

			ActionResult init(RotatingDirection sens);

			void set_offset(Angle offset) {
				_offset = offset;
			}

			ActionResult set_angle_blocking(Angle angle);
			Angle get_angle() const;

		protected:
			ActionResult set_elevator_blocking(std::size_t pos);
			ActionResult set_elevator(std::size_t pos);
			ActionResult wait_for_elevator();
			bool verify_elevator_position() const;
			bool verify_elevator_blocking() const;

			std::size_t _position = 0;

			PhysicalRobot::Motors& _module;
			const std::vector<Angle>& _positions;
			std::mutex _mutex_moves;
			Angle _offset;

			uint8_t _id_motor;
		};
	} // namespace Interfacer
} // namespace Strategy


#endif // ROOT_ELEVATOR_H
