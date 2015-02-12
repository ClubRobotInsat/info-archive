//
//  Units.h
//
//  Created by Rémi on 17/01/2015.
//

#ifndef Club_Robot_Units_h
#define Club_Robot_Units_h

#include "Unit.h"

using Grandeur = Unit<0, 0, 0, true>;

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
 * Retourne la racine carrée de la surface s, c'est à dire la longueur du côté d'un carré de surface s
 */
inline Distance sqrt(Surface const &s) {
	return Distance::makeFromM(std::sqrt(s.toM2()));
}

/**
 * Retourne le produit d'une grandeur physique avec une autre grandeur physique.
 * Le type de retour est la grandeur physique correspondant (ex : vitesse * temps => distance)
 *
 * @param v1 le premier facteur du produit
 * @param v2 le deuxième facteur du produit
 * @return une nouvelle instance représentant la multiplication des deux paramètres
 */
template<int Kg1, int M1, int S1, int Kg2, int M2, int S2>
constexpr auto operator*(Unit<Kg1, M1, S1, true> const &t1, Unit<Kg2, M2, S2, true> const &t2) -> Unit<Kg1 + Kg2, M1 + M2, S1 + S2, true> {
	return Unit<Kg1 + Kg2, M1 + M2, S1 + S2, true>::makeFromValue(t1.value() * t2.value());
}

/**
 * Retourne le quotient de deux grandeurs physiques. Le type de retour est la grandeur physique correspondant au quotient.
 *
 * @param v1 le dividende de la division
 * @param v2 le diviseur de la division
 * @return une nouvelle instance représentant la division des deux paramètres
 */
template<int Kg1, int M1, int S1, int Kg2, int M2, int S2>
constexpr auto operator/(Unit<Kg1, M1, S1, true> const &t1, Unit<Kg2, M2, S2, true> const &t2) -> Unit<Kg1 - Kg2, M1 - M2, S1 - S2, true> {
	return Unit<Kg1 - Kg2, M1 - M2, S1 - S2, true>::makeFromValue(t1.value() / t2.value());
}

template<int Kg1, int M1, int S1>
constexpr auto operator/(Unit<Kg1, M1, S1, true> const &t1, Unit<Kg1, M1, S1, true> const &t2) -> UnitBase::ValueType {
	return t1.value() / t2.value();
}



#endif
