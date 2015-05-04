//
//  Masse.h
//
//  Created by Rémi on 17/01/2015.
//

#ifndef Club_Robot_Masse_h
#define Club_Robot_Masse_h

#include "Unit.h"

using Masse = Unit<1, 0, 0, true>;

/**
 * Classe représentant une grandeur de masse.
 */
template<>
class Unit<1, 0, 0, true> : public Unit<1, 0, 0, false> {
	friend class Unit<1, 0, 0, false>;
public:
	using Unit<1, 0, 0, false>::ValueType;
	using Type = Unit<1, 0, 0, true>;

	/**
	 * Écrit la masse sur le flux.
	 * @param s le flux sur lequel écrire la masse.
	 * @param v la masse à écrire
	 * @return le flux
	 */
	friend std::ostream &operator<<(std::ostream &stream, Type const &v);

	/**
	 * Créé une masse avec la valeur en grammes spécifiée.
	 * @param g la valeur de masse en grammes
	 * @return une grandeur de masse avec la valeur spécifiée.
	 */
	static constexpr Type makeFromG(long double g) { return Type(g / 1000); }

	/**
	 * Créé une masse avec la valeur en kilogrammes spécifiée.
	 * @param kg la valeur de masse en kilogrammes
	 * @return une grandeur de masse avec la valeur spécifiée.
	 */
	static constexpr Type makeFromKg(long double kg) { return Type(kg); }

	/**
	 * Retourne la valeur stockée sous forme de kilogrammes.
	 * Attention, une erreur sera affichée si le type vers lequel est convertie la valeur
	 * ne peut pas représenter cette dernière. Voir la documentation de NumericValue::value() pour plus d'informations.
	 * @return la valeur en kilogrammes de la masse.
	 */
	template<typename Rep = ValueType>
	constexpr Rep toKg() const {
		return (*this).value<Rep>();
	}

private:
	using Unit<1, 0, 0, false>::Unit;
};

/**
 * Permet de créer une masse à partir d'une valeur littérale en grammes : 1_g, 2_g, 0.5_g …
 */
inline constexpr Masse operator"" _g(long double g) { return Masse::makeFromG(g); }
inline constexpr Masse operator"" _g(unsigned long long g) { return Masse::makeFromG(g); }

/**
 * Permet de créer une masse à partir d'une valeur littérale en kilogrammes : 1_kg, 2_kg, 0.5_kg …
 */
inline constexpr Masse operator"" _kg(long double kg) { return Masse::makeFromKg(kg); }
inline constexpr Masse operator"" _kg(unsigned long long kg) { return Masse::makeFromKg(kg); }

/**
 * Permet de considérer une Masse comme un nombre
 */
namespace std {
	template<>
	struct is_scalar<Masse> : public std::integral_constant<bool, true> {};
}

#endif
