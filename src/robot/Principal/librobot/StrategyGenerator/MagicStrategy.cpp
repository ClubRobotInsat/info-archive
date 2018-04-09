//
// Created by terae on 3/29/18.
//

#include "MagicStrategy.h"
#include <ConstantesCommunes.h>
#include <cassert>

void StrategyGenerator::MagicStrategy::run(Duration max_refresh_time) {
	using namespace std::chrono_literals;

	assert(_initialized);

	// TODO: test if all elements in the initial table and possible elements have good functions to call

	_start_time.reset();
	std::mutex mutex_petri_running;

	std::atomic<bool> petri_finished(false);

	_previous_actions.emplace_back(_initial_table, _actual_action);
	std::thread execute_action = std::thread([] { std::this_thread::sleep_for(90s); });
	execute_action.detach();

	pthread_t id_thread = 0;

	logDebug("Début de MagicStrategy::run(", max_refresh_time, ").");

	while(_start_time.getElapsedTime() < MATCH_DURATION * 10000) {
		StopWatch calculation_time;
		calculation_time.reset();

		// ----------------------------------------------------------------- //
		DecisionalTree action_tree(_initial_table);

		action_tree.add_edge(action_tree.get_root(),
		                     _previous_actions.back().first,
		                     Action(_start_time.getElapsedTime(),
		                            _total_points,
		                            _previous_actions.back().second.get_coordonnees(),
		                            {},
		                            ActionType::NOTHING,
		                            "transition_action"));

		generate_tree(action_tree, 0.75 * max_refresh_time - calculation_time.getElapsedTime());

		std::list<Action> action_path{action_tree.generate_action_path()};
		// logDebug0("Tree generated successfully with ", action_tree.size(), " nodes inside. Associated path has ",
		// action_path.size(), " actions to be run and it will give ", action_tree.max_points, " points.");

		if(action_path.empty()) {
			logDebug1("MagicStrategy determines that no action can be launched.");
			break;
		}

		// the robot should change its strategy (new action path found are previous action finished)
		if((action_path.front() != _actual_action && mutex_petri_running.try_lock()) || petri_finished) {
			mutex_petri_running.unlock();

			logDebug4("===== action path: =====");
			for(Action a : action_path) {
				logDebug4(a);
			}

			if(petri_finished) {
				// petri action has finished: update the previous actions and increment points; then calculate again the
				// new best path
				_previous_actions.emplace_back(action_tree.get_table_after_action(_actual_action), _actual_action);
				_total_points += _actual_action.get_nr_points();
				petri_finished = false;
				logDebug1("The action '", _actual_action, "' has finished with success.");
				logDebug1("_previous_actions.size() = ",
				          _previous_actions.size(),
				          "; _total_points = ",
				          get_total_points(),
				          "; action_path.size() = ",
				          action_path.size());
				continue;
			}

			logDebug("new action found: abort '", _actual_action, "' and execution of '", action_path.front(), "'");

			// kill the executive thread which will stop the robot before dying
			pthread_cancel(id_thread);

			_actual_action = action_path.front();
			// execution of the thread: first the action is running and then we stipulate that petri had time to finish
			// its life
			execute_action = std::thread(
			    [&petri_finished](std::mutex& mutex_petri_running, const Action& action) {
				    executioner(mutex_petri_running, action);
				    petri_finished = true;
				},
			    std::ref(mutex_petri_running),
			    std::cref(_actual_action));
			id_thread = execute_action.native_handle();
			execute_action.detach(); // FIXME: detach() or join() in this case?
		}
		// TODO: sleep(500ms - execution_time) to always guarantee an execution of 500ms?
	}
	sleep(MATCH_DURATION - _start_time.getElapsedTime());
	// TODO: stop everything (deplacement + petri)
}

void StrategyGenerator::MagicStrategy::generate_tree(StrategyGenerator::DecisionalTree& tree, Duration timeout) {
	StopWatch calculation_time;

	DecisionalTree::node* actual_node;
	while((actual_node = tree.pop_frontier_node()) != nullptr && calculation_time.getElapsedTime() < timeout) {
		Duration actual_time{tree.calculate_cost(actual_node).second};
		if(actual_time < Constantes::MATCH_DURATION) {
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
	if(calculation_time.getElapsedTime() >= timeout) {
		logWarn("generate_tree time out");
	}
}
