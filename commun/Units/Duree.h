//
//  Duree.h
//
//  Created by Rémi on 17/01/2015.
//

#ifndef Club_Robot_Duree_h
#define Club_Robot_Duree_h

#include "Unit.h"
#include <chrono>

using Duree = Unit<0, 0, 1, true>;
using TimePoint = Duree;

/**
 * Classe représentant une grandeur de temps.
 */
template<>
class Unit<0, 0, 1, true> : public Unit<0, 0, 1, false> {
	friend class Unit<0, 0, 1, false>;
public:
	using Unit<0, 0, 1, false>::ValueType;
	using Type = Unit<0, 0, 1, true>;

	/**
	 * Écrit la durée sur le flux.
	 * @param s le flux sur lequel écrire la durée.
	 * @param d la durée à écrire
	 * @return le flux
	 */
	friend inline std::ostream &operator<<(std::ostream &s, Type const &d) {
		if(abs(d._val) >= 3600)
			s << d._val / 3600 << " h";
		else if(abs(d._val) >= 60)
			s << d._val / 60 << " min";
		else if(abs(d._val) >= 1)
			s << d._val << " s";
		else if(abs(d._val) >= 1e-3)
			s << d._val * 1e3 << " ms";
		else if(abs(d._val) >= 1e-6)
			s << d._val * 1e6 << " us";
		else
			s << d._val * 1e9 << " ns";

		return s;
	}

	/**
	 * Créé une durée avec la valeur en nanosecondes spécifiée.
	 * @param ns la valeur de durée en nanosecondes
	 * @return une grandeur de durée avec la valeur spécifiée.
	 */
	static constexpr Type makeFromNs(long double ns) { return Type(ns / 1e9); }

	/**
	 * Créé une durée avec la valeur en microsecondes spécifiée.
	 * @param us la valeur de durée en microsecondes
	 * @return une grandeur de durée avec la valeur spécifiée.
	 */
	static constexpr Type makeFromUs(long double us) { return Type(us / 1e6); }

	/**
	 * Créé une durée avec la valeur en millisecondes spécifiée.
	 * @param ms la valeur de durée en millisecondes
	 * @return une grandeur de durée avec la valeur spécifiée.
	 */

	static constexpr Type makeFromMs(long double ms) { return Type(ms / 1e3); }

	/**
	 * Créé une durée avec la valeur en secondes spécifiée.
	 * @param s la valeur de durée en secondes
	 * @return une grandeur de durée avec la valeur spécifiée.
	 */
	static constexpr Type makeFromS(long double s) { return Type(s); }

	/**
	 * Retourne la valeur stockée sous forme de secondes.
	 * Attention, une erreur sera affichée si le type vers lequel est convertie la valeur
	 * ne peut pas représenter cette dernière. Voir la documentation de NumericValue::value() pour plus d'informations.
	 * @return la valeur en secondes de la durée.
	 */
	template<typename Rep = ValueType>
	constexpr Rep toS() const {
		return (*this).value<Rep>();
	}

	/**
	 * Retourne la valeur stockée sous forme de std::chrono::nanoseconds.
	 * Attention, une erreur sera affichée si le type vers lequel est convertie la valeur
	 * ne peut pas représenter cette dernière. Voir la documentation de NumericValue::value() pour plus d'informations.
	 * @return la valeur en std::chrono::nanoseconds de la durée.
	 */
	std::chrono::nanoseconds toSystemDelay() const {
		return std::chrono::nanoseconds(static_cast<long long>(this->toS() * 1e9));
	}

	operator std::chrono::nanoseconds() const {
		return this->toSystemDelay();
	}

private:
	using Unit<0, 0, 1, false>::Unit;
};

/**
 * Permet de créer une durée à partir d'une valeur littérale en nanosecondes : 1_ns, 2_ns, 0.5_ns …
 */
inline constexpr Duree operator"" _ns(long double duration) { return Duree::makeFromNs(duration); }
inline constexpr Duree operator"" _ns(unsigned long long duration) { return Duree::makeFromNs(duration); }

/**
 * Permet de créer une durée à partir d'une valeur littérale en microsecondes : 1_us, 2_us, 0.5_us …
 */
inline constexpr Duree operator"" _us(long double duration) { return Duree::makeFromUs(duration); }
inline constexpr Duree operator"" _us(unsigned long long duration) { return Duree::makeFromUs(duration); }

/**
 * Permet de créer une durée à partir d'une valeur littérale en millisecondes : 1_ms, 2_ms, 0.5_ms …
 */
inline constexpr Duree operator"" _ms(long double duration) { return Duree::makeFromMs(duration); }
inline constexpr Duree operator"" _ms(unsigned long long duration) { return Duree::makeFromMs(duration); }

/**
 * Permet de créer une durée à partir d'une valeur littérale en secondes : 1_s, 2_s, 0.5_s …
 */
inline constexpr Duree operator"" _s(long double duration) { return Duree::makeFromS(duration); }
inline constexpr Duree operator"" _s(unsigned long long duration) { return Duree::makeFromS(duration); }

/**
 * Permet de créer une durée à partir d'une valeur littérale en minutes : 1_min, 2_min, 0.5_min …
 */
inline constexpr Duree operator"" _min(long double duration) { return Duree::makeFromS(duration * 60); }
inline constexpr Duree operator"" _min(unsigned long long duration) { return Duree::makeFromS(duration * 60); }

/**
 * Permet de créer une durée à partir d'une valeur littérale en heures : 1_h, 2_h, 0.5_h …
 */
inline constexpr Duree operator"" _h(long double duration) { return Duree::makeFromS(duration * 3600); }
inline constexpr Duree operator"" _h(unsigned long long duration) { return Duree::makeFromS(duration * 3600); }

/**
 * Permet de considérer une Durée comme un nombre
 */
namespace std {
	template<>
	struct is_scalar<Duree> : public std::integral_constant<bool, true> {};
}

#endif
