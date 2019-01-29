//
// Created by terae on 21/01/19.
//

#ifndef ROOT_INTERFACER_SERVOS_H
#define ROOT_INTERFACER_SERVOS_H

#include "../../Modules/Servos.h"
#include "Interfacer.hpp"
#include "ServosManager.h"

namespace Strategy {
	namespace Interfacer {

		class ServosManager final : public AbstractInterfacer {
		public:
			using interfaced_type = PhysicalRobot::Servos;

			using servo_t = interfaced_type::servo_t;

			/**
			 * Toutes les fonctions publiques sont bloquantes.
			 */
			explicit ServosManager(interfaced_type& module_servos /*, std::function<Angle(servo_t, uint8_t)> get_servo_position*/);
			explicit ServosManager(std::shared_ptr<PhysicalRobot::Robot> robot /*, std::function<Angle(servo_t, uint8_t)> get_servo_position*/);
			explicit ServosManager(PhysicalRobot::Robot& robot /*, std::function<Angle(servo_t, uint8_t)> get_servo_position*/);

			/**
			 * Positionne le servomoteur à la position demandée en gérant les blocages
			 */
			ActionResult set_position(servo_t, Angle position);

			interfaced_type* operator->();

			/**
			 * Ajoute un offset à la valeur théorique où doit aller le servo
			 */
			void set_offset(servo_t servo, Angle offset);

			Angle get_offset(servo_t servo);

		private:
			interfaced_type& _module;
			std::function<Angle(servo_t, uint8_t)> _get_servo_position;
			Angle _offset[PhysicalRobot::Servos::ID_MAX_SERVOS];
		};
	} // namespace Interfacer
} // namespace Strategy

#endif // ROOT_INTERFACER_SERVOS_H
