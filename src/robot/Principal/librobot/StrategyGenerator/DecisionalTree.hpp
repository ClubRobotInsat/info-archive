//
// Created by terae on 4/2/18.
//

#ifndef ROOT_DECISIONALTREE_H
#define ROOT_DECISIONALTREE_H

#include "Action.hpp"
#include "Table.hpp"
#include <assert.h>
#include <iomanip>

namespace StrategyGenerator {
	/**
	 * @brief Arbre de recherche pour déterminer la meilleure stratégie
	 *
	 * Cet arbre doit être rempli par une instance extérieure (MagicStrategy)
	 * Il calcule au fur et à mesure la meilleure stratégie en fonction du nombre de points sans dépasser 90_s
	 *
	 * Chaque noeud représente une `Table` ainsi que le temps actuel et les points marqués
	 * Chaque arc représente une `Action` du robot (déplacement inclus)
	 *
	 * Sa structure est la suivante :
	 *
	 *             NOTHING                     BEE                  CUBE
	 * [table à 0s] ----> [BEE, CUBE, SPHERE] ----> [CUBE, SPHERE] ----> [SPHERE]
	 *                                        CUBE
	 *                                        ----> [BEE, SPHERE]
	 *                                        SPHERE
	 *                                        ----> [BEE, CUBE]
	 *
	 * La première action `NOTHING` permet à t=Xs l'ensemble des actions précédemment faites
	 * Ca permet de prendre en compte le temps de jeu et les points marqués pour générer la suite
	 *
	 * Lorsque toute la table a été générée, il suffit d'appeler `generate_action_path()` qui retourne la table suivante
	 * et le chemin optimal pour maximiser les points
	 */
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
		std::size_t _size;
		int max_points;
		node* best_node;

	public:
		struct node {
			node(Table table, edge parent, int previous_points, Duration previous_time)
			        : data(std::move(table)), in_edge(std::move(parent)), cost(std::make_pair(previous_points, previous_time)) {}
			~node() {
				for(edge e : out_edges) {
					delete e.target;
				}
			}

			node* add_child(Table table, Action action) {
				edge in(action, this);
				node* next = new node(std::move(table),
				                      in,
				                      cost.first + action.get_nr_points(),
				                      cost.second + action.get_execution_time(data.get_robot_position()));
				out_edges.emplace_back(action, next);
				return next;
			}

			node* get_parent() const {
				return in_edge.target;
			}

			Table data;
			std::list<edge> out_edges;
			edge in_edge;
			std::pair<int, Duration> cost;
		};

		explicit DecisionalTree(Table initial_state) : _size(0), max_points(0), best_node(nullptr) {
			_first = new node(std::move(initial_state), edge(ActionWait(0_s), nullptr), 0, 0_s);
		}
		~DecisionalTree() {
			delete _first;
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
			// return n->cost;
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
			if(n != nullptr && time < MATCH_DURATION && points > max_points) {
				max_points = points;
				best_node = n;
			}

			return std::make_pair(points, time);
		}

		void add_edge(node* n, Table table, Action action) {
			frontier.push_back(n->add_child(std::move(table), action));
			_size++;
		}

		inline std::size_t size() const {
			return frontier.size();
		}

		std::pair<std::list<Action>, Table> generate_action_path() {
			std::list<Action> action_path;
			Table* next_table = nullptr;
			node* n{best_node};
			while(n != nullptr && n != _first) {
				action_path.push_front(n->in_edge.cost);
				n = n->in_edge.target;
				next_table = &n->data;
			}
			// remove 'previous_actions' from the path
			if(!action_path.empty()) {
				assert(action_path.cbegin()->get_name() == "NOTHING");
				action_path.erase(action_path.cbegin());
			}
			if(next_table == nullptr && !action_path.empty()) {
				throw std::runtime_error("bad correspondence between table and action_path");
			}
			return std::make_pair(action_path, *next_table);
		}

		friend std::ostream& operator<<(std::ostream& os, const DecisionalTree& tree) {
			tree.print_rec(os, tree._first, 0);
			return os;
		}

	private:
		void print_rec(std::ostream& os, node* n, int depth) const {
			if(n != nullptr) {
				std::stringstream ss;
				ss << n->data << '(' << n->cost.first << ", " << n->cost.second << ") ";
				std::size_t size = ss.str().size();
				os << "\033[34m" << n->data << "\033[0m(" << n->cost.first << ", " << n->cost.second << ") ";

				if(n == best_node) {
					os << "*";
				}

				if(!n->out_edges.empty()) {
					std::size_t max_size_action = 0;
					for(const edge& e : n->out_edges) {
						std::stringstream ss;
						ss << e.cost;
						if(ss.str().size() > max_size_action) {
							max_size_action = ss.str().size();
						}
					}

					auto it = n->out_edges.cbegin();
					os << "--> " << std::setw(max_size_action + 1) << std::left << it->cost << "--> ";
					print_rec(os, it->target, size + max_size_action + 9 + depth);

					while(++it != n->out_edges.cend()) {
						os << "\n"
						   << std::string(depth + size, ' ') << "--> " << std::setw(max_size_action + 1) << it->cost << "--> ";
						print_rec(os, it->target, size + max_size_action + 9 + depth);
					}
					os << std::endl;
				}
			}
		}
	};
}

#endif // ROOT_DECISIONALTREE_H
