/**
 * Stratégie d'un match à durée limitée
 * Cette classe regroupe les informations propres à un match :
 * - couleur de l'équipe
 * - nombre de points
 * - référence du côté de la table
 * - détection de la tirette
 * - prise en compte de l'adversaire
 * - gestion du temps de jeu
 * - environnement
 * - funny action
 */

#ifndef ROOT_STRATEGY_H
#define ROOT_STRATEGY_H

#include "../Robot.h"
#include "Commun.h"
#include "Environment/environment.h"
#include <Constants.h>

#include <memory>
#include <thread>

namespace Strategy {

	class AbstractStrategy {
	public:
		void start(Duration match = GLOBAL_CONSTANTS().get_match_duration());

		Constants::RobotColor get_color() const {
			return _color;
		}

		const repere::Repere& get_reference() const {
			return GLOBAL_CONSTANTS().get_reference(_color);
		}

		Duration get_left_time() const;
		Duration get_time() const;

		void reset_timer();

		Environment& get_environment() {
			return *_env;
		}

	protected:
		AbstractStrategy(std::unique_ptr<PhysicalRobot::Robot>, Constants::RobotColor);

		virtual ~AbstractStrategy() = default;

		/// Contient la logique de la stratégie elle-même
		virtual void execute() = 0;
		// Exécute la funny action à la fin du match
		virtual void funny_action() {}

		/// Thread dans lequel s'exécute la stratégie
		std::thread _execution;

		std::unique_ptr<PhysicalRobot::Robot> _robot;
		std::unique_ptr<Environment> _env;

	private:
		void create_environment();

		Constants::RobotColor _color;

		Duration _total_duration_match = 0_s;
		StopWatch _chrono_match;

		// Appel de la fonction virtuelle 'execute' ; ça permets d'arrêter le thread de la stratégie à la fin du match
		void exec();
	};
} // namespace Strategy

#endif // ROOT_STRATEGY_H
