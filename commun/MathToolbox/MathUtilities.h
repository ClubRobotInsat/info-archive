// MathUtilities.h

#ifndef MATH_UTILITIES_H
#define MATH_UTILITIES_H

#include <cmath>
#include <iostream>
#include <limits>
#include "../log/Log.h"

#ifndef M_PI
#define M_E 2.7182818284590452354
#define M_LOG2E 1.4426950408889634074
#define M_LOG10E 0.43429448190325182765
#define M_LN2 0.69314718055994530942
#define M_LN10 2.30258509299404568402
#define M_PI 3.14159265358979323846
#define M_PI_2 1.57079632679489661923
#define M_PI_4 0.78539816339744830962
#define M_1_PI 0.31830988618379067154
#define M_2_PI 0.63661977236758134308
#define M_2_SQRTPI 1.12837916709551257390
#define M_SQRT2 1.41421356237309504880
#define M_SQRT1_2 0.70710678118654752440
#endif

// EPSILON sert lors de la comparaison approchée entre des flottants
#define EPSILON 0.01

template<typename T>
T minNonNul(T a, T b) {
	return max(min(a, b), 0);
}

template<typename T>
bool estDansIntervalOuvert(T val, T bMin, T bMax) {
	return (val > bMin) && (val < bMax);
}

/*
 * Les grandeurs physiques suivantes comportent toutes les fonctionnalités suivantes :
 *  Constructeur de copie
 *	operator+=
 *	operator+
 *	operator-=
 *	operator-
 *	operator*=
 *	operator*
 *	operator/=
 *	operator/
 *	operator%=
 *	operator%
 *  operator<
 *  operator<=
 *  operator>
 *  operator>=
 *  operator==
 *  operator!=
 */

/*
 * Angles :
 *
 * La classe angleRad (voir ci-dessous) encapsule un angle en radians. On peut y faire toutes les opérations usuelles
 * (+, -, *, /, %).
 *
 * On ne peut pas directement créer un objet angleRad avec son une valeur numérique, de manière à éviter les erreurs d'unités.
 * On peut par contre en créer un en ajoutant le suffixe _rad à une valeur numérique :
 *		3_rad est un angleRad de valeur 3
 *		45.654_rad est un angleRad de valeur 45.654 etc.
 *      45_deg + 0.25_PI -> valeur π / 2
 *
 * On peut aussi créer un angleRad en ajoutant le suffixe _deg à une valeur numérique.
 * Dans ce cas, la conversion degrés -> radians est faite pour nous :
 *		180_deg est un angleRad de valeur π/2
 *		-45_deg est un angleRad de valeur -π/4
 *		36.55 est angleRad de valeur (36.55 / 180 * π)
 *
 * Troisième manière d'en créer : ajouter le suffie _PI à une valeur numérique
 *		2_PI : angleRad de valeur 2 * π
 *		-0.75_PI : explicite non ?
 *
 * Dernière façon de créer un angleRad : appeler une des fonctions static angleRad::makeFromRad ou angleRad::makeFromDeg.
 * La première interprète la valeur du paramètre en radians, la 2e va le convertir de degrés vers radians.
 * *
 * class angleRad {
 *	angleRad(angleRad const &);
 *	static angleRad makeFromRad(nombre n);
 *	static angleRad makeFromDeg(nombre n);
 *  angleRad angleMod2Pi(); -> renvoie un angle compris dans [0, 2π[
 *  angleRad toMoinsPiPi(); -> renvoie un angle compris dans [-π, π[
 *  nombre toDeg(); -> valeur en degrés de l'angle
 *};
 *
 */

/*
 * Distances :
 *
 * La classe distanceM (voir ci-dessous) encapsule une distance en mètres
 *
 * On ne peut pas directement créer un objet angleRad avec une valeur numérique, de manière à éviter les erreurs d'unités.
 * On peut par contre en créer un en ajoutant un des suffixes _m, _cm ou _mm à une valeur numérique :
 *		3_m est une distanceM de valeur 3000 mm
 *		3.456_cm est une distanceM de valeur 34.56 mm
 *		4.567_mm est une distanceM de valeur 4.567 mm
 *      3_m + 5_cm -> 3050 mm
 *
 * Dernière façon de créer une distanceM : appeler une des fonctions static distanceM::makeFromMm, distanceM::makeFromCm, distanceM::makeFromDm ou distanceM::makeFromM.
 * La 1ère interprète le paramètre en millimètres, la 2e en centimètres, la 3e en décimètres et la 4e en mètres.
 *
 *
 * class distanceM {
 *	distanceM(distanceM const &);
 *	static distanceM makeFromMm(nombre n);
 *	static distanceM makeFromCm(nombre n);
 *	static distanceM makeFromDm(nombre n);
 *	static distanceM makeFromM(nombre n);
 *  angleRad atan2(distanceM y, distanceM x);
 *};
 *
 */

/*
 * Masses :
 * Même fonctionnalités que les classes ci-dessus : masseKg gère les suffixe _g et _kg.
 */

/*
 * Durées :
 * Même fonctionnalités que les classes ci-dessus : dureeS gère les suffixe _s, _ms, _us et _ns.
 */

/*
 * Vitesse linéaire :
 * Même fonctionnalités que les classes ci-dessus : vitesseM_s gère les suffixe m_s, _dm_s, _cm_s et _mm_s.
 */

/*
 * Vitesse angulaire :
 * Même fonctionnalités que les classes ci-dessus : vitesseRad_s gère les suffixe rad_s et _deg_s.
 */

// g++ même dans la version 4.9 ne se conforme pas au standard C++14 pour l'utilisation de fonctions constepxr
#if __clang__
#define CONDITIONAL_CONSTEXPR constexpr
#else
#define CONDITIONAL_CONSTEXPR
#endif

template<typename T, typename ValType>
class numericValue {
public:
	using ValueType = ValType;

	constexpr numericValue() : _val(ValType()) {}
	numericValue(T const &v) : _val(v._val) {}
	static constexpr T makeValue(ValueType v) {
		return T(v);
	}

	constexpr friend void swap(T &v1, T const &v2) {
		using std::swap;
		swap(v1._val, v2._val);
	}

	CONDITIONAL_CONSTEXPR T &operator=(T v) {
		std::swap(static_cast<T &>(*this), v);
		return static_cast<T &>(*this);
	}

	constexpr T operator-() const {
		return makeValue(-_val);
	}

	constexpr T operator+() const {
		return makeValue(_val);
	}

	CONDITIONAL_CONSTEXPR T &operator+=(T const &val) {
		_val += val._val;
		return static_cast<T &>(*this);
	}
	constexpr friend T operator+(T const &v1, T const &v2) {
		return makeValue(v1._val + v2._val);
	}

	CONDITIONAL_CONSTEXPR T &operator-=(T const &val) {
		_val -= val._val;
		return static_cast<T &>(*this);
	}
	constexpr friend T operator-(T const &v1, T const &v2) {
		return makeValue(v1._val - v2._val);
	}

	template<typename U>
	CONDITIONAL_CONSTEXPR std::enable_if_t<std::is_arithmetic<U>::value, T &>
	operator*=(U val) {
		_val *= val;
		return static_cast<T &>(*this);
	}
	template<typename U>
	constexpr friend std::enable_if_t<std::is_arithmetic<U>::value, T>
	operator*(T const &v1, U val) {
		return makeValue(v1._val * val);
	}

	template<typename U>
	CONDITIONAL_CONSTEXPR std::enable_if_t<std::is_arithmetic<U>::value, T &>
	operator/=(U val) {
		_val /= val;
		return static_cast<T &>(*this);
	}
	template<typename U>
	constexpr friend std::enable_if_t<std::is_arithmetic<U>::value, T>
	operator/(T const &v1, U val) {
		return makeValue(v1._val / val);
	}

	constexpr friend ValueType operator/(T const &v1, T const &v2) {
		return v1._val / v2._val;
	}

	CONDITIONAL_CONSTEXPR T &operator%=(T val) {
		_val = std::fmod(_val, val._val);
		return static_cast<T &>(*this);
	}

	constexpr friend T operator%(T const & v1, T val) {
		return makeValue(std::fmod(v1._val, val._val));
	}

	constexpr friend bool operator==(T const &val1, T const &val2) {
		return val1._val == val2._val;
	}

	constexpr friend bool operator<(T const &val1, T const &val2) {
		return val1._val < val2._val;
	}

	constexpr friend bool operator!=(T const &val1, T const &val2) {
		return !(val1 == val2);
	}

	constexpr friend bool operator>(T const &val1, T const &val2) {
		return val2._val < val1._val;
	}

	constexpr friend bool operator<=(T const &val1, T const &val2) {
		return !(val1._val > val2._val);
	}

	constexpr friend bool operator>=(T const &val1, T const &val2) {
		return !(val1._val < val2._val);
	}

	constexpr friend T abs(T const &t) {
		return t >= T() ? t : -t;
	}

	constexpr friend ValueType cos(T const &t) {
		return std::cos(t._val);
	}
	constexpr friend ValueType sin(T const &t) {
		return std::sin(t._val);
	}

protected:
	template<typename U = ValueType>
	CONDITIONAL_CONSTEXPR U value() const {
		if(_val > std::numeric_limits<U>::max() || _val < std::numeric_limits<U>::lowest()) {
			logError("<ACHTUNG !>");
			logError("Le type vers lequel est convertie la grandeur physique ne peut pas représenter la valeur actuelle de la grandeur !");
			logError("C'est grave ! FIXME !");
			logError("</ACHTUNG !>");
		}
		return static_cast<U>(_val);
	}

protected:
	constexpr numericValue(ValueType val) : _val(val) {}

	ValueType _val;
};

// Voir doc en haut du fichier
class angleRad : public numericValue<angleRad, long double> {
	friend class numericValue<angleRad, long double>;
public:
	using numericValue::ValueType;

	friend std::ostream &operator<<(std::ostream &s, angleRad const &v) {
		return s << v._val << " rad";
	}

	static inline constexpr angleRad makeFromRad(long double rad) { return angleRad(rad); }
	static inline constexpr angleRad makeFromMilliRad(long double millirad) { return angleRad(millirad / 1000); }
	static inline constexpr angleRad makeFromDeg(long double deg) {
		return angleRad(deg / 180 * M_PI);
	}

	template<typename Rep = ValueType>
	constexpr Rep toRad() const {
		return (*this).value<Rep>();
	}

	template<typename Rep = ValueType>
	constexpr Rep toDeg() const {
		return (*this * 180 / M_PI).value<Rep>();
	}

	template<typename Rep = ValueType>
	constexpr Rep toMilliRad() const {
		return (*this * 1000).value<Rep>();
	}

	constexpr angleRad angleMod2Pi() const {
		return ((*this % angleRad::makeFromRad(2 * M_PI)) + angleRad::makeFromRad(4 * M_PI)) % angleRad::makeFromRad(2 * M_PI);
	}

	CONDITIONAL_CONSTEXPR angleRad toMoinsPiPi() const {
		auto mod2Pi = this->angleMod2Pi();

		if(mod2Pi >= angleRad::makeFromRad(M_PI))
			mod2Pi -= angleRad::makeFromRad(2 * M_PI);
		if(mod2Pi < angleRad::makeFromRad(-M_PI))
			mod2Pi += angleRad::makeFromRad(2 * M_PI);

		return mod2Pi;
	}

private:
	using numericValue::numericValue;
};

// Voir doc en haut du fichier
class distanceM : public numericValue<distanceM, long double> {
	friend class numericValue<distanceM, long double>;
public:
	using numericValue::ValueType;

	friend std::ostream &operator<<(std::ostream &s, distanceM const &d) {
		if(d._val >= 1)
			s << d._val << " m";
		else if(d._val >= 1e-2)
			s << d._val * 1e2 << " cm";
		else
			s << d._val * 1e3 << " mm";

		return s;
	}

	template<typename Rep = ValueType>
	constexpr Rep toMm() const {
		return (*this * 1000).value<Rep>();
	}

	template<typename Rep = ValueType>
	constexpr Rep toM() const {
		return (*this).value<Rep>();
	}

	template<typename Rep = ValueType>
	constexpr Rep toDm() const {
		return (*this * 10).value<Rep>();
	}

	friend angleRad atan2(distanceM const &y, distanceM const &x);

	static constexpr distanceM makeFromMm(long double mm) { return distanceM(mm / 1000); }
	static constexpr distanceM makeFromCm(long double cm) { return distanceM(cm / 100); }
	static constexpr distanceM makeFromDm(long double dm) { return distanceM(dm / 10); }
	static constexpr distanceM makeFromM(long double m) { return distanceM(m); }

private:
	using numericValue::numericValue;
};

inline angleRad atan2(distanceM const &y, distanceM const &x) {
	return angleRad::makeFromRad(std::atan2(y.value(), x.value()));
}

// Voir doc en haut du fichier
class distanceM2 : public numericValue<distanceM2, long double> {
	friend class numericValue<distanceM2, long double>;
public:
	using numericValue::ValueType;

	friend std::ostream &operator<<(std::ostream &stream, distanceM2 const &v) {
		return stream << v._val << " m2";
	}

	template<typename Rep = ValueType>
	constexpr Rep toM2() const {
		return (*this).value<Rep>();
	}

	template<typename Rep = ValueType>
	constexpr Rep toMm2() const {
		return (*this * 1000000).value<Rep>();
	}

	friend distanceM sqrt(distanceM2 const &d);

	static constexpr distanceM2 makeFromMm2(long double mm2) { return distanceM2(mm2 / 1000000); }
	static constexpr distanceM2 makeFromCm2(long double cm2) { return distanceM2(cm2 / 10000); }
	static constexpr distanceM2 makeFromDm2(long double dm2) { return distanceM2(dm2 / 100); }
	static constexpr distanceM2 makeFromM2(long double m2) { return distanceM2(m2); }

private:
	using numericValue::numericValue;
};

// Voir doc en haut du fichier
class masseKg : public numericValue<masseKg, long double> {
	friend class numericValue<masseKg, long double>;
public:
	using numericValue::ValueType;

	friend std::ostream &operator<<(std::ostream &stream, masseKg const &v) {
		return stream << v._val << " kg";
	}

	template<typename Rep = ValueType>
	constexpr Rep toKg() const {
		return (*this).value<Rep>();
	}

	static constexpr masseKg makeFromKg(long double kg) { return masseKg(kg); }
	static constexpr masseKg makeFromG(long double g) { return masseKg(g / 1000); }

private:
	using numericValue::numericValue;
};

// Voir doc en haut du fichier
class dureeS : public numericValue<dureeS, long double> {
	friend class numericValue<dureeS, long double>;
public:
	using numericValue::ValueType;

	friend inline std::ostream &operator<<(std::ostream &s, dureeS const &d) {
		if(d._val >= 1)
			s << d._val << " s";
		else if(d._val >= 1e-3)
			s << d._val * 1e3 << " ms";
		else if(d._val >= 1e-6)
			s << d._val * 1e6 << " us";
		else
			s << d._val * 1e9 << " ns";

		return s;
	}

	template<typename Rep = ValueType>
	constexpr Rep toS() const {
		return (*this).value<Rep>();
	}

	static constexpr dureeS makeFromNs(long double ns) { return dureeS(ns / 1e9); }
	static constexpr dureeS makeFromUs(long double us) { return dureeS(us / 1e6); }
	static constexpr dureeS makeFromMs(long double ms) { return dureeS(ms / 1e3); }
	static constexpr dureeS makeFromS(long double s) { return dureeS(s); }

private:
	using numericValue::numericValue;
};

// Voir doc en haut du fichier
class vitesseM_s : public numericValue<vitesseM_s, long double> {
	friend class numericValue<vitesseM_s, long double>;
public:
	using numericValue::ValueType;

	friend inline std::ostream &operator<<(std::ostream &s, vitesseM_s const &v) {
		if(v._val >= 1)
			s << v._val << " m/s";
		else if(v._val >= 1e-2)
			s << v._val * 1e2 << " cm/s";
		else
			s << v._val * 1e3 << " mm/s";

		return s;
	}

	template<typename Rep = ValueType>
	constexpr Rep toMm_s() const {
		return (*this * 1000).value<Rep>();
	}

	template<typename Rep = ValueType>
	constexpr Rep toM_s() const {
		return (*this).value<Rep>();
	}

	template<typename Rep = ValueType>
	constexpr Rep toDm_s() const {
		return (*this * 10).value<Rep>();
	}

	static constexpr vitesseM_s makeFromM_s(long double m_s) { return vitesseM_s(m_s); }
	static constexpr vitesseM_s makeFromDm_s(long double dm_s) { return vitesseM_s(dm_s / 10); }
	static constexpr vitesseM_s makeFromCm_s(long double cm_s) { return vitesseM_s(cm_s / 100); }
	static constexpr vitesseM_s makeFromMm_s(long double mm_s) { return vitesseM_s(mm_s / 1000); }

private:
	using numericValue::numericValue;
};

// Voir doc en haut du fichier
class vitesseRad_s : public numericValue<vitesseRad_s, long double> {
	friend class numericValue<vitesseRad_s, long double>;
public:
	using numericValue::ValueType;

	friend inline std::ostream &operator<<(std::ostream &s, vitesseRad_s const &v) {
		return s << v._val << " rad/s";

		return s;
	}

	template<typename Rep = ValueType>
	constexpr Rep toRad_s() const {
		return (*this).value<Rep>();
	}

	template<typename Rep = ValueType>
	constexpr Rep toMilliRad_s() const {
		return (*this * 1000).value<Rep>();
	}

	static constexpr vitesseRad_s makeFromRad_s(long double rad_s) { return vitesseRad_s(rad_s); }
	static constexpr vitesseRad_s makeFromDeg_s(long double deg_s) { return vitesseRad_s(deg_s * M_PI / 180); }
	static constexpr vitesseRad_s makeFromMilliRad_s(long double millirad_s) { return vitesseRad_s(millirad_s / 1000); }

private:
	using numericValue::numericValue;
};

template <typename T, typename Scalar>
inline std::enable_if_t<std::is_base_of<numericValue<T, typename T::ValueType>, T>::value && std::is_scalar<Scalar>::value, T>
operator*(Scalar s, T const &num) {
	return num * s;
}

namespace std {
	template<>
	struct is_scalar<distanceM> : public std::integral_constant<bool, true> {};
	template<>
	struct is_scalar<angleRad> : public std::integral_constant<bool, true> {};
	template<>
	struct is_scalar<masseKg> : public std::integral_constant<bool, true> {};
	template<>
	struct is_scalar<dureeS> : public std::integral_constant<bool, true> {};
	template<>
	struct is_scalar<vitesseM_s> : public std::integral_constant<bool, true> {};
	template<>
	struct is_scalar<vitesseRad_s> : public std::integral_constant<bool, true> {};
}

inline vitesseM_s operator/(distanceM const &d, dureeS const &t) {
	return vitesseM_s::makeFromM_s(d.toM() / t.toS());
}

inline dureeS operator/(distanceM const &d, vitesseM_s const &v) {
	return dureeS::makeFromS(d.toM() / v.toM_s());
}

inline distanceM operator*(vitesseM_s const &v, dureeS const &t) {
	return distanceM::makeFromM(v.toM_s() * t.toS());
}

inline distanceM operator*(dureeS const &t, vitesseM_s const &v) {
	return distanceM::makeFromM(v.toM_s() * t.toS());
}

inline vitesseRad_s operator/(angleRad const &a, dureeS const &t) {
	return vitesseRad_s::makeFromRad_s(a.toRad() / t.toS());
}

inline dureeS operator/(angleRad const &a, vitesseRad_s const &v) {
	return dureeS::makeFromS(a.toRad() / v.toRad_s());
}

inline angleRad operator*(vitesseRad_s const &a, dureeS const &t) {
	return angleRad::makeFromRad(a.toRad_s() * t.toS());
}

inline angleRad operator*(dureeS const &t, vitesseRad_s const &a) {
	return angleRad::makeFromRad(a.toRad_s() * t.toS());
}

inline distanceM sqrt(distanceM2 const &d) {
	return distanceM::makeFromM(std::sqrt(d.toM2()));
}

inline distanceM2 operator*(distanceM const &d1, distanceM const &d2) {
	return distanceM2::makeFromM2(d1.toM() * d2.toM());
}

inline constexpr angleRad operator"" _PI(long double val) {
	return angleRad::makeFromRad(val * M_PI);
}

inline constexpr angleRad operator"" _PI(unsigned long long val) {
	return angleRad::makeFromRad(val * M_PI);
}

inline constexpr angleRad operator"" _deg(long double deg) {
	return angleRad::makeFromDeg(deg);
}

inline constexpr angleRad operator"" _deg(unsigned long long deg) {
	return angleRad::makeFromDeg(deg);
}

inline constexpr angleRad operator"" _rad(long double rad) {
	return angleRad::makeFromRad(rad);
}

inline constexpr angleRad operator"" _rad(unsigned long long rad) {
	return angleRad::makeFromRad(rad);
}

inline constexpr distanceM operator"" _mm(long double dist) {
	return distanceM::makeFromMm(dist);
}

inline constexpr distanceM operator"" _mm(unsigned long long dist) {
	return distanceM::makeFromMm(dist);
}

inline constexpr distanceM operator"" _cm(long double dist) {
	return distanceM::makeFromCm(dist);
}

inline constexpr distanceM operator"" _cm(unsigned long long dist) {
	return distanceM::makeFromCm(dist);
}

inline constexpr distanceM operator"" _dm(long double dist) {
	return distanceM::makeFromDm(dist);
}

inline constexpr distanceM operator"" _dm(unsigned long long dist) {
	return distanceM::makeFromDm(dist);
}

inline constexpr distanceM operator"" _m(long double dist) {
	return distanceM::makeFromM(dist);
}

inline constexpr distanceM operator"" _m(unsigned long long dist) {
	return distanceM::makeFromM(dist);
}

inline constexpr distanceM2 operator"" _mm2(long double dist) {
	return distanceM2::makeFromMm2(dist);
}

inline constexpr distanceM2 operator"" _mm2(unsigned long long dist) {
	return distanceM2::makeFromMm2(dist);
}

inline constexpr masseKg operator"" _g(long double g) {
	return masseKg::makeFromG(g);
}

inline constexpr masseKg operator"" _g(unsigned long long g) {
	return masseKg::makeFromG(g);
}

inline constexpr masseKg operator"" _kg(long double kg) {
	return masseKg::makeFromKg(kg);
}

inline constexpr masseKg operator"" _kg(unsigned long long kg) {
	return masseKg::makeFromKg(kg);
}

/*
 * On définit ci-dessous les suffixes _ns, _us, _ms et _s pour créer des nano, micro, milli secondes et secondes.
 */

inline constexpr dureeS operator"" _ns(long double duration) {
	return dureeS::makeFromNs(duration);
}

inline constexpr dureeS operator"" _ns(unsigned long long duration) {
	return dureeS::makeFromNs(duration);
}

inline constexpr dureeS operator"" _us(long double duration) {
	return dureeS::makeFromUs(duration);
}

inline constexpr dureeS operator"" _us(unsigned long long duration) {
	return dureeS::makeFromUs(duration);
}

inline constexpr dureeS operator"" _ms(long double duration) {
	return dureeS::makeFromMs(duration);
}

inline constexpr dureeS operator"" _ms(unsigned long long duration) {
	return dureeS::makeFromMs(duration);
}

inline constexpr dureeS operator"" _s(long double duration) {
	return dureeS::makeFromS(duration);
}

inline constexpr dureeS operator"" _s(unsigned long long duration) {
	return dureeS::makeFromS(duration);
}

inline constexpr vitesseM_s operator"" _mm_s(long double v) {
	return vitesseM_s::makeFromMm_s(v);
}

inline constexpr vitesseM_s operator"" _mm_s(unsigned long long v) {
	return vitesseM_s::makeFromMm_s(v);
}

inline constexpr vitesseM_s operator"" _cm_s(long double v) {
	return vitesseM_s::makeFromCm_s(v);
}

inline constexpr vitesseM_s operator"" _cm_s(unsigned long long v) {
	return vitesseM_s::makeFromCm_s(v);
}

inline constexpr vitesseM_s operator"" _dm_s(long double v) {
	return vitesseM_s::makeFromDm_s(v);
}

inline constexpr vitesseM_s operator"" _dm_s(unsigned long long v) {
	return vitesseM_s::makeFromDm_s(v);
}

inline constexpr vitesseM_s operator"" _m_s(long double v) {
	return vitesseM_s::makeFromM_s(v);
}

inline constexpr vitesseM_s operator"" _m_s(unsigned long long v) {
	return vitesseM_s::makeFromM_s(v);
}

inline constexpr vitesseRad_s operator"" _rad_s(long double v) {
	return vitesseRad_s::makeFromRad_s(v);
}

inline constexpr vitesseRad_s operator"" _rad_s(unsigned long long v) {
	return vitesseRad_s::makeFromRad_s(v);
}

inline constexpr vitesseRad_s operator"" _deg_s(long double v) {
	return vitesseRad_s::makeFromDeg_s(v);
}

inline constexpr vitesseRad_s operator"" _deg_s(unsigned long long v) {
	return vitesseRad_s::makeFromDeg_s(v);
}

#endif

