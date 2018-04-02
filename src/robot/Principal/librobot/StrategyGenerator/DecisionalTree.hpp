//
// Created by terae on 4/2/18.
//

#ifndef ROOT_DECISIONALTREE_H
#define ROOT_DECISIONALTREE_H

#include "Action.hpp"
#include "Table.hpp"

namespace StrategyGenerator {
	class DecisionalTree {
	public:
		struct node;

	private:
		struct edge {
			edge(Action action, node* ptr) : cost(std::move(action)), target(ptr) {}
			Action cost;
			node* target;
		};

		node* _first;
		std::deque<node*> frontier;

	public:
		int max_points;
		node* best_node;

	public:
		struct node {
			node(Table table, edge parent) : data(std::move(table)), in_edge(std::move(parent)) {}

			node* add_child(Table table, Action action) {
				edge in(action, this);
				node* next = new node(std::move(table), in);
				out_edges.emplace_back(action, next);
				return next;
			}

			node* get_parent() const {
				return in_edge.target;
			}

			Table data;
			std::list<edge> out_edges;
			edge in_edge;
		};


		explicit DecisionalTree(Table initial_state) : max_points(0), best_node(nullptr) {
			_first = new node(initial_state, edge(ActionWait(0_s), nullptr));
			frontier.push_back(_first);
		}

		node* get_root() const {
			return _first;
		}

		Table get_table(node* n) const {
			return n->data;
		}

		node* pop_frontier_node() {
			if(frontier.empty()) {
				return nullptr;
			}
			node* result{frontier.front()};
			frontier.pop_front();
			return result;
		}

		std::pair<int, Duration> calculate_cost(node* n) {
			int points{0};
			Duration time{0_s};
			node* expanded{n};
			while(expanded != nullptr) {
				points += expanded->in_edge.cost.get_nr_points();
				// same position in tables 0 and 1: move time == 0_s (already supported)
				Vector2m robot_position{expanded->get_parent() == nullptr ? expanded->data.get_robot_position() :
				                                                            expanded->get_parent()->data.get_robot_position()};
				time += expanded->in_edge.cost.get_execution_time(robot_position);
				expanded = expanded->in_edge.target;
			}
			if(n != nullptr && time < 90_s && points > max_points) {
				max_points = points;
				best_node = n;
			}

			return std::make_pair(points, time);
		}

		void add_edge(node* n, Table table, Action action) {
			frontier.push_back(n->add_child(std::move(table), action));
		}

		std::list<Action> generate_action_path() {
			std::list<Action> result;
			node* n{best_node};
			while(n != nullptr) {
				result.push_front(n->in_edge.cost);
				n = n->in_edge.target;
			}
			return result;
		}
	};
}

#endif // ROOT_DECISIONALTREE_H
