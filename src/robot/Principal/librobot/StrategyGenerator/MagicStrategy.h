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
	class MagicStrategy {
	private:
		int _total_points;

		std::map<StrategyGenerator::ElementType, std::function<StrategyGenerator::Action(repere::Coordonnees)>> _element_to_action;
		std::map<StrategyGenerator::ElementType, std::function<bool()>> _element_actionable;

		StopWatch _start_time;

		std::vector<std::pair<Table, Action>> _previous_actions;

		Table _initial_table;

		Action _actual_action;

		std::unique_ptr<Petri::PetriDynamicLib> _petri;

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

		void run(Duration max_refresh_time);

		static void cleanup(void*) {
			// TODO: stop AllerA (arret d'urgence)
			logDebug("MagicStrategy::cleanup() called.");
		}

		// signature of called function for the executive thread
		static void executioner(std::mutex& mutex_petri_running, const Action& action) {
			pthread_cleanup_push(cleanup, nullptr);

			logDebug("MagicStrategy::execute_action() called");

			//_dep.arretUrgence();
			//_dep.allerA(action.get_start_position());
			//_dep.tournerAbsolu(action.get_start_angle());

			pthread_cleanup_pop(1);

			logDebug("launch of petri for the action '", action, "'");

			// lock mutex at creation, unlock it at destruction
			std::lock_guard<std::mutex> guard(mutex_petri_running);
			sleep(5_s);
			// action.execute_petri(*_petri, MATCH_DURATION - _start_time.getElapsedTime());
			logDebug("end of execute_action");
		}

		inline int get_total_points() const {
			return _total_points;
		}
	};
}

#endif // ROOT_MAGICSTRATEGY_H
