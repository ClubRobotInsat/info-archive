//
//  Masse.h
//
//  Created by Rémi on 17/01/2015.
//

#ifndef Club_Robot_Masse_h
#define Club_Robot_Masse_h

#include "Unit.h"

/**
 * Classe représentant une grandeur de masse.
 */
class Masse : public NumericValue<Masse> {
	friend class NumericValue<Masse>;
public:
	using NumericValue::ValueType;

	/**
	 * Écrit la masse sur le flux.
	 * @param s le flux sur lequel écrire la masse.
	 * @param v la masse à écrire
	 * @return le flux
	 */
	friend std::ostream &operator<<(std::ostream &stream, Masse const &v) {
		return stream << v._val << " kg";
	}

	/**
	 * Créé une masse avec la valeur en grammes spécifiée.
	 * @param g la valeur de masse en grammes
	 * @return une grandeur de masse avec la valeur spécifiée.
	 */
	static constexpr Masse makeFromG(long double g) { return Masse(g / 1000); }

	/**
	 * Créé une masse avec la valeur en kilogrammes spécifiée.
	 * @param kg la valeur de masse en kilogrammes
	 * @return une grandeur de masse avec la valeur spécifiée.
	 */
	static constexpr Masse makeFromKg(long double kg) { return Masse(kg); }

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
	using NumericValue::NumericValue;
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
