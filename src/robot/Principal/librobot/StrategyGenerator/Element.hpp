//
// Created by terae on 3/29/18.
//

#ifndef ROOT_ELEMENT_H
#define ROOT_ELEMENT_H

#include <MathToolbox/MathToolbox.h>
#include <functional>
#include <memory>

#include "Action.hpp"

namespace StrategyGenerator {
	enum class ElementType : std::size_t { BEE, SWITCH, CUBE, SPHERE };
	class Action;

	class Element : public std::enable_shared_from_this<Element> {
		ElementType _type;

		// Middle of the position
		Vector2m _position;

	public:
		explicit Element(ElementType type, const Vector2m& pos) : _type(type), _position(pos) {}

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

		friend bool operator==(const Element& e1, const Element& e2) {
			return e1.get_type() == e2.get_type() && e1.get_position() == e2.get_position();
		}

		friend bool operator!=(const Element& e1, const Element& e2) {
			return !(e1 == e2);
		}
	};
}


#endif // ROOT_ELEMENT_H
