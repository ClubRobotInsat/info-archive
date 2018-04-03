//
// Created by terae on 3/29/18.
//

#include "MagicStrategy.h"
#include <ConstantesCommunes.h>

void StrategyGenerator::MagicStrategy::run(const Table& initial_table, Duration max_refresh_time) {
	StopWatch start_time;
	std::mutex mutex_action_running;

	while(start_time.getElapsedTime() <= 90_s /*Constantes::MATCH_DURATION*/) {
		StopWatch calculation_time;
		calculation_time.reset();

		// ----------------------------------------------------------------- //
		DecisionalTree action_tree(initial_table);

		action_tree.add_edge(action_tree.get_root(),
		                     _previous_actions.back().first,
		                     Action(start_time.getElapsedTime(),
		                            _total_points,
		                            _previous_actions.back().second.get_start_position(),
		                            _previous_actions.back().second.get_start_angle(),
		                            {} /*,
                                                 nullptr*/));

		generate_tree(action_tree, 0.75 * max_refresh_time - calculation_time.getElapsedTime());
		// std::cout << "max_points = " << action_tree.max_points << "; best_node = " << action_tree.best_node <<
		// std::endl;
		std::list<Action> action_path{action_tree.generate_action_path()};
		std::cout << "Action path : " << action_path.size() << " different actions with " << action_tree.max_points << " points: ";
		for(Action a : action_path) {
			std::cout << '[' << a.get_start_position().x.toCm() << ',' << a.get_start_position().y.toCm() << "] ";
		}
		std::cout << "\n" << std::endl;

		// the robot should change its strategy (new action path found)
		if(action_path.front() != _actual_action || mutex_action_running.try_lock()) {
			mutex_action_running.unlock();
			pthread_cancel(_id_thread);

			_actual_action = action_path.front();
			_execute_action = std::thread(execute_action /*, mutex_action_running, _actual_action, _total_points*/);
			_id_thread = _execute_action.native_handle();
			_execute_action.join();
		}
	}
}

void StrategyGenerator::MagicStrategy::generate_tree(StrategyGenerator::DecisionalTree& tree, Duration timeout) {
	StopWatch calculation_time;

	DecisionalTree::node* actual_node;
	while((actual_node = tree.pop_frontier_node()) != nullptr && calculation_time.getElapsedTime() < timeout) {
		Duration actual_time{tree.calculate_cost(actual_node).second};
		if(actual_time < 90_s /*Constantes::MATCH_DURATION*/) {
			for(std::shared_ptr<Element> e : actual_node->data) {
				if(_element_actionable[e->get_type()]()) {
					Table next_table{tree.get_table(actual_node)};
					next_table.erase(e);
					Action associated_action(_element_to_action[e->get_type()](e->get_position()));
					for(Element new_e : associated_action.get_next_elements()) {
						next_table.emplace(std::make_shared<Element>(new_e));
					}
					tree.add_edge(actual_node, next_table, associated_action);
				}
			}
		}
	}
}
