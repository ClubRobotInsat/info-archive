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
		repere::Coordonnees _robot_coords;

		friend class MagicStrategy;

	public:
		Table() = default;

		void set_robot_coords(repere::Coordonnees robot_coords) {
			_robot_coords = std::move(robot_coords);
		}

		/*std::size_t hash() {
		    std::size_t seed = this->size();
		    for(const std::shared_ptr<Element>& e : *this) {
		        seed ^= e->hash() + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		    }
		    return seed;
		}*/

		Vector2m get_robot_position() const {
			return _robot_coords.getPos2D();
		}
	};
}

#endif // ROOT_ENVIRONMENT_H
