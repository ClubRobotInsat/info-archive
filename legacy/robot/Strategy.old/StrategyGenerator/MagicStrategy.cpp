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

	auto petri_net = petri.createPetriNet();
	std::thread execute_action = std::thread(&MagicStrategy::action_executor, this, std::ref(dep), std::ref(*petri_net));

	execute_action.detach();

	while(_start_time.getElapsedTime() < MATCH_DURATION * 10000) {
		StopWatch calculation_time;
		calculation_time.reset();

		// ----------------------------------------------------------------- //
		DecisionalTree action_tree(_initial_table);

		action_tree.add_edge(action_tree.get_root(),
		                     _previous_actions.back().first,
		                     Action(_start_time.getElapsedTime(),
		                            _total_points,
		                            Element(ElementType::NOTHING, _previous_actions.back().second->get_coordonnees()),
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

		Action best_action = action_path.front();

		// the robot should change its strategy (new action path found are previous action finished)
		if(_actual_action == nullptr || best_action != *_actual_action) {
			std::cout << "decisioner: new best action found: " << best_action.get_name() << std::endl;
			std::lock_guard<std::mutex> lk(_mut_action_available);

			while(_state != ActionState::WAIT) {
				if(_mut_petri_running.try_lock()) {
					_mut_petri_running.unlock();

					dep.arreter();
				} else {
					break;
				}
			}
			std::cout << "decisioner: _state == WAIT" << std::endl;

			if(!_mut_petri_running.try_lock()) {
				continue;
			}
			_mut_petri_running.unlock();

			if(_last_petri_succeed == true) {
				std::cout << "decisioner: last petri succeed" << std::endl;
				std::cout << "new table : " << p.second << std::endl;
				std::cout << "_actual_action = " << *_actual_action << " ; action_path.front() == " << p.first.front() << std::endl;

				_previous_actions.emplace_back(p.second, _actual_action);
				//_previous_actions.emplace_back(action_tree.get_table_after_action(_actual_action),
				//_actual_action);
				_total_points += _actual_action->get_nr_points();

				logDebug1("The action '", _actual_action, "' has finished with success.");
				logDebug1("_previous_actions.size() = ",
				          _previous_actions.size(),
				          "; _total_points = ",
				          get_total_points(),
				          "; action_path.size() = ",
				          action_path.size());
				_last_petri_succeed = false;
			}
			_actual_action = std::make_shared<Action>(best_action);
			_ia_cv.notify_all();
			std::cout << "decisioner: notify all" << std::endl;
		}
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
