//
//  Angle.h
//
//  Created by Rémi on 17/01/2015.
//

#ifndef Club_Robot_Angle_h
#define Club_Robot_Angle_h

#include "Unit.h"

/**
 * Classe représentant une grandeur d'angle.
 * Elle gère les radians et les degrés.
 */
class Angle : public NumericValue<Angle> {
	friend class NumericValue<Angle>;
public:
	using NumericValue::ValueType;

	/**
	 * Écrit l'angle sur le flux.
	 * @param s le flux sur lequel écrire l'angle.
	 * @param v l'angle à écrire
	 * @return le flux
	 */
	friend std::ostream &operator<<(std::ostream &s, Angle const &v) {
		return s << v._val << " rad";
	}

	/**
	 * Créé un angle avec la valeur en radians spécifiée.
	 * @param rad la valeur d'angle en radians
	 * @return une grandeur d'angle avec la valeur spécifiée.
	 */
	static inline constexpr Angle makeFromRad(long double rad) { return Angle(rad); }

	/**
	 * Créé un angle avec la valeur en milliradians spécifiée.
	 * @param millirad la valeur d'angle en milliradians
	 * @return une grandeur d'angle avec la valeur spécifiée.
	 */
	static inline constexpr Angle makeFromMilliRad(long double millirad) { return Angle(millirad / 1000); }

	/**
	 * Créé un angle avec la valeur en degrés spécifiée.
	 * @param deg la valeur d'angle en degrés
	 * @return une grandeur d'angle avec la valeur spécifiée.
	 */
	static inline constexpr Angle makeFromDeg(long double deg) {
		return Angle(deg / 180 * M_PI);
	}

	/**
	 * Retourne la valeur stockée sous forme de radians.
	 * Attention, une erreur sera affichée si le type vers lequel est convertie la valeur
	 * ne peut pas représenter cette dernière. Voir la documentation de NumericValue::value() pour plus d'informations.
	 * @return la valeur en radians de l'angle.
	 */
	template<typename Rep = ValueType>
	constexpr Rep toRad() const {
		return (*this).value<Rep>();
	}

	/**
	 * Retourne la valeur stockée sous forme de degrés.
	 * Attention, une erreur sera affichée si le type vers lequel est convertie la valeur
	 * ne peut pas représenter cette dernière. Voir la documentation de NumericValue::value() pour plus d'informations.
	 * @return la valeur en degrés de l'angle.
	 */
	template<typename Rep = ValueType>
	constexpr Rep toDeg() const {
		return (*this * 180 / M_PI).value<Rep>();
	}

	/**
	 * Retourne la valeur stockée sous forme de milliradians.
	 * Attention, une erreur sera affichée si le type vers lequel est convertie la valeur
	 * ne peut pas représenter cette dernière. Voir la documentation de NumericValue::value() pour plus d'informations.
	 * @return la valeur en milliradians de l'angle.
	 */
	template<typename Rep = ValueType>
	constexpr Rep toMilliRad() const {
		return (*this * 1000).value<Rep>();
	}

	/**
	 * Retourne l'angle dans l'intervalle [0, 2π[.
	 * @return une copie de l'instance avec pour valeur son angle dans l'intervalle [0,2π[
	 */
	constexpr Angle angleMod2Pi() const {
		return ((*this % Angle::makeFromRad(2 * M_PI)) + Angle::makeFromRad(4 * M_PI)) % Angle::makeFromRad(2 * M_PI);
	}

	/**
	 * Retourne l'angle dans l'intervalle [-π, π[.
	 * @return une copie de l'instance avec pour valeur son angle dans l'intervalle [-π,π[
	 */
	CONDITIONAL_CONSTEXPR Angle toMoinsPiPi() const {
		auto mod2Pi = this->angleMod2Pi();

		if(mod2Pi >= Angle::makeFromRad(M_PI))
			mod2Pi -= Angle::makeFromRad(2 * M_PI);
		else if(mod2Pi < Angle::makeFromRad(-M_PI))
			mod2Pi += Angle::makeFromRad(2 * M_PI);

		return mod2Pi;
	}

private:
	using NumericValue::NumericValue;
};

/**
 * Permet de créer un angle à partir d'une valeur littérale multiple de π : 1_PI, 2_PI, 0.5_PI …
 */
inline constexpr Angle operator"" _PI(long double val) { return Angle::makeFromRad(val * M_PI); }
inline constexpr Angle operator"" _PI(unsigned long long val) { return Angle::makeFromRad(val * M_PI); }

/**
 * Permet de créer un angle à partir d'une valeur littérale en degrés : 1_deg, 2_deg, 0.5_deg …
 */
inline constexpr Angle operator"" _deg(long double deg) { return Angle::makeFromDeg(deg); }
inline constexpr Angle operator"" _deg(unsigned long long deg) { return Angle::makeFromDeg(deg); }

/**
 * Permet de créer un angle à partir d'une valeur littérale en radians : 1_rad, 2_rad, 0.5_rad …
 */
inline constexpr Angle operator"" _rad(long double rad) { return Angle::makeFromRad(rad); }
inline constexpr Angle operator"" _rad(unsigned long long rad) { return Angle::makeFromRad(rad); }

/**
 * Permet de créer un angle à partir d'une valeur littérale en milliradians : 1_mrad, 2_mrad, 0.5_mrad …
 */
inline constexpr Angle operator"" _mrad(long double rad) { return Angle::makeFromMilliRad(rad); }
inline constexpr Angle operator"" _mrad(unsigned long long rad) { return Angle::makeFromMilliRad(rad); }

/**
 * Permet de considérer un Angle comme un nombre
 */
namespace std {
	template<>
	struct is_scalar<Angle> : public std::integral_constant<bool, true> {};
}

#endif
