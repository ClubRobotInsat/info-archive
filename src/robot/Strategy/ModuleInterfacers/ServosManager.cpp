//
// Created by terae on 21/01/19.
//

#include "ServosManager.h"
#include <log/Log.h>

namespace Strategy {
	namespace Interfacer {

		ServosManager::ServosManager(PhysicalRobot::Servos& module_servos, std::function<Angle(uint8_t, uint8_t)> get_servo_position)
		        : AbstractInterfacer(), _module(module_servos), _get_servo_position(get_servo_position) {
			std::fill(_offset, std::end(_offset), 0_deg);
		}

		ServosManager::ServosManager(PhysicalRobot::Robot& robot, std::function<Angle(servo_t, uint8_t)> get_servo_position)
		        : ServosManager(robot.get_module<PhysicalRobot::Servos>(), get_servo_position) {}

		ActionResult ServosManager::set_position(servo_t servo, Angle pos) {
			auto TIMEOUT_SERVO = 1000_ms; // 500_ms;
			auto timeoutDate = TimePoint::now() + TIMEOUT_SERVO;

			// Ajout de l'offset à la position en cours
			pos += _offset[servo];
			Angle old_pos = _module.read_position(servo);
			_module.set_position(servo, pos);

			while(true) {
				if(_module.is_moving_done(servo)) {
					return ActionResult::SUCCESS;
				}

				if(TimePoint::now() >= timeoutDate) {
					logWarn("TIMEOUT !");
					return ActionResult::TIMEOUT;
				}

				if(_module.is_blocking(servo)) {
					logWarn("Blocked, I go back to the previous position!");
					_module.set_position(servo, old_pos);
					return ActionResult::FAILURE;
				}
				sleep(50_ms);
			}
		}
	} // namespace Interfacer
} // namespace Strategy
