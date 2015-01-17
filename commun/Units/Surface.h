//
//  Surface.h
//
//  Created by Rémi on 17/01/2015.
//

#ifndef Club_Robot_Surface_h
#define Club_Robot_Surface_h

#include "Unit.h"

/**
 * Classe représentant une grandeur de surface, longueur au carré.
 */
class Surface : public NumericValue<Surface> {
	friend class NumericValue<Surface>;
public:
	using NumericValue::ValueType;

	/**
	 * Écrit la surface sur le flux.
	 * @param s le flux sur lequel écrire la surface.
	 * @param v la surface à écrire
	 * @return le flux
	 */
	friend std::ostream &operator<<(std::ostream &stream, Surface const &v) {
		return stream << v._val << " m2";
	}

	/**
	 * Créé une surface avec la valeur en millimètres carrés spécifiée.
	 * @param mm2 la valeur de surface en millimètres carrés
	 * @return une grandeur de surface avec la valeur spécifiée.
	 */
	static constexpr Surface makeFromMm2(long double mm2) { return Surface(mm2 / 1000000); }

	/**
	 * Créé une surface avec la valeur en centimètres carrés spécifiée.
	 * @param cm2 la valeur de surface en centimètres carrés
	 * @return une grandeur de surface avec la valeur spécifiée.
	 */
	static constexpr Surface makeFromCm2(long double cm2) { return Surface(cm2 / 10000); }

	/**
	 * Créé une surface avec la valeur en decimètres carrés spécifiée.
	 * @param dm2 la valeur de surface en decimètres carrés
	 * @return une grandeur de surface avec la valeur spécifiée.
	 */
	static constexpr Surface makeFromDm2(long double dm2) { return Surface(dm2 / 100); }

	/**
	 * Créé une surface avec la valeur en mètres carrés spécifiée.
	 * @param m2 la valeur de surface en mètres carrés
	 * @return une grandeur de surface avec la valeur spécifiée.
	 */
	static constexpr Surface makeFromM2(long double m2) { return Surface(m2); }

	/**
	 * Retourne la valeur stockée sous forme de mètres carrés.
	 * Attention, une erreur sera affichée si le type vers lequel est convertie la valeur
	 * ne peut pas représenter cette dernière. Voir la documentation de NumericValue::value() pour plus d'informations.
	 * @return la valeur en mètres par seconde de la surface.
	 */
	template<typename Rep = ValueType>
	constexpr Rep toM2() const {
		return (*this).value<Rep>();
	}

	/**
	 * Retourne la valeur stockée sous forme de millimètres carrés.
	 * Attention, une erreur sera affichée si le type vers lequel est convertie la valeur
	 * ne peut pas représenter cette dernière. Voir la documentation de NumericValue::value() pour plus d'informations.
	 * @return la valeur en millimètres carrés de la surface.
	 */
	template<typename Rep = ValueType>
	constexpr Rep toMm2() const {
		return (*this * 1000000).value<Rep>();
	}

private:
	using NumericValue::NumericValue;
};

/**
 * Permet de créer une surface à partir d'une valeur littérale en millimètres carrés : 1_mm2, 2_mm2, 0.5_mm2 …
 */
inline constexpr Surface operator"" _mm2(long double dist) { return Surface::makeFromMm2(dist); }
inline constexpr Surface operator"" _mm2(unsigned long long dist) { return Surface::makeFromMm2(dist); }

/**
 * Permet de considérer une Surface comme un nombre
 */
namespace std {
	template<>
	struct is_scalar<Surface> : public std::integral_constant<bool, true> {};
}

#endif
