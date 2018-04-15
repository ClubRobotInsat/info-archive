//
// Created by terae on 3/29/18.
//

#ifndef ROOT_ELEMENT_H
#define ROOT_ELEMENT_H

#include <MathToolbox/MathToolbox.h>
#include <functional>
#include <memory>

#include "Action.hpp"
#include "Enum/Enum.h"

namespace StrategyGenerator {
	/**
	 * @brief Type d'un élément qui permet de lui associer une action spécifique
	 *
	 * Cette enum est à mettre à jour pour chaque stratégie/année
	 */
	ENUM_CLASS_NS(StrategyGenerator, ElementType, NOTHING, BEE, SWITCH, CUBE, SPHERE);

	class Action;

	/**
	 * @brief Représente un élément sur la table sur lequel le robot doit faire une action
	 *
	 * Un élément peut être défini statiquement en début de match, ou être rajouté dynamiquement pendant le match
	 *
	 * @todo Créer une fonction qui génère un élément depuis des capteurs (reconnaissance de forme + position)
	 */
	class Element : public std::enable_shared_from_this<Element> {
		ElementType _type;

		/// Représente les coordonnées du centre de l'élément
		repere::Coordonnees _coordonnees;

	public:
		explicit Element(ElementType type, const repere::Coordonnees& coords) : _type(type), _coordonnees(coords) {}

		inline ElementType get_type() const {
			return _type;
		}

		inline repere::Coordonnees get_coordonnees() const {
			return _coordonnees;
		}

		/*std::size_t hash() {
		    return std::hash<int>()(static_cast<int>(_type)) + std::hash<double>()(_position.x.toMm()) +
		           std::hash<double>()(_position.y.toMm());
		}*/

		friend bool operator==(const Element& e1, const Element& e2) {
			return e1.get_type() == e2.get_type() && e1.get_coordonnees().getPos2D() == e2.get_coordonnees().getPos2D();
		}

		friend bool operator!=(const Element& e1, const Element& e2) {
			return !(e1 == e2);
		}
	};
}


#endif // ROOT_ELEMENT_H
