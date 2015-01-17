//
//  VitesseAngulaire.h
//
//  Created by Rémi on 17/01/2015.
//

#ifndef Club_Robot_VitesseAngulaire_h
#define Club_Robot_VitesseAngulaire_h

#include "Unit.h"

/**
 * Classe représentant une grandeur de vitesse angulaire.
 */
class VitesseAngulaire : public NumericValue<VitesseAngulaire> {
	friend class NumericValue<VitesseAngulaire>;
public:
	using NumericValue::ValueType;

	/**
	 * Écrit la vitesse angulaire sur le flux.
	 * @param s le flux sur lequel écrire la vitesse angulaire.
	 * @param v la vitesse angulaire à écrire
	 * @return le flux
	 */
	friend inline std::ostream &operator<<(std::ostream &s, VitesseAngulaire const &v) {
		return s << v._val << " rad/s";
	}

	/**
	 * Créé une vitesse angulaire avec la valeur en radians par secondes spécifiée.
	 * @param rad_s la valeur de vitesse angulaire en radians par secondes
	 * @return une grandeur de vitesse angulaire avec la valeur spécifiée.
	 */
	static constexpr VitesseAngulaire makeFromRad_s(long double rad_s) { return VitesseAngulaire(rad_s); }

	/**
	 * Créé une vitesse angulaire avec la valeur en degrés par secondes spécifiée.
	 * @param rad_s la valeur de vitesse angulaire en degrés par secondes
	 * @return une grandeur de vitesse angulaire avec la valeur spécifiée.
	 */
	static constexpr VitesseAngulaire makeFromDeg_s(long double deg_s) { return VitesseAngulaire(deg_s * M_PI / 180); }

	/**
	 * Créé une vitesse angulaire avec la valeur en milliradians par secondes spécifiée.
	 * @param rad_s la valeur de vitesse angulaire en milliradians par secondes
	 * @return une grandeur de vitesse angulaire avec la valeur spécifiée.
	 */
	static constexpr VitesseAngulaire makeFromMilliRad_s(long double millirad_s) { return VitesseAngulaire(millirad_s / 1000); }

	/**
	 * Retourne la valeur stockée sous forme de radians par seconde.
	 * Attention, une erreur sera affichée si le type vers lequel est convertie la valeur
	 * ne peut pas représenter cette dernière. Voir la documentation de NumericValue::value() pour plus d'informations.
	 * @return la valeur en radians par seconde de la vitesse angulaire.
	 */
	template<typename Rep = ValueType>
	constexpr Rep toRad_s() const {
		return (*this).value<Rep>();
	}

	/**
	 * Retourne la valeur stockée sous forme de degrés par seconde.
	 * Attention, une erreur sera affichée si le type vers lequel est convertie la valeur
	 * ne peut pas représenter cette dernière. Voir la documentation de NumericValue::value() pour plus d'informations.
	 * @return la valeur en degrés par seconde de la vitesse angulaire.
	 */
	template<typename Rep = ValueType>
	constexpr Rep toMilliRad_s() const {
		return (*this * 1000).value<Rep>();
	}

private:
	using NumericValue::NumericValue;
};

/**
 * Permet de créer une vitesse angulaire à partir d'une valeur littérale en radians par seconde : 1_rad_s, 2_rad_s, 0.5_rad_s …
 */
inline constexpr VitesseAngulaire operator"" _rad_s(long double v) { return VitesseAngulaire::makeFromRad_s(v); }
inline constexpr VitesseAngulaire operator"" _rad_s(unsigned long long v) { return VitesseAngulaire::makeFromRad_s(v); }

/**
 * Permet de créer une vitesse angulaire à partir d'une valeur littérale en degrés par seconde : 1_deg_s, 2_deg_s, 0.5_deg_s …
 */
inline constexpr VitesseAngulaire operator"" _deg_s(long double v) { return VitesseAngulaire::makeFromDeg_s(v); }
inline constexpr VitesseAngulaire operator"" _deg_s(unsigned long long v) { return VitesseAngulaire::makeFromDeg_s(v); }

/**
 * Permet de considérer une VitesseAngulaire comme un nombre
 */
namespace std {
	template<>
	struct is_scalar<VitesseAngulaire> : public std::integral_constant<bool, true> {};
}

#endif
