//
// Created by terae on 05/02/19.
//

#include "NavigationInterfacer.h"

namespace Strategy {
	namespace Interfacer {

		// Constructor
		NavigationInterfacer::NavigationInterfacer(std::shared_ptr<PhysicalRobot::Robot> robot, Environment& env, AvoidanceInterfacer& avoidance)
		        : _module(robot->get_module<interfaced_type>()), _avoidance(avoidance), _env(env) {
			push_linear_speed(GLOBAL_CONSTANTS()[Constants::name(robot->name)].get_linear_speed());
			push_angular_speed(GLOBAL_CONSTANTS()[Constants::name(robot->name)].get_angular_speed());
			push_linear_accuracy(GLOBAL_CONSTANTS()[Constants::name(robot->name)].get_linear_accuracy());
			push_angular_accuracy(GLOBAL_CONSTANTS()[Constants::name(robot->name)].get_angular_accuracy());
		}

		// States
		repere::Coordinates NavigationInterfacer::get_origin_coordinates() const {
			return _origin_coordinates;
		}

		Speed NavigationInterfacer::get_linear_speed() const {
			return peek_stack(_linear_speed);
		}

		void NavigationInterfacer::push_linear_speed(Speed speed) {
			push_stack(_linear_speed, speed);
			_module.update_linear_speed(speed);
		}

		Speed NavigationInterfacer::pop_linear_speed() {
			auto speed = pop_stack(_linear_speed);
			_module.update_linear_speed(speed);
			return speed;
		}

		AngularSpeed NavigationInterfacer::get_angular_speed() const {
			return peek_stack(_angular_speed);
		}

		void NavigationInterfacer::push_angular_speed(AngularSpeed speed) {
			push_stack(_angular_speed, speed);
			_module.update_angular_speed(speed);
		}

		AngularSpeed NavigationInterfacer::pop_angular_speed() {
			auto speed = pop_stack(_angular_speed);
			_module.update_angular_speed(speed);
			return speed;
		}

		Distance NavigationInterfacer::get_linear_accuracy() const {
			return peek_stack(_linear_accuracy);
		}

		void NavigationInterfacer::push_linear_accuracy(Distance accuracy) {
			push_stack(_linear_accuracy, accuracy);
			_module.update_linear_accuracy(accuracy);
		}

		Distance NavigationInterfacer::pop_linear_accuracy() {
			auto accuracy = pop_stack(_linear_accuracy);
			_module.update_linear_accuracy(accuracy);
			return accuracy;
		}

		Angle NavigationInterfacer::get_angular_accuracy() const {
			return peek_stack(_angular_accuracy);
		}

		void NavigationInterfacer::push_angular_accuracy(Angle accuracy) {
			push_stack(_angular_accuracy, accuracy);
			_module.update_angular_accuracy(accuracy);
		}

		Angle NavigationInterfacer::pop_angular_accuracy() {
			auto accuracy = pop_stack(_angular_accuracy);
			_module.update_angular_accuracy(accuracy);
			return accuracy;
		}

		// States
		ActionResult NavigationInterfacer::move_to(repere::Position destination, Duration timeout) {
			return move_to(destination, SensAdvance::Forward, timeout);
		}

		ActionResult NavigationInterfacer::move_to(repere::Position destination, SensAdvance sens, Duration timeout) {
			bool shouldIStayOrShouldIGo =
#ifdef HOMOLOGATION
			    false; // le robot ne tente pas les contournements
#else
			    true; // le robot se chauffe et tente les contournements
#endif
			// TODO
			return ActionResult::TIMEOUT;
		}

		ActionResult NavigationInterfacer::internal_forward(Distance distance, SensAdvance sens, Duration timeout) {
			auto threshold = std::min(distance + GLOBAL_CONSTANTS()["primary"].get_radius_rotation() / 2, 30_cm);
			if(_avoidance.adversary_detected(threshold, sens)) {
				return ActionResult::BLOCKED_BY_ADV;
			}

			TimePoint date_timeout = TimePoint::now() + timeout;
			ActionResult result = ActionResult::FAILURE;
			auto lock = get_lock_for_action(date_timeout);
			if(lock.owns_lock()) {
				_module.forward(distance, sens);
				result = wait_end_trajectory(test_unit_navigation_ended(), date_timeout, true, true, sens);
			} else {
				logDebug3("Module not available");
			}

			// On s'arrête si l'adversaire nous bloque
			if(result == ActionResult::BLOCKED_BY_ADV) {
				logWarn("Adversary detected, STOP!!!");
				this->emergency_stop();
				sleep(100_ms);
			}

			return result;
		}

		ActionResult NavigationInterfacer::forward(Distance distance, SensAdvance sens, Duration timeout) {
			// Attente que la position soit actualisée à coup sûr
			sleep(GLOBAL_CONSTANTS().get_frame_period());

			auto begin = TimePoint::now();

			auto initial_coords = _module.get_coordinates();
			auto ref = _module.get_reference();
			auto initial_pos = initial_coords.getPos2D(ref);
			auto initial_angle = initial_coords.getAngle(ref);

			int reverser = (sens == SensAdvance::Forward ? 1 : -1);
			auto final_coords = initial_pos + reverser * distance * Vector2d{cos(initial_angle), sin(initial_angle)};

			ActionResult result;
			do {
				sleep(GLOBAL_CONSTANTS().get_frame_period());
				auto current_pos = _module.get_coordinates().getPos2D(ref);
				distance = (final_coords - current_pos).norm();
				logDebug("Current pos: ", current_pos, " | dest: ", final_coords, " | left distance: ", distance);
				result = internal_forward(distance, sens, timeout - (TimePoint::now() - begin));

				if(result != ActionResult::SUCCESS) {
					logWarn("Failed to forward to ", final_coords, ", sens: ", sens, ", result: ", result);
				}

				if(result == ActionResult::BLOCKED) {
					break;
				}
			} while(result == ActionResult::BLOCKED_BY_ADV && (TimePoint::now() - begin) <= timeout);

			if(result != ActionResult::SUCCESS) {
				logWarn("Failed to forward to ", final_coords, ", sens: ", sens, ", result: ", result);
			}

			logDebug("END OF NavigationInterfacer::forward(); STATUS = ", result);
			return result;
		}

		ActionResult NavigationInterfacer::turn_absolute(repere::Orientation angle, Duration timeout) {
			SensRotation sens = optimal_rotation_sens(_module.get_orientation(), angle);

			return turn_absolute(angle, sens, timeout);
		}

		ActionResult NavigationInterfacer::turn_absolute(repere::Orientation angle,
		                                                 Strategy::Interfacer::NavigationInterfacer::SensRotation sens,
		                                                 Duration timeout) {
			TimePoint date_timeout = TimePoint::now() + timeout;
			auto lock = get_lock_for_action(date_timeout);
			if(lock.owns_lock()) {
				_module.turn_absolute(angle.getAngle(), sens);
				return wait_end_trajectory(test_unit_navigation_ended(), date_timeout, true, false);
			}
			logDebug3("Module not available");
			return ActionResult::FAILURE;
		}

		ActionResult NavigationInterfacer::turn_relative(Angle angle, Duration timeout) {
			TimePoint date_timeout = TimePoint::now() + timeout;
			auto lock = get_lock_for_action(date_timeout);
			if(lock.owns_lock()) {
				// FIXME: elecs get a relative value or an absolute one?
				_module.turn_relative(angle);
				return wait_end_trajectory(test_unit_navigation_ended(), date_timeout, true, false);
			}
			logDebug3("Module not available");
			return ActionResult::FAILURE;
		}

		ActionResult NavigationInterfacer::stop() {
			_module.stop();
			sleep(100_ms);
			return ActionResult::SUCCESS;
		}

		ActionResult NavigationInterfacer::emergency_stop() {
			// TODO
			return ActionResult::TIMEOUT;
		}

		NavigationInterfacer::SensRotation
		    NavigationInterfacer::optimal_rotation_sens(repere::Orientation from, repere::Orientation to) {
			return (to.getAngle() - from.getAngle()).toMinusPiPi() >= 0_rad ? SensRotation::Trigo : SensRotation::Clockwise;
		}

		std::unique_lock<std::recursive_timed_mutex> NavigationInterfacer::try_get_lock_for_action() {
			auto lock = std::unique_lock<std::recursive_timed_mutex>(_mutex_action, std::defer_lock);
			lock.try_lock();
			return lock;
		}

		std::unique_lock<std::recursive_timed_mutex> NavigationInterfacer::get_lock_for_action(const TimePoint& try_until) {
			return std::unique_lock<std::recursive_timed_mutex>(_mutex_action, try_until.value());
		}

		std::unique_lock<std::recursive_timed_mutex> NavigationInterfacer::get_lock_for_action(std::defer_lock_t) {
			return std::unique_lock<std::recursive_timed_mutex>(_mutex_action, std::defer_lock);
		}

		std::function<bool()> NavigationInterfacer::test_unit_navigation_ended() {
			return [this]() { return _module.is_moving_done(); };
		}

		std::function<bool()> NavigationInterfacer::test_unit_navigation_accuracy_reached() {
			return [this]() { return _module.is_precision_reached(); };
		};

		ActionResult NavigationInterfacer::wait_end_trajectory(const std::function<bool()>& condition_end_trajectory,
		                                                       TimePoint date_timeout,
		                                                       bool stop,
		                                                       bool check_adversary,
		                                                       SensAdvance sens) {
			for(;;) {
				// Timeout
				if(TimePoint::now() >= date_timeout) {
					return ActionResult::TIMEOUT;
				}

				// Adversary
				if(check_adversary && _avoidance.adversary_detected(sens)) {
					return ActionResult::BLOCKED_BY_ADV;
				}

				auto lock = try_get_lock_for_action();
				// Module unavailable: trajectory not valid
				if(!lock.owns_lock()) {
					return ActionResult::FAILURE;
				}

				// Blocking
				if(_module.is_physically_blocked()) {
					return ActionResult::BLOCKED;
				}

				// Trajectory ended
				if(condition_end_trajectory()) {
					if(stop) {
						this->stop();
						sleep(100_ms);
					}
					return ActionResult::SUCCESS;
				}

				sleep(10_ms);
			}
		}
	} // namespace Interfacer
} // namespace Strategy
