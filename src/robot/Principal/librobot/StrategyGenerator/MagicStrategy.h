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
		enum ActionState {
			WAIT,

			MOVE_RUNNING,
			PETRI_RUNNING,
		};

		std::condition_variable _ia_cv;
		std::atomic<ActionState> _state;
		std::atomic_bool _last_petri_succeed;
		std::mutex _mut_petri_running;
		std::mutex _mut_action_available;


		int _total_points;

		std::map<StrategyGenerator::ElementType, std::function<StrategyGenerator::Action(repere::Coordonnees)>> _element_to_action;
		std::map<StrategyGenerator::ElementType, std::function<bool()>> _element_actionable;

		StopWatch _start_time;

		std::vector<std::pair<Table, std::shared_ptr<Action>>> _previous_actions;

		Table _initial_table;

		std::shared_ptr<Action> _actual_action;

		void check_element(std::map<ElementType, bool>& typeOk, Element element) const;

		void generate_tree(DecisionalTree& tree, Duration timeout);

		// MagicStrategy should be run only if it is completely set up
		bool _initialized;

	public:
		MagicStrategy() : _total_points(0), _actual_action(nullptr), _initialized(false) {}

		void associate_element(ElementType type, std::function<Action(repere::Coordonnees)> action, std::function<bool()> element_actionable) {
			_element_to_action[type] = std::move(action);
			_element_actionable[type] = std::move(element_actionable);
		}

		void initialize(Table initial_table) {
			_initial_table = std::move(initial_table);
			_previous_actions.emplace_back(_initial_table,
			                               std::make_shared<Action>(
			                                   Action(0_s, 0, Element(ElementType::NOTHING, _initial_table._robot_coords), {})));

			_last_petri_succeed = false;

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
		void run(Commun::Deplacement& dep, Petri::PetriDynamicLib& petri, Duration max_refresh_time);

		inline int get_total_points() const {
			return _total_points;
		}

	private:
		/**
		 * Fonction appelée par le thread exécutif
		 * @param mutex_petri_running Empêche le thread principal de donner une autre action si l'action petri est en
		 * cours
		 * @param action Action à exécuter
		 */
		void action_executor(Commun::Deplacement& dep, Petri::PetriNet& petri) {
			std::cout << "petri: " << petri.name() << std::endl;
			while(_start_time.getElapsedTime() < Constantes::MATCH_DURATION) {
				_state = WAIT;

				std::unique_lock<std::mutex> lk(_mut_action_available);
				std::cout << "executor: before waiting" << std::endl;
				_ia_cv.wait(lk, [this]() { return _actual_action != nullptr; });
				std::cout << "executor : wait passed" << std::endl;
				lk.unlock();

				if(_start_time.getElapsedTime() > Constantes::MATCH_DURATION) {
					return;
				}

				_last_petri_succeed = false;

				_state = MOVE_RUNNING;
				if(dep.allerA(_actual_action->get_coordonnees().getPos2D()) == ResultatAction::REUSSI) {
					if(dep.tournerAbsolu(_actual_action->get_coordonnees().getAngle()) == ResultatAction::REUSSI) {
						std::lock_guard<std::mutex> guard(_mut_petri_running);
						_state = PETRI_RUNNING;
						std::cout << "executor: petri '" << petri.name()
						          << "' execution ; _actual_action: " << _actual_action->get_name() << std::endl;
						_actual_action->execute_petri(petri, Constantes::MATCH_DURATION - _start_time.getElapsedTime());
						std::cout << "executor: end of petri" << std::endl;
						if(petri.variables().isReturnValues()) {
							std::cout << "executor: Petri return values: " << std::endl;
							for(auto& var : petri.variables()) {
								std::cout << "\t" << var.name() << ": " << var.value() << std::endl;
							}
						}
						_last_petri_succeed = true;
					}
				}

				_actual_action = nullptr;
			}
		}
	};
}

#endif // ROOT_MAGICSTRATEGY_H
