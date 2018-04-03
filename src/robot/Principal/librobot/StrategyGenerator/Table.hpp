//
// Created by terae on 27/01/18.
//

#ifndef ROOT_ENVIRONMENT_H
#define ROOT_ENVIRONMENT_H

#include <memory>
#include <set>

#include "Action.hpp"
#include "Element.hpp"

namespace StrategyGenerator {
	class Table : public std::set<std::shared_ptr<Element>> {
		Vector2m _robot_position;

	public:
		// using set::set;
		Table(Vector2m robot_position) : _robot_position(robot_position) {}

		std::size_t hash() {
			std::size_t seed = this->size();
			for(const std::shared_ptr<Element>& e : *this) {
				seed ^= e->hash() + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			}
			return seed;
		}

		Vector2m get_robot_position() const {
			return _robot_position;
		}
	};
}

#endif // ROOT_ENVIRONMENT_H
