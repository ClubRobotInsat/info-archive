// MathUtilities.h

#ifndef MATH_UTILITIES_H
#define MATH_UTILITIES_H

#include <cmath>
#include <iostream>
#include <limits>
#include "Vector2.h"
#include "Vector3.h"
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
 * La classe distanceMm (voir ci-dessous) encapsule une distance en millimètres
 *
 * On ne peut pas directement créer un objet angleRad avec une valeur numérique, de manière à éviter les erreurs d'unités.
 * On peut par contre en créer un en ajoutant un des suffixes _m, _cm ou _mm à une valeur numérique :
 *		3_m est une distanceMm de valeur 3000 mm
 *		3.456_cm est une distanceMm de valeur 34.56 mm
 *		4.567_mm est une distanceMm de valeur 4.567 mm
 *      3_m + 5_cm -> 3050 mm
 *
 * Dernière façon de créer une distanceMm : appeler une des fonctions static distanceMm::makeFromMm, distanceMm::makeFromCm, distanceMm::makeFromDm ou distanceMm::makeFromM.
 * La 1ère interprète le paramètre en millimètres, la 2e en centimètres, la 3e en décimètres et la 4e en mètres.
 *
 *
 * class distanceMm {
 *	distanceMm(distanceMm const &);
 *	static distanceMm makeFromMm(nombre n);
 *	static distanceMm makeFromCm(nombre n);
 *	static distanceMm makeFromDm(nombre n);
 *	static distanceMm makeFromM(nombre n);
 *  angleRad atan2(distanceMm y, distanceMm x);
 *};
 *
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

	template<typename U = ValueType>
	constexpr U value() const {
		if(_val > std::numeric_limits<U>::max() || _val < std::numeric_limits<U>::lowest()) {
			logError("<ACHTUNG !>");
			logError("Le type vers lequel est convertie la grandeur physique ne peut pas représenter la valeur actuelle de la grandeur !");
			logError("C'est grave ! FIXME !");
			logError("</ACHTUNG !>");
		}
		return static_cast<U>(_val);
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
		return T(-_val);
	}

	constexpr T operator+() const {
		return T(_val);
	}

	CONDITIONAL_CONSTEXPR T &operator+=(T const &val) {
		_val += val._val;
		return static_cast<T &>(*this);
	}
	constexpr friend T operator+(T const &v1, T const &v2) {
		return T(v1._val + v2._val);
	}

	CONDITIONAL_CONSTEXPR T &operator-=(T const &val) {
		_val -= val._val;
		return static_cast<T &>(*this);
	}
	constexpr friend T operator-(T const &v1, T const &v2) {
		return T(v1._val - v2._val);
	}

	template<typename U>
	CONDITIONAL_CONSTEXPR typename std::enable_if<std::is_arithmetic<U>::value, T &>::type
	operator*=(U val) {
		_val *= val;
		return static_cast<T &>(*this);
	}
	template<typename U>
	constexpr friend typename std::enable_if<std::is_arithmetic<U>::value, T>::type
	operator*(T const &v1, U val) {
		return T(v1._val * val);
	}

	template<typename U>
	CONDITIONAL_CONSTEXPR typename std::enable_if<std::is_arithmetic<U>::value, T &>::type
	operator/=(U val) {
		_val /= val;
		return static_cast<T &>(*this);
	}
	template<typename U>
	constexpr friend typename std::enable_if<std::is_arithmetic<U>::value, T>::type
	operator/(T const &v1, U val) {
		return T(v1._val / val);
	}

	constexpr friend ValueType operator/(T const &v1, T const &v2) {
		return v1._val / v2._val;
	}

	CONDITIONAL_CONSTEXPR T &operator%=(T val) {
		_val = std::fmod(_val, val._val);
		return static_cast<T &>(*this);
	}

	constexpr friend T operator%(T const & v1, T val) {
		return T(std::fmod(v1._val, val._val));
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

	// g++ jusqu'à la version 4.9 semble avoir un bug, qui fait qu'une classe friend ne peut pas accéder aux champs protected...
#ifdef __clang__
protected:
#endif
	constexpr numericValue(ValueType val) : _val(val) {}
protected:
	ValueType _val;
};

template <typename T, typename Scalar>
inline typename std::enable_if<std::is_base_of<numericValue<T, typename T::ValueType>, T>::value && std::is_scalar<Scalar>::value, T>::type
operator*(Scalar s, T const &num) {
	return num * s;
}

class angleRad : public numericValue<angleRad, long double> {
	friend class numericValue<angleRad, long double>;
public:
	using numericValue::ValueType;

	friend std::ostream &operator<<(std::ostream &stream, angleRad const &v) {
		return stream << v._val << " rad";
	}

	static inline constexpr angleRad makeFromRad(long double rad) { return angleRad(rad); }
	static inline constexpr angleRad makeFromMilliRad(long double millirad) { return angleRad(millirad * 1000); }
	static inline constexpr angleRad makeFromDeg(long double deg) {
		return angleRad(deg / 180 * M_PI);
	}

	constexpr ValueType toDeg();

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

template<>
template<>
constexpr
angleRad::ValueType numericValue<angleRad, angleRad::ValueType>::value<angleRad::ValueType>() const {
	return _val;
}

constexpr angleRad::ValueType angleRad::toDeg() {
	return this->value() * 180 / M_PI;
}

class distanceMm : public numericValue<distanceMm, long double> {
	friend class numericValue<distanceMm, long double>;
public:
	using numericValue::ValueType;

	friend std::ostream &operator<<(std::ostream &stream, distanceMm const &v) {
		return stream << v._val << " mm";
	}

	friend angleRad atan2(distanceMm const &y, distanceMm const &x);

	static constexpr distanceMm makeFromMm(long double mm) { return distanceMm(mm); }
	static constexpr distanceMm makeFromCm(long double cm) { return distanceMm(cm * 10); }
	static constexpr distanceMm makeFromDm(long double dm) { return distanceMm(dm * 100); }
	static constexpr distanceMm makeFromM(long double m) { return distanceMm(m * 1000); }

private:
	using numericValue::numericValue;
};

template<>
template<>
constexpr
inline distanceMm::ValueType numericValue<distanceMm, distanceMm::ValueType>::value<distanceMm::ValueType>() const {
	return _val;
}

inline angleRad atan2(distanceMm const &y, distanceMm const &x) {
	return angleRad::makeFromRad(std::atan2(y.value(), x.value()));
}

class distanceMm2 : public numericValue<distanceMm2, long double> {
	friend class numericValue<distanceMm2, long double>;
public:
	using numericValue::ValueType;

	friend std::ostream &operator<<(std::ostream &stream, distanceMm2 const &v) {
		return stream << v._val << " mm2";
	}

	friend distanceMm sqrt(distanceMm2 const &d);

	static constexpr distanceMm2 makeFromMm2(long double mm2) { return distanceMm2(mm2); }
	static constexpr distanceMm2 makeFromCm2(long double cm2) { return distanceMm2(cm2 * 100); }
	static constexpr distanceMm2 makeFromDm2(long double dm2) { return distanceMm2(dm2 * 10000); }
	static constexpr distanceMm2 makeFromM2(long double m2) { return distanceMm2(m2 * 1000000); }

private:
	using numericValue::numericValue;
};

template<>
template<>
inline constexpr
distanceMm2::ValueType numericValue<distanceMm2, distanceMm2::ValueType>::value<distanceMm2::ValueType>() const {
	return _val;
}

inline distanceMm sqrt(distanceMm2 const &d) {
	return distanceMm::makeFromMm(std::sqrt(d.value()));
}

inline distanceMm2 operator*(distanceMm const &d1, distanceMm const &d2) {
	return distanceMm2::makeFromMm2(d1.value() * d2.value());
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

inline constexpr distanceMm operator"" _mm(long double dist) {
	return distanceMm::makeFromMm(dist);
}

inline constexpr distanceMm operator"" _mm(unsigned long long dist) {
	return distanceMm::makeFromMm(dist);
}

inline constexpr distanceMm operator"" _cm(long double dist) {
	return distanceMm::makeFromCm(dist);
}

inline constexpr distanceMm operator"" _cm(unsigned long long dist) {
	return distanceMm::makeFromCm(dist);
}

inline constexpr distanceMm operator"" _dm(long double dist) {
	return distanceMm::makeFromDm(dist);
}

inline constexpr distanceMm operator"" _dm(unsigned long long dist) {
	return distanceMm::makeFromDm(dist);
}

inline constexpr distanceMm operator"" _m(long double dist) {
	return distanceMm::makeFromM(dist);
}

inline constexpr distanceMm operator"" _m(unsigned long long dist) {
	return distanceMm::makeFromM(dist);
}

inline constexpr distanceMm2 operator"" _mm2(long double dist) {
	return distanceMm2::makeFromMm2(dist);
}

inline constexpr distanceMm2 operator"" _mm2(unsigned long long dist) {
	return distanceMm2::makeFromMm2(dist);
}

#endif

