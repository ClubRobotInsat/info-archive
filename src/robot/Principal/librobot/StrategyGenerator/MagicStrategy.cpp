//
// Created by terae on 3/29/18.
//

#include "MagicStrategy.h"
#include <ConstantesCommunes.h>
#include <cassert>

void StrategyGenerator::MagicStrategy::check_element(std::map<ElementType, bool>& typeOk, Element element) const {
	if(typeOk.find(element.get_type()) != typeOk.cend()) {
		return;
	}

	assert(_element_to_action.find(element.get_type()) != _element_to_action.cend());
	assert(_element_actionable.find(element.get_type()) != _element_actionable.cend());

	typeOk[element.get_type()] = true;

	for(auto e : _element_to_action.find(element.get_type())->second(repere::Coordonnees()).get_next_elements()) {
		check_element(typeOk, e);
	}
}

void StrategyGenerator::MagicStrategy::run(Commun::Deplacement& dep, Petri::PetriDynamicLib& petri, Duration max_refresh_time) {
	using namespace std::chrono_literals;

	// Check if all conditions are respected before launching the strategy
	assert(_initialized);
	{
		std::map<ElementType, bool> typeOk;
		for(const auto& e : _initial_table) {
			check_element(typeOk, *e);
		}
	}
	logDebug("All conditions are respected, start of MagicStrategy::run(", max_refresh_time, ").");

	_start_time.reset();
	std::mutex mutex_petri_running;

	std::atomic<bool> petri_finished(false);
	bool petri_returned_value;

	_previous_actions.emplace_back(_initial_table, _actual_action);
	std::thread execute_action = std::thread([] { std::this_thread::sleep_for(90s); });
	execute_action.detach();

	pthread_t id_thread = 0;

	while(_start_time.getElapsedTime() < MATCH_DURATION * 10000) {
		StopWatch calculation_time;
		calculation_time.reset();

		// ----------------------------------------------------------------- //
		DecisionalTree action_tree(_initial_table);

		action_tree.add_edge(action_tree.get_root(),
		                     _previous_actions.back().first,
		                     Action(_start_time.getElapsedTime(),
		                            _total_points,
		                            Element(ElementType::NOTHING, _previous_actions.back().second.get_coordonnees()),
		                            {}));

		generate_tree(action_tree, 0.75 * max_refresh_time - calculation_time.getElapsedTime());

		auto p = action_tree.generate_action_path();
		std::list<Action> action_path = p.first;

		if(0) {
			std::cout << "\n\n\nTree generated: \n\n" << action_tree << "\n\n\n";
			std::cout << "action path: ";
			for(Action a : action_path) {
				std::cout << "--> " << a;
			}
			std::cout << std::endl;
		}

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
				petri_finished = false;
				if(petri_returned_value == true) {
					// petri action has finished: update the previous actions and increment points; then calculate again
					// the
					// new best path
					std::cout << "new table : " << p.second << std::endl;
					std::cout << "_actual_action = " << _actual_action
					          << " ; action_path.front() == " << p.first.front() << std::endl;

					_previous_actions.emplace_back(p.second, _actual_action);
					//_previous_actions.emplace_back(action_tree.get_table_after_action(_actual_action),
					//_actual_action);
					_total_points += _actual_action.get_nr_points();

					logDebug1("The action '", _actual_action, "' has finished with success.");
					logDebug1("_previous_actions.size() = ",
					          _previous_actions.size(),
					          "; _total_points = ",
					          get_total_points(),
					          "; action_path.size() = ",
					          action_path.size());
				}
				continue;
			}

			logDebug("new action found: abort '", _actual_action, "' and execution of '", action_path.front(), "'");

			// kill the executive thread which will stop the robot before dying
			pthread_cancel(id_thread);

			_actual_action = action_path.front();
			std::cout << "=== Next table : " << p.second << std::endl;
			// execution of the thread: first the action is running and then we stipulate that petri had time to finish
			// its life
			execute_action = std::thread(
			    [&](std::mutex& mutex_petri_running, const Action& action) {
				    petri_returned_value = executioner(dep,
				                                       *(petri.createPetriNet()),
				                                       mutex_petri_running,
				                                       action,
				                                       MATCH_DURATION - _start_time.getElapsedTime());
				    petri_finished = true;
				    std::cout << "MagicStrategy::executioner finished with return value : " << std::boolalpha
				              << petri_returned_value << std::endl;
				    pthread_exit(nullptr);
				},
			    std::ref(mutex_petri_running),
			    std::cref(_actual_action));
			id_thread = execute_action.native_handle();
			execute_action.detach();
		}
		// TODO: sleep(500ms - execution_time) to always guarantee an execution of 500ms?
	}
	sleep(MATCH_DURATION - _start_time.getElapsedTime());
	// TODO: stop everything (deplacement + petri)
}

void StrategyGenerator::MagicStrategy::generate_tree(StrategyGenerator::DecisionalTree& tree, Duration timeout) {
	StopWatch calculation_time;

	DecisionalTree::node* actual_node;
	while((actual_node = tree.pop_frontier_node()) != nullptr /*&& calculation_time.getElapsedTime() < timeout*/) {
		Duration actual_time{tree.calculate_cost(actual_node).second};
		if(actual_time < MATCH_DURATION) {
			for(std::shared_ptr<Element> e : actual_node->data) {
				if(_element_actionable[e->get_type()]()) {
					Table next_table{tree.get_table(actual_node)};
					next_table.erase(e);
					Action associated_action(_element_to_action[e->get_type()](e->get_coordonnees()));
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
