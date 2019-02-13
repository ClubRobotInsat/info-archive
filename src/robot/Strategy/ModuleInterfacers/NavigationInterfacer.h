//
// Created by terae on 05/02/19.
//

#ifndef ROOT_NAVIGATIONINTERFACER_H
#define ROOT_NAVIGATIONINTERFACER_H

#include "../../Modules/Navigation.h"
#include "AvoidanceInterfacer.h"

#include <list>
#include <stack>

namespace Strategy {
	namespace Interfacer {

		class NavigationInterfacer : public AbstractInterfacer {
		public:
			using interfaced_type = PhysicalRobot::Navigation;

			using SensRotation = PhysicalRobot::SensRotation;
			using SensAdvance = PhysicalRobot::SensAdvance;

			NavigationInterfacer(std::shared_ptr<PhysicalRobot::Robot>, Environment&, AvoidanceInterfacer&);

			repere::Coordinates get_origin_coordinates() const;

			// Linear speed
			Speed get_linear_speed() const;

			void push_linear_speed(Speed);

			Speed pop_linear_speed();

			// Angular speed
			AngularSpeed get_angular_speed() const;

			void push_angular_speed(AngularSpeed);

			AngularSpeed pop_angular_speed();

			// Linear accuracy
			Distance get_linear_accuracy() const;

			void push_linear_accuracy(Distance);

			Distance pop_linear_accuracy();

			// Angular accuracy
			Angle get_angular_accuracy() const;

			void push_angular_accuracy(Angle);

			Angle pop_angular_accuracy();

			/**
			 * Actions
			 */

			ActionResult move_to(repere::Position destination, Duration timeout);

			ActionResult move_to(repere::Position destination, SensAdvance = SensAdvance::Forward, Duration timeout = 25_s);

			ActionResult forward(Distance, SensAdvance, Duration timeout = 10_s);

			/// Tourne selon le sens le plus rapide qui permets d'orienter le robot à l'angle absolu souhaité
			ActionResult turn_absolute(repere::Orientation, Duration timeout = 10_s);

			ActionResult turn_absolute(repere::Orientation, SensRotation, Duration timeout = 10_s);

			ActionResult turn_relative(Angle, Duration timeout = 10_s);

			ActionResult stop();

			ActionResult emergency_stop();

			/// Helpers
			SensRotation optimal_rotation_sens(repere::Orientation from, repere::Orientation to);

		private:
			interfaced_type& _module;
			AvoidanceInterfacer& _avoidance;

			template <typename T>
			T peek_stack(const std::stack<T>& stack) const {
				std::lock_guard<std::mutex> lk(_mutex_states);
				return stack.top();
			}

			template <typename T>
			void push_stack(std::stack<T>& stack, T value) {
				std::lock_guard<std::mutex> lk(_mutex_states);
				if(stack.size() > 1000) {
					const std::string msg_error = "Too much values on the stack!";
					logError(msg_error);
					throw std::runtime_error(msg_error);
				}
				stack.push(value);
			}

			template <typename T>
			T pop_stack(std::stack<T>& stack) {
				std::lock_guard<std::mutex> lk(_mutex_states);
				if(stack.size() == 1) {
					const std::string msg_error = "Only one value on the stack, can't pop!";
					logError(msg_error);
					throw std::runtime_error(msg_error);
				}
				T result = stack.top();
				stack.pop();
				return result;
			}

			std::unique_lock<std::recursive_timed_mutex> try_get_lock_for_action();
			std::unique_lock<std::recursive_timed_mutex>
			    get_lock_for_action(const TimePoint& try_until = TimePoint::distantFuture());
			std::unique_lock<std::recursive_timed_mutex> get_lock_for_action(std::defer_lock_t);

			std::function<bool()> test_unit_navigation_ended();
			std::function<bool()> test_unit_navigation_accuracy_reached();

			ActionResult wait_end_trajectory(const std::function<bool()>& condition_end_trajectory,
			                                 TimePoint date_timeout,
			                                 bool stop = true,
			                                 bool check_adversary = true,
			                                 SensAdvance = SensAdvance::Forward);

			ActionResult internal_forward(Distance, SensAdvance, Duration timeout);

			// TODO: reverse-engineering the `AllerA` code to reproduce it
			struct TrajectoryPart {
				TrajectoryPart(const repere::Coordinates& coord, SensAdvance sens) : coordinates(coord), sens(sens) {}

				repere::Coordinates coordinates;
				SensAdvance sens;
			};

			using Trajectory = std::list<TrajectoryPart>;


			mutable std::mutex _mutex_states;
			std::stack<Speed> _linear_speed;
			std::stack<AngularSpeed> _angular_speed;
			std::stack<Distance> _linear_accuracy;
			std::stack<Angle> _angular_accuracy;

			std::recursive_timed_mutex _mutex_action;

			Environment& _env;

			repere::Coordinates _origin_coordinates;
		};

	} // namespace Interfacer
} // namespace Strategy


#endif // ROOT_NAVIGATIONINTERFACER_H