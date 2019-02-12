//
// Created by terae on 22/01/19.
//

#include "ElevatorInterfacer.h"

namespace Strategy {
	namespace Interfacer {
		const Duration ElevatorInterfacer::TIMEOUT = 2_s;

		ElevatorInterfacer::ElevatorInterfacer(PhysicalRobot::Motors& module, const std::vector<Angle>& positions, uint8_t id_motor)
		        : _module(module), _positions(positions), _id_motor(id_motor) {}

		ElevatorInterfacer::ElevatorInterfacer(std::shared_ptr<PhysicalRobot::Robot> robot, const std::vector<Angle>& positions, uint8_t id_motor)
		        : ElevatorInterfacer(*robot, positions, id_motor) {}

		ElevatorInterfacer::ElevatorInterfacer(PhysicalRobot::Robot& robot, const std::vector<Angle>& positions, uint8_t id_motor)
		        : ElevatorInterfacer(robot.get_module<PhysicalRobot::Motors>(), positions, id_motor) {}

		ActionResult ElevatorInterfacer::increment() {
			return this->set_elevator_blocking(_position + 1);
		}

		ActionResult ElevatorInterfacer::decrement() {
			return this->set_elevator_blocking(_position - 1);
		}

		ActionResult ElevatorInterfacer::set_elevator_blocking(std::size_t pos) {
			std::lock_guard<std::mutex> lock(_mutex_moves);

			set_elevator(pos);
			return wait_for_elevator();
		}

		ActionResult ElevatorInterfacer::set_angle_blocking(Angle angle) {
			std::lock_guard<std::mutex> lock(_mutex_moves);
			logDebug("ElevatorInterfacer goes to the angle ", angle.toDeg());
			_module.set_position_angle(_id_motor, angle);

			return this->wait_for_elevator();
		}

		ActionResult ElevatorInterfacer::set_elevator(std::size_t pos) {
			Angle angle = _positions.at(pos) + _offset;
			logDebug("ElevatorInterfacer goes to the position ", angle.toDeg());

			_position = pos;
			_module.set_position_angle(_id_motor, angle);

			return ActionResult::SUCCESS;
		}

		ActionResult ElevatorInterfacer::wait_for_elevator() {
			StopWatch compteur;
			while(!this->verify_elevator_position() && compteur.getElapsedTime() < TIMEOUT) {
				if(this->verify_elevator_blocking()) {
					_module.stop_controlled_motor(_id_motor);
					return ActionResult::BLOCKED;
				}
				sleep(20_ms);
			}

			if(compteur.getElapsedTime() >= TIMEOUT) {
				_module.stop_controlled_motor(_id_motor);
				return ActionResult::TIMEOUT;
			}

			return verify_elevator_position() ? ActionResult::SUCCESS : ActionResult::FAILURE;
		}

		bool ElevatorInterfacer::verify_elevator_position() const {
			return _module.is_position_finished(_id_motor);
		}

		bool ElevatorInterfacer::verify_elevator_blocking() const {
			// TODO
			// return _module.is_blocking(_id_motor);
			return false;
		}

		ActionResult ElevatorInterfacer::init(Strategy::Interfacer::ElevatorInterfacer::RotatingDirection sens) {
			// TODO
			return ActionResult::SUCCESS;
			/*_module.set_position_angle(_id_motor, -10000_rad);
			auto res = wait_for_elevator();
			_module.fix_angle(0_deg);
			return res;*/
		}

		Angle ElevatorInterfacer::get_angle() const {
			while(!verify_elevator_position()) {
				sleep(1_ms);
			}
			return _positions[_position];
		}
	} // namespace Interfacer
} // namespace Strategy
