//
// Created by terae on 3/29/18.
//

#include "MagicStrategy.h"
#include <ConstantesCommunes.h>

void StrategyGenerator::MagicStrategy::run(const Table& initial_table, Duration max_refresh_time) {
	StopWatch start_time;
	start_time.reset();

	_previous_actions.emplace_back(std::make_pair(initial_table, ActionWait(0_s)));

	while(1 /*start_time.getElapsedTime() <= Constantes::MATCH_DURATION*/) {
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


		// ----------------------------------------------------------------- //

		/*// graph which will be expanded in this function during `max_refresh_time` seconds
		// it is initialized by `G[0] --------> G[1] -> `, with both initial and actual table
		//                           (time, pts)
		decisional_graph action_network; // TODO: Action: max() and infinity()
		action_network[0] = initial_table;
		action_network[1] = _previous_actions.back().first;
		action_network(0, 1) = Action(start_time.getElapsedTime(),
		                              _total_points,
		                              _previous_actions.back().second.get_start_position(),
		                              _previous_actions.back().second.get_start_angle(),
		                              {},
		                              nullptr);

		next_id = 2;

		Duration D = 0.75 * max_refresh_time - calculation_time.getElapsedTime();
		double d = D.toMs();
		//generate_graph(action_network, D);
		std::size_t DEBUG_nr_nodes{action_network.size()};
		std::cout << DEBUG_nr_nodes << std::endl;*/

		// while (calculation_time.getElapsedTime() < max_refresh_time) {

		// generation of all future possible states from the actual_table to `MATCH_DURATION` time reached
		// . . .

		// selection of the best one
		// if move-to-next already computed on the `executive thread`, don't touch anything
		// else update the `executive thread`

		// when an action is finished, * update * _previous_actions table: give it the just-after table, the action
		//                                      * _total_points
		//                             * launch the move-to-the-next action before re-calculating everything
		//}
	}
}

void StrategyGenerator::MagicStrategy::generate_tree(StrategyGenerator::DecisionalTree& tree, Duration timeout) {
	StopWatch calculation_time;

	DecisionalTree::node* actual_node;
	while((actual_node = tree.pop_frontier_node()) != nullptr /* && calculation_time.getElapsedTime() < timeout*/) {
		Duration actual_time{tree.calculate_cost(actual_node).second};
		if(actual_time < 90_s) {
			for(std::shared_ptr<Element> e : actual_node->data) {
				// for (auto it = actual_node->data.cbegin(); it != actual_node->data.cend(); ++it) {
				//    std::shared_ptr<Element> e{*it};
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

void StrategyGenerator::MagicStrategy::generate_graph(StrategyGenerator::MagicStrategy::decisional_graph& graph, Duration timeout) {
	/*StopWatch calculation_time;
	calculation_time.reset();

	using search_path    = decisional_graph::search_path;
	using const_iterator = decisional_graph::const_iterator;

	std::list<const_iterator> expanded;
	std::deque<search_path> frontier;

	{
	    search_path p;
	    p.push_back({graph.find(1), ActionWait(0_s)});
	    frontier.push_front(std::move(p));
	}

	while (!frontier.empty() && calculation_time.getElapsedTime() < timeout) {
	    search_path p{frontier.front()};
	    frontier.pop_front();

	    if (std::find(expanded.cbegin(), expanded.cend(), p.back().first) == expanded.cend()) {
	        const_iterator last{p.back().first};
	        expanded.push_back(last);

	        // generation of all possible next steps
	        for (std::shared_ptr<Element>e : last->second->get()) {
	        //for (auto it = last->second->get().cbegin(); it != last->second->get().cend(); ++it) {
	        //std::shared_ptr<Element> e = *it;
	            if (_element_actionable[e->get_type()]() && p.total_cost().get_execution_time() < 90_s) {
	                Table next_table{last->second->get()};
	                next_table.erase(e);
	                Action associated_action{_element_to_action[e->get_type()](e->get_position())};
	                for (Element e : associated_action.get_next_elements()) {
	                    next_table.emplace(std::make_shared<Element>(e));
	                }
	                //std::size_t hash_table{next_table.hash()};
	                graph.add_edge(last, graph.add_node(next_id++, next_table).first, associated_action);
	            }
	        }

	        auto legal_actions{graph.get_out_edges(last)};
	        for (const auto &edge : legal_actions) {
	            search_path new_path{p};
	            new_path.push_back({edge.target(), edge.cost()});
	            frontier.push_back(new_path);
	        }
	    }
	}*/

	/*struct path_comparator : public std::binary_function<search_path, search_path, bool> {
	    bool operator()(const search_path &sp1, const search_path &sp2) const {
	        return sp1.total_cost().get_nr_points() < sp2.total_cost().get_nr_points();
	    }
	};
	std::list<decisional_graph::const_iterator> expanded;
	std::priority_queue<decisional_graph::search_path, std::vector<decisional_graph::search_path>, path_comparator>
	frontier;
	{
	    search_path p;
	    p.push_back({graph.find(1), ActionWait(0_s)});
	    frontier.push(std::move(p));
	}
	bool A = !frontier.empty();
	auto B = calculation_time.getElapsedTime() < timeout;
	std::cout << calculation_time.getElapsedTime().toMs() << " " << timeout.toMs() << std::endl;

	double d = timeout.toMs();
	while (!frontier.empty() && calculation_time.getElapsedTime() < timeout) {
	    search_path p{frontier.top()};
	    frontier.pop();

	    if (std::find(expanded.cbegin(), expanded.cend(), p.back().first) == expanded.end()) {
	        decisional_graph::const_iterator last{p.back().first};
	        expanded.push_back(last);

	        // generation of all possible next steps
	        for (auto it = last->second->get().cbegin(); it != last->second->get().cend(); ++it) {
	            decltype(it) ITEND = last->second->get().cend();
	            std::shared_ptr<Element> ptr = *it;
	            if (_element_actionable[(*it)->get_type()]() && p.total_cost().get_execution_time() < 90_s) {
	                Table next_table{last->second->get()};
	                next_table.erase(it);
	                Action associated_action{_element_to_action[(*it)->get_type()]((*it)->get_position())};
	                for (Element e : associated_action.get_next_elements()) {
	                    next_table.emplace(std::make_shared<Element>(e));
	                }
	                //std::size_t hash_table{next_table.hash()};
	                graph.add_edge(last, graph.add_node(next_id++, next_table).first, associated_action);
	            }
	        }

	        auto legal_actions{graph.get_out_edges(last)};
	        for (auto edge : legal_actions) {
	            search_path new_path{p};
	            new_path.push_back({edge.target(), edge.cost()});
	            frontier.push(new_path);
	        }
	    }
	}*/
}
