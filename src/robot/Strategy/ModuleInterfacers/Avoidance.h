//
// Created by terae on 23/01/19.
//

#ifndef ROOT_AVOIDANCE_H
#define ROOT_AVOIDANCE_H

#include "../Environment/environment.h"
#include "Interfacer.hpp"

namespace Strategy {
	namespace Interfacer {
		class Avoidance final : public AbstractInterfacer {
		public:
			using interfaced_type = void;

			Avoidance(std::shared_ptr<PhysicalRobot::Robot> robot, Environment& env, Vector2m turret_shift = Vector2m(0_m, 0_m));

			~Avoidance();

			void set_turret_shift(Vector2m shift);

			std::vector<repere::Position> get_adversary_positions() const;

			/**
			 * @return true si l'adversaire est devant le robot
			 * @param threshold Ajustement de la distance à partir de laquelle l'adversaire est considéré comme dangereux
			 */
			bool adversary_detected(Distance threshold) const;

			/**
			 * Fonction permettant de modifier l'angle de détection de l'adversaire
			 * Attention: il faut donner la taille de la demi-zone
			 *
			 * ex: pour avoir un cone de +/- 0.25_PI, donner 0.25_PI
			 * default : +/- 0.25_PI
			 */
			void set_adversary_detection_angle(Angle angle);

			Angle get_adversary_detection_angle() const;

		private:
			std::atomic_bool _is_running;

			std::shared_ptr<PhysicalRobot::Robot> _robot;
			Environment& _env;

			/**
			 * x : décalage vers l'avant du robot
			 * y : décalage vers le côté droit du robot
			 */
			Vector2m _turret_shift;
			std::atomic<Angle> _angle_detection_adversary;

			void thread_lidar();
			std::thread _find_robots;
			mutable std::mutex _mutex_adversary;
			std::vector<repere::Position> _adversary_positions;
		};
	} // namespace Interfacer
} // namespace Strategy


#endif // ROOT_AVOIDANCE_H
