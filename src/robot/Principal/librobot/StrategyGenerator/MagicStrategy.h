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

		std::map<StrategyGenerator::ElementType, std::function<StrategyGenerator::Action(Vector2m)>> _element_to_action;
		std::map<StrategyGenerator::ElementType, std::function<bool()>> _element_actionable;

		std::vector<std::pair<Table, Action>> _previous_actions;

		// executive thread; TODO : bind with a IAPrincipal function (access to _dep and _meca)
		std::thread _execute_action;
		pthread_t _id_thread;
		Action _actual_action;

		void generate_tree(DecisionalTree& tree, Duration timeout);

	public:
		explicit MagicStrategy(std::map<StrategyGenerator::ElementType, std::function<StrategyGenerator::Action(Vector2m)>> element_to_action,
		                       std::map<StrategyGenerator::ElementType, std::function<bool()>> element_actionable)
		        : _total_points(0)
		        , _element_to_action(std::move(element_to_action))
		        , _element_actionable(std::move(element_actionable)) {}

		void run(const Table& initial_table, Duration max_refresh_time);

		// signature of called function for the executive thread
		static void execute_action(/*std::mutex mutex_action_running, Action action, int &points*/) {
			/*mutex_action_running.lock();

			//_dep.allerA(action.get_start_position());
			//_dep.tournerAbsolu(action.get_start_angle());
			action.execute();
			points += action.get_nr_points();

			mutex_action_running.unlock();*/
		}

		inline int get_total_points() const {
			return _total_points;
		}
	};
}

#endif // ROOT_MAGICSTRATEGY_H
