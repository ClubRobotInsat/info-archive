//
// Created by terae on 3/22/18.
//

#ifndef ROOT_MAGICSTRATEGY_H
#define ROOT_MAGICSTRATEGY_H

#include <thread>

#include "Action.hpp"
#include "Element.hpp"
#include "Table.hpp"

#include "DecisionalTree.hpp"

namespace StrategyGenerator {
	/**
	 * @brief Créateur automatique de stratégie
	 *
	 * Ce générateur doit être initialisé avec la `Table` initiale
	 * Il faut aussi associer à chaque `ElementType` des foncteurs pour :
	 * - créer l'action associée avec des `Coordonnees` en paramètre
	 * - savoir si l'action est réalisable en fonction de l'état du robot
	 *
	 * La classe travaille en multi-threading :
	 * - le thread principal calcule toutes les 500ms l'action_path optimal
	 * - le thread exécutif qui fait un `allerA` puis lance petri avec le bon paramètre
	 */
	class MagicStrategy {
	private:
		int _total_points;

		std::map<StrategyGenerator::ElementType, std::function<StrategyGenerator::Action(repere::Coordonnees)>> _element_to_action;
		std::map<StrategyGenerator::ElementType, std::function<bool()>> _element_actionable;

		StopWatch _start_time;

		std::vector<std::pair<Table, Action>> _previous_actions;

		Table _initial_table;

		Action _actual_action;

		void check_element(std::map<ElementType, bool>& typeOk, Element element) const;

		void generate_tree(DecisionalTree& tree, Duration timeout);

		// MagicStrategy should be run only if it is completely set up
		bool _initialized;

	public:
		MagicStrategy() : _total_points(0), _actual_action(ActionWait(0_s)), _initialized(false) {}

		void associate_element(ElementType type, std::function<Action(repere::Coordonnees)> action, std::function<bool()> element_actionable) {
			_element_to_action[type] = std::move(action);
			_element_actionable[type] = std::move(element_actionable);
		}

		void initialize(Table initial_table) {
			_initial_table = std::move(initial_table);
			_actual_action =
			    Action(0_s, 0, Element(ElementType::NOTHING, _initial_table._robot_coords), {}, "initial_action");

			_initialized = true;
		}

		/**
		 * Contient toute la logique de la génération de stratégie
		 * Durée de vie : 90_s
		 * A intervalle régulier (tous les @param max_refresh_time), exécution de l'algo :
		 * - génération complète de l'arbre de décision (temps limité à 3/4 * max_refresh_time)
		 * - récupération de la liste des actions optimale
		 * - SI liste vide : le robot n'a plus rien à faire, EXIT
		 * - SI action_en_cours != liste.begin()
		 *    - SI petri_ancienne_action fini
		 *        - incrémentation des points marqués
		 *        - ajout de l'ancienne action dans les actions exécutées
		 *        - CONTINUE
		 *    - cancel du thread exécutif
		 *    - création du nouveau thread exécutif pour gérer la nouvelle action
		 *
		 * @code
		 * MagicStrategy strat;
		 * std::function<bool()> always_possible = []() -> bool { return true; };
		 * strat.associate_element(BEE,    action_bee,    always_possible);
		 * strat.associate_element(CUBE,   action_cube,   always_possible);
		 * strat.associate_element(SPHERE, action_sphere, always_possible);
		 * strat.associate_element(SWITCH, action_switch, always_possible);
		 *
		 * Table t = generate_table();
		 * strat.initialize(t);
		 *
		 * // Enjoy
		 * strat.run(500_ms);
		 */
		void run(Petri::PetriDynamicLib& petri, Duration max_refresh_time);

		/**
		 * Fonction `cleanup` appelée lors du kill du thread exécutif lors du changement d'action
		 * Appelée lors du déplacement du robot avec `allerA` ; jamais appelé j=lors du run d'un petri
		 */
		static void cleanup(void*) {
			// TODO: stop AllerA (arret d'urgence)
			logDebug("MagicStrategy::cleanup() called.");
		}

		/**
		 * Fonction appelée par le thread exécutif
		 * @param mutex_petri_running Empêche le thread principal de tuer le thread si l'action petri est en cours
		 * @param action Action à exécuter
		 */
		static void executioner(Petri::PetriNet& petri, std::mutex& mutex_petri_running, const Action& action, Duration timeout) {
			pthread_cleanup_push(cleanup, nullptr);

			logDebug("MagicStrategy::execute_action() called");

			//_dep.arretUrgence();
			//_dep.allerA(action.get_start_position());
			//_dep.tournerAbsolu(action.get_start_angle());

			pthread_cleanup_pop(1);

			logDebug("launch of petri for the action '", action, "'");

			// lock mutex at creation, unlock it at destruction
			std::lock_guard<std::mutex> guard(mutex_petri_running);
			// sleep(5_s);
			action.execute_petri(petri, timeout);
			logDebug("end of execute_action");
		}

		inline int get_total_points() const {
			return _total_points;
		}
	};
}

#endif // ROOT_MAGICSTRATEGY_H
