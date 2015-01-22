//
//  Units.h
//
//  Created by Rémi on 17/01/2015.
//

#ifndef Club_Robot_Units_h
#define Club_Robot_Units_h

#include "Unit.h"

#include "Angle.h"
#include "Distance.h"
#include "Surface.h"
#include "Duree.h"
#include "Masse.h"
#include "VitesseLineaire.h"
#include "VitesseAngulaire.h"

/**
 * Retourne l'angle du vecteur (x, y)
 */
inline Angle atan2(Distance const &y, Distance const &x) {
	return Angle::makeFromRad(std::atan2(y.toM(), x.toM()));
}

/**
 * Divise une distance par une durée, pour former une vitesse linéaire
 */
inline VitesseLineaire operator/(Distance const &d, Duree const &t) {
	return VitesseLineaire::makeFromM_s(d.toM() / t.toS());
}

/**
 * Divise une distance par une vitesse linéaire, pour former une durée
 */
inline Duree operator/(Distance const &d, VitesseLineaire const &v) {
	return Duree::makeFromS(d.toM() / v.toM_s());
}

/**
 * Multiplie une vitesse lineaire par une durée, pour former une distance
 */
inline Distance operator*(VitesseLineaire const &v, Duree const &t) {
	return Distance::makeFromM(v.toM_s() * t.toS());
}

/**
 * Multiplie une durée par une vitesse linéaire, pour former une distance
 */
inline Distance operator*(Duree const &t, VitesseLineaire const &v) {
	return Distance::makeFromM(v.toM_s() * t.toS());
}

/**
 * Divise un angle par une durée, pour former une vitesse angulaire
 */
inline VitesseAngulaire operator/(Angle const &a, Duree const &t) {
	return VitesseAngulaire::makeFromRad_s(a.toRad() / t.toS());
}

/**
 * Divise un angle par une vitesse angulaire, pour former une durée
 */
inline Duree operator/(Angle const &a, VitesseAngulaire const &v) {
	return Duree::makeFromS(a.toRad() / v.toRad_s());
}

/**
 * Multiplie une vitesse angulaire par une durée, pour former un angle
 */
inline Angle operator*(VitesseAngulaire const &a, Duree const &t) {
	return Angle::makeFromRad(a.toRad_s() * t.toS());
}

/**
 * Multiplie une dirée par une vitesse angulaire, pour former un angle
 */
inline Angle operator*(Duree const &t, VitesseAngulaire const &a) {
	return Angle::makeFromRad(a.toRad_s() * t.toS());
}

/**
 * Retourne la racine carrée de la surface d, c'est à dire la longueur du côté d'un carré de surface d
 */
inline Distance sqrt(Surface const &d) {
	return Distance::makeFromM(std::sqrt(d.toM2()));
}


/**
 * Multiplie deux longueurs et retourne la surface calculée
 */
inline Surface operator*(Distance const &d1, Distance const &d2) {
	return Surface::makeFromM2(d1.toM() * d2.toM());
}

/**
 * Retourne le produit d'une sous-classe de NumericValue avec un nombre.
 * Le nombre ne doit pas représenter une grandeur physique (il doit être un simple scalaire).
 * En revanche, il doit correspondre au type trait std::scalar.
 *
 * @param v1 le premier facteur du produit
 * @param v2 le deuxième facteur du produit
 * @return une nouvelle instance de la sous-classe de NumericValue représentant la multiplication de l'instance courante par le paramètre
 */
template <typename T_, typename Scalar>
constexpr std::enable_if_t<std::is_base_of<NumericValue<T_, typename T_::ValueType>, T_>::value && std::is_scalar<Scalar>::value, T_>
operator*(Scalar s, T_ const &num) {
	return num * s;
}


#endif
