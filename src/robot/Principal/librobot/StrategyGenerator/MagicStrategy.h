//
// Created by terae on 3/22/18.
//

#ifndef ROOT_MAGICSTRATEGY_H
#define ROOT_MAGICSTRATEGY_H

#include "Action.hpp"
#include "Element.hpp"
#include "Table.hpp"
#include "graph.hpp"

#include "DecisionalTree.hpp"

namespace StrategyGenerator {
	class MagicStrategy {
	private:
		int _total_points;
		using decisional_graph = graph_directed<int, Table, Action>;

		std::map<StrategyGenerator::ElementType, std::function<StrategyGenerator::Action(Vector2m)>> _element_to_action;
		std::map<StrategyGenerator::ElementType, std::function<bool()>> _element_actionable;

		std::vector<std::pair<Table, Action>> _previous_actions;

		int next_id;
		// TODO: remember the best path

		void generate_graph(decisional_graph& graph, Duration timeout);
		void generate_tree(DecisionalTree& tree, Duration timeout);

	public:
		explicit MagicStrategy(std::map<StrategyGenerator::ElementType, std::function<StrategyGenerator::Action(Vector2m)>> element_to_action,
		                       std::map<StrategyGenerator::ElementType, std::function<bool()>> element_actionable)
		        : _total_points(0)
		        , _element_to_action(std::move(element_to_action))
		        , _element_actionable(std::move(element_actionable)) {}

		void run(const Table& initial_table, Duration max_refresh_time);

		// void calculate_best_path(Duration timeout);

		// void execute_best_path();

		/*        void move_to(const Action &action) {
		            /// on another thread:
		            // _dep.AllerA(action.start_position);
		            // _dep.TournerAbsolu(action.start_angle);
		        }*/
	};
}

#endif // ROOT_MAGICSTRATEGY_H
