//
// Created by terae on 3/29/18.
//

#ifndef ROOT_ELEMENT_H
#define ROOT_ELEMENT_H

//#include "Table.hpp"
//#include "/home/terae/Programs/Graph/single_include/graph.hpp"
#include <MathToolbox/MathToolbox.h>
#include <functional>
#include <memory>
//#include <vector>

#include "Action.hpp"

namespace StrategyGenerator {
	enum class ElementType : std::size_t { BEE, SWITCH, CUBE, SPHERE };
	class Action;

	class Element : public std::enable_shared_from_this<Element> {
		ElementType _type;

		// Middle of the position
		Vector2m _position;

		// TRUE if initial position; FALSE if has was moved in the match
		bool _initial_pos;

	public:
		explicit Element(ElementType type, const Vector2m& pos) : _type(type), _position(pos), _initial_pos(true) {}

		inline ElementType get_type() const {
			return _type;
		}

		inline Vector2m get_position() const {
			return _position;
		}

		std::size_t hash() {
			return std::hash<int>()(static_cast<int>(_type)) + std::hash<double>()(_position.x.toMm()) +
			       std::hash<double>()(_position.y.toMm());
		}

		/*Action get_associated_action() const {
		    return _action_generator(_position);
		}

		Table get_next_table(Table table) const {
		    // TODO
		    return table;
		}*/
	};
}


#endif // ROOT_ELEMENT_H
