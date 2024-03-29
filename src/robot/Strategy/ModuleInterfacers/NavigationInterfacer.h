//
// Created by terae on 05/02/19.
//

#ifndef ROOT_NAVIGATIONINTERFACER_H
#define ROOT_NAVIGATIONINTERFACER_H

#include "AvoidanceInterfacer.h"

#include <list>
#include <stack>

namespace Strategy::Interfacer {

	class NavigationInterfacer : public AbstractInterfacer {
	public:
		using interfaced_type = PhysicalRobot::Navigation;

		using SensRotation = PhysicalRobot::SensRotation;
		using SensAdvance = PhysicalRobot::SensAdvance;

		NavigationInterfacer(std::shared_ptr<PhysicalRobot::Robot>, Environment&, AvoidanceInterfacer&);

		interfaced_type* operator->();

		repere::Coordinates get_origin_coordinates() const;

		void activate_asserv();

		void deactivate_asserv();

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

		Outcome move_to(repere::Coordinates destination, Duration timeout);
		Outcome move_to(repere::Coordinates destination, SensAdvance = SensAdvance::Forward, Duration timeout = 25_s);

		Outcome forward(Distance, SensAdvance, Duration timeout = 10_s);

		Outcome forward_infinity(SensAdvance, Duration timeout = 10_s);

		/// Tourne selon le sens le plus rapide qui permets d'orienter le robot à l'angle absolu souhaité
		Outcome turn_absolute(const repere::Orientation&, Duration timeout = 10_s);

		Outcome turn_absolute(const repere::Orientation&, SensRotation, Duration timeout = 10_s);

		Outcome turn_relative(Angle, Duration timeout = 10_s, const repere::Repere& repere = repere::ABSOLUTE_REFERENCE);

		Outcome stop();

		Outcome emergency_stop();

		/// Helpers
		SensRotation optimal_rotation_sens(const repere::Orientation& from, const repere::Orientation& to);

		/// Recallages
		Outcome recaling_top(SensAdvance sens, Distance y);

		Outcome recaling_bottom(SensAdvance sens, Distance y);

		Outcome recaling_right(SensAdvance sens, Distance x);

		Outcome recaling_left(SensAdvance sens, Distance x);

		EXCEPTION_CLASS(ExceptionStack);

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
				throw ExceptionStack(msg_error);
			}
			stack.push(value);
		}

		template <typename T>
		T pop_stack(std::stack<T>& stack) {
			std::lock_guard<std::mutex> lk(_mutex_states);
			if(stack.size() == 1) {
				const std::string msg_error = "Only one value on the stack, can't pop!";
				logError(msg_error);
				throw ExceptionStack(msg_error);
			}
			T result = stack.top();
			stack.pop();
			return result;
		}

		std::unique_lock<std::recursive_timed_mutex> try_get_lock_for_action();
		std::unique_lock<std::recursive_timed_mutex> get_lock_for_action(const TimePoint& try_until = TimePoint::distantFuture());
		std::unique_lock<std::recursive_timed_mutex> get_lock_for_action(std::defer_lock_t);

		/** @returns Une fonction retournant true si le déplacement courant est terminé. */
		std::function<bool()> get_check_moving_done();

		/** @returns Une fonction retournant true si la précision a été atteinte pour le déplacement courant. */
		std::function<bool()> get_check_accuracy_reached();

		/** Attend la fin de la trajectoire et retourne le résultat. Si le robot est bloqué par l'adversaire,
		 * il est arrêté en urgence. */
		Outcome wait_end_trajectory(const std::function<bool()>& condition_end_trajectory,
		                            const TimePoint& date_timeout,
		                            bool stop = true,
		                            bool check_adversary = true,
		                            SensAdvance = SensAdvance::Forward);

		Outcome internal_forward(Distance, SensAdvance, Duration timeout);

		struct TrajectoryPoint {
			TrajectoryPoint(const repere::Coordinates& coord, SensAdvance sens) : coordinates(coord), sens(sens) {}

			repere::Coordinates coordinates;
			SensAdvance sens;
		};

		using Trajectory = std::list<TrajectoryPoint>;

		Trajectory compute_trajectory(const repere::Coordinates& destination, SensAdvance sens);

		Outcome follow_trajectory(Trajectory&& trajectory, TimePoint const& timeoutDate);

		Outcome go_to_point_straight(const repere::Coordinates& destination, SensAdvance sens, TimePoint const& date_timeout);

		/** Propose une position de repli en cas de bloquage par l'adversaire. Choisit un point possible
		 * parmi un nombre fini de possibilités autour de la position présente du robot */
		Distance compute_backup_distance(SensAdvance escapeSens, Distance escapeRadius);

		Outcome recaling_helper(SensAdvance, Distance other_component, std::pair<Angle, Angle> angles, bool isX);


		mutable std::mutex _mutex_states;
		std::stack<Speed> _linear_speed;
		std::stack<AngularSpeed> _angular_speed;
		std::stack<Distance> _linear_accuracy;
		std::stack<Angle> _angular_accuracy;

		std::recursive_timed_mutex _mutex_action;

		Environment& _env;
		/// Si true, les trajectoires calculées par l'A* sont écrites dans un fichier.
		bool _write_astar = false;

		repere::Coordinates _origin_coordinates;
	};

} // namespace Strategy::Interfacer


#endif // ROOT_NAVIGATIONINTERFACER_H
