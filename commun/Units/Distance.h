//
//  Distance.h
//
//  Created by Rémi on 17/01/2015.
//

#ifndef Club_Robot_Distance_h
#define Club_Robot_Distance_h

#include "Unit.h"

using Distance = Unit<0, 1, 0, true>;

/**
 * Classe représentant une grandeur de longueur.
 */
template<>
class Unit<0, 1, 0, true> : public Unit<0, 1, 0, false> {
	friend class Unit<0, 1, 0, false>;
public:
	using Unit<0, 1, 0, false>::ValueType;
	using Type = Unit<0, 1, 0, true>;

	/**
	 * Écrit la longueur sur le flux.
	 * @param s le flux sur lequel écrire la longueur.
	 * @param d la longueur à écrire
	 * @return le flux
	 */
	friend std::ostream &operator<<(std::ostream &s, Type const &d);

	/**
	 * Créé une distance avec la valeur en millimètres spécifiée.
	 * @param mm la valeur de distance en millimètres
	 * @return une grandeur de longueur avec la valeur spécifiée.
	 */
	static constexpr Type makeFromMm(long double mm) { return Type(mm / 1000); }

	/**
	 * Créé une distance avec la valeur en centimètres spécifiée.
	 * @param cm la valeur de distance en centimètres
	 * @return une grandeur de longueur avec la valeur spécifiée.
	 */
	static constexpr Type makeFromCm(long double cm) { return Type(cm / 100); }

	/**
	 * Créé une distance avec la valeur en décimètres spécifiée.
	 * @param dm la valeur de distance en décimètres
	 * @return une grandeur de longueur avec la valeur spécifiée.
	 */
	static constexpr Type makeFromDm(long double dm) { return Type(dm / 10); }

	/**
	 * Créé une distance avec la valeur en mètres spécifiée.
	 * @param m la valeur de distance en mètres
	 * @return une grandeur de longueur avec la valeur spécifiée.
	 */
	static constexpr Type makeFromM(long double m) { return Type(m); }

	/**
	 * Créé une distance avec la valeur en kilomètres spécifiée.
	 * @param km la valeur de distance en kilomètres
	 * @return une grandeur de longueur avec la valeur spécifiée.
	 */
	static constexpr Type makeFromKm(long double km) { return Type(km * 1000); }

	/**
	 * Retourne la valeur stockée sous forme de millimètres.
	 * Attention, une erreur sera affichée si le type vers lequel est convertie la valeur
	 * ne peut pas représenter cette dernière. Voir la documentation de NumericValue::value() pour plus d'informations.
	 * @return la valeur en millimètres de la distance.
	 */
	template<typename Rep = ValueType>
	constexpr Rep toMm() const {
		return (*this * 1000).value<Rep>();
	}

	/**
	 * Retourne la valeur stockée sous forme de mètres.
	 * Attention, une erreur sera affichée si le type vers lequel est convertie la valeur
	 * ne peut pas représenter cette dernière. Voir la documentation de NumericValue::value() pour plus d'informations.
	 * @return la valeur en mètres de la distance.
	 */
	template<typename Rep = ValueType>
	constexpr Rep toM() const {
		return (*this).value<Rep>();
	}

	/**
	 * Retourne la valeur stockée sous forme de décimètres.
	 * Attention, une erreur sera affichée si le type vers lequel est convertie la valeur
	 * ne peut pas représenter cette dernière. Voir la documentation de NumericValue::value() pour plus d'informations.
	 * @return la valeur en décimètres de la distance.
	 */
	template<typename Rep = ValueType>
	constexpr Rep toDm() const {
		return (*this * 10).value<Rep>();
	}


private:
	using Unit<0, 1, 0, false>::Unit;
};


/**
 * Permet de créer une distance à partir d'une valeur littérale en millimètres : 1_mm, 2_mm, 0.5_mm …
 */
inline constexpr Distance operator"" _mm(long double dist) { return Distance::makeFromMm(dist); }
inline constexpr Distance operator"" _mm(unsigned long long dist) { return Distance::makeFromMm(dist); }

/**
 * Permet de créer une distance à partir d'une valeur littérale en centimètres : 1_cm, 2_cm, 0.5_cm …
 */
inline constexpr Distance operator"" _cm(long double dist) { return Distance::makeFromCm(dist); }
inline constexpr Distance operator"" _cm(unsigned long long dist) { return Distance::makeFromCm(dist); }

/**
 * Permet de créer une distance à partir d'une valeur littérale en décimètres : 1_dm, 2_dm, 0.5_dm …
 */
inline constexpr Distance operator"" _dm(long double dist) { return Distance::makeFromDm(dist); }
inline constexpr Distance operator"" _dm(unsigned long long dist) { return Distance::makeFromDm(dist); }

/**
 * Permet de créer une distance à partir d'une valeur littérale en mètres : 1_m, 2_m, 0.5_m …
 */
inline constexpr Distance operator"" _m(long double dist) { return Distance::makeFromM(dist); }
inline constexpr Distance operator"" _m(unsigned long long dist) { return Distance::makeFromM(dist); }

/**
 * Permet de créer une distance à partir d'une valeur littérale en kilomètres : 1_km, 2_km, 0.5_km …
 */
inline constexpr Distance operator"" _km(long double dist) { return Distance::makeFromKm(dist); }
inline constexpr Distance operator"" _km(unsigned long long dist) { return Distance::makeFromKm(dist); }

/**
 * Permet de considérer une Distance comme un nombre
 */
namespace std {
	template<>
	struct is_scalar<Distance> : public std::integral_constant<bool, true> {};
}

#endif
