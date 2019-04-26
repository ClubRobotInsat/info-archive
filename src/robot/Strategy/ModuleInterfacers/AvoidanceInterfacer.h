//
// Created by terae on 23/01/19.
//

#ifndef ROOT_AVOIDANCE_H
#define ROOT_AVOIDANCE_H

#include "../Environment/environment.h"
#include "Interfacer.hpp"

namespace Strategy {
	namespace Interfacer {

		class AvoidanceInterfacer final : public AbstractInterfacer {
		public:
			using interfaced_type = void;

			AvoidanceInterfacer(std::shared_ptr<PhysicalRobot::Robot> robot,
			                    Environment& env,
			                    Vector2m turret_shift = Vector2m(0_m, 0_m));

			~AvoidanceInterfacer() override;

			void set_turret_shift(Vector2m shift);

			std::vector<repere::Position> get_adversary_positions() const;

			/** Ajoute une position d'adversaire fictive. Utilisé entre autres dans les tests unitaires pour contrôller
			 * que le traitement des positions de l'adversaire est correct. */
			void add_fake_adversary_position(const repere::Position& position);

			/**
			 * @return true si l'adversaire est devant le robot
			 * @param threshold Ajustement de la distance à partir de laquelle l'adversaire est considéré comme dangereux
			 */
			bool adversary_detected(Distance threshold = GLOBAL_CONSTANTS().get_threshold_adversary_detection()) const;

			/**
			 * @return true si l'adversaire est devant le robot en fonction de son @param sens de déplacement
			 * @param threshold Ajustement de la distance à partir de laquelle l'adversaire est considéré comme dangereux
			 */
			bool adversary_detected(PhysicalRobot::SensAdvance) const;
			bool adversary_detected(Distance threshold, PhysicalRobot::SensAdvance) const;

			/** Ajoute des DynamicShape correspondant aux adversaires detectés sur la table.
			 * @returns l'id des DynamicShape ainsi ajoutées. */
			std::vector<int> update_environment(Environment& env) const;

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

			void thread_lidar_main();
			std::thread _thread_lidar;

			/// Mutex gérant l'accès à la liste des positions de l'adversaire
			mutable std::mutex _mutex_adversary;
			std::vector<repere::Position> _adversary_positions;
			std::vector<repere::Position> _fake_positions;

			repere::Position get_robot_position() const;

			repere::Orientation get_robot_orientation() const;
		};
	} // namespace Interfacer
} // namespace Strategy


#endif // ROOT_AVOIDANCE_H
