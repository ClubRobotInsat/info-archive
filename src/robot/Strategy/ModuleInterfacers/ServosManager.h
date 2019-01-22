//
// Created by terae on 21/01/19.
//

#ifndef ROOT_INTERFACER_SERVOS_H
#define ROOT_INTERFACER_SERVOS_H

#include "../../Modules/Servos.h"
#include "../../Robot.h"
#include "Interfacer.hpp"
#include "ServosManager.h"

namespace PhysicalRobot {
	class Servos;
}

namespace Strategy {
	namespace Interfacer {
		class AbstractInterfacer;

		class ServosManager final : public AbstractInterfacer {
		public:
			using servo_t = PhysicalRobot::Servos::servo_t;

			using interfaced_type = PhysicalRobot::Servos;

			/**
			 * Toutes les fonctions publiques sont bloquantes.
			 */
			ServosManager(PhysicalRobot::Servos& module_servos, std::function<Angle(servo_t, uint8_t)> get_servo_position);
			ServosManager(PhysicalRobot::Robot& robot, std::function<Angle(servo_t, uint8_t)> get_servo_position);

			/**
			 * Positionne le servomoteur à la position demandée en gérant les blocages
			 */
			ActionResult set_position(servo_t, Angle position);

			interfaced_type* operator->() {
				return &_module;
			}

		private:
			interfaced_type& _module;
			std::function<Angle(servo_t, uint8_t)> _get_servo_position;
			Angle _offset[PhysicalRobot::Servos::ID_MAX_SERVOS];
		};
	} // namespace Interfacer
} // namespace Strategy

#endif // ROOT_INTERFACER_SERVOS_H
