//
//  VitesseAngulaire.h
//
//  Created by Rémi on 17/01/2015.
//

#ifndef Club_Robot_VitesseAngulaire_h
#define Club_Robot_VitesseAngulaire_h

#include "Unit.h"

using VitesseAngulaire = Unit<0, 0, -1, true>;

/**
 * Classe représentant une grandeur de vitesse angulaire.
 */
template<>
class Unit<0, 0, -1, true> : public Unit<0, 0, -1, false> {
	friend class Unit<0, 0, -1, false>;
public:
	using Unit<0, 0, -1, false>::ValueType;
	using Type = Unit<0, 0, -1, true>;

	/**
	 * Écrit la vitesse angulaire sur le flux.
	 * @param s le flux sur lequel écrire la vitesse angulaire.
	 * @param v la vitesse angulaire à écrire
	 * @return le flux
	 */
	friend std::ostream &operator<<(std::ostream &s, Type const &v);

	/**
	 * Créé une vitesse angulaire avec la valeur en radians par secondes spécifiée.
	 * @param rad_s la valeur de vitesse angulaire en radians par secondes
	 * @return une grandeur de vitesse angulaire avec la valeur spécifiée.
	 */
	static constexpr Type makeFromRad_s(long double rad_s) { return Type(rad_s); }

	/**
	 * Créé une vitesse angulaire avec la valeur en degrés par secondes spécifiée.
	 * @param rad_s la valeur de vitesse angulaire en degrés par secondes
	 * @return une grandeur de vitesse angulaire avec la valeur spécifiée.
	 */
	static constexpr Type makeFromDeg_s(long double deg_s) { return Type(deg_s * M_PI / 180); }

	/**
	 * Créé une vitesse angulaire avec la valeur en milliradians par secondes spécifiée.
	 * @param rad_s la valeur de vitesse angulaire en milliradians par secondes
	 * @return une grandeur de vitesse angulaire avec la valeur spécifiée.
	 */
	static constexpr Type makeFromMilliRad_s(long double millirad_s) { return Type(millirad_s / 1000); }

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
	using Unit<0, 0, -1, false>::Unit;
};

/**
 * Permet de créer une vitesse angulaire à partir d'une valeur littérale en radians par seconde : 1_rad_s, 2_rad_s, 0.5_rad_s …
 */
inline constexpr VitesseAngulaire operator"" _rad_s(long double v) { return VitesseAngulaire::makeFromRad_s(v); }
inline constexpr VitesseAngulaire operator"" _rad_s(unsigned long long v) { return VitesseAngulaire::makeFromRad_s(v); }

/**
 * Permet de créer une vitesse angulaire à partir d'une valeur littérale en milli-radians par seconde : 1_mrad_s, 2_mrad_s, 0.5_mrad_s …
 */
inline constexpr VitesseAngulaire operator"" _mrad_s(long double v) { return VitesseAngulaire::makeFromRad_s(v/1000); }
inline constexpr VitesseAngulaire operator"" _mrad_s(unsigned long long v) { return VitesseAngulaire::makeFromRad_s(v/1000.0); }

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
