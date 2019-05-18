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

#include "Commun.h"
#include "Environment/environment.h"
#include "ModuleInterfacers/RobotManager.h"
#include <Constants.h>

#include <memory>
#include <thread>

namespace Strategy {

	class AbstractStrategy {
	public:
		void start(Duration match_duration = GLOBAL_CONSTANTS().get_match_duration());

		// Arrête tous les actionneurs
		virtual void stop();

		Constants::RobotColor get_color() const;

		const repere::Repere& get_reference() const;

		Duration get_left_time() const;
		Duration get_time() const;

		void reset_timer();

		Environment& get_environment() const;

		// Automatically tries to create all possible interfacers
		// Call `add_manager`
		std::shared_ptr<Interfacer::RobotManager> add_robot(std::shared_ptr<PhysicalRobot::Robot>);
		// Automatically tries to create all high-level PetriLab functions
		std::shared_ptr<Interfacer::RobotManager> add_manager(std::shared_ptr<Interfacer::RobotManager>);

		std::shared_ptr<Interfacer::RobotManager> get_robot(const std::string& name);

		std::vector<std::string> get_robot_names() const;

		void wait_for_tirette() const;

		int get_points() const;

		int add_points(int n);

		int set_points(int n);

		bool debug_mode = true;

	protected:
		explicit AbstractStrategy(Constants::RobotColor);

		virtual ~AbstractStrategy() = default;

		/// Contient la logique de la stratégie elle-même
		virtual void execute() = 0;
		// Exécute la funny action à la fin du match
		virtual void funny_action() {}

		/// Thread dans lequel s'exécute la stratégie
		std::thread _execution;

		std::map<std::string, std::shared_ptr<Interfacer::RobotManager>> _interfacers;
		std::unique_ptr<Environment> _env;

	private:
		void create_environment();

		Constants::RobotColor _color;

		Duration _total_duration_match = 0_s;
		StopWatch _chrono_match;

		std::atomic_int _nb_points;

		// Appel de la fonction virtuelle 'execute' ; ça permets d'arrêter le thread de la stratégie à la fin du match
		void exec();
	};
} // namespace Strategy

#endif // ROOT_STRATEGY_H
