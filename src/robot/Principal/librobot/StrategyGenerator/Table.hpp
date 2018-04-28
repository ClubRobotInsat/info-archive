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
	/**
	 * @brief Représente la table connue du robot à un instant t
	 *
	 * Une table dépend d'un ensemble d'éléments (type + coords) ainsi que la position du robot
	 *
	 * @code
	 * Table t;
	 * t.emplace(std::make_shared<Element>(ElementType::SPHERE, Coordonnees({10_cm, 50_cm})));
	 * t.emplace(std::make_shared<Element>(ElementType::CUBE, Coordonnees({50_cm, 2_m})));
	 * t.set_robot_coords(Coordonnees({1.5_m, 50_cm}, 40_deg));
	 * @endcode
	 */
	class Table : public std::set<std::shared_ptr<Element>> {
		repere::Coordonnees _robot_coords;

		friend class MagicStrategy;

	public:
		Table() = default;

		void set_robot_coords(const repere::Coordonnees& robot_coords) {
			_robot_coords = robot_coords;
		}

		/*std::size_t hash() {
		    std::size_t seed = this->size();
		    for(const std::shared_ptr<Element>& e : *this) {
		        seed ^= e->hash() + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		    }
		    return seed;
		}*/

		/// @return Position du robot dans l'`ABSOLUTE_REFERENCE`
		Vector2m get_robot_position() const {
			return _robot_coords.getPos2D();
		}

		friend std::ostream& operator<<(std::ostream& os, const Table table) {
			os << '[';
			for(auto it = table.cbegin(); it != table.cend();) {
				os << toString((*it)->get_type()) << (++it != table.cend() ? ", " : "");
			}
			return os << ']';
		}
	};
}

#endif // ROOT_ENVIRONMENT_H
