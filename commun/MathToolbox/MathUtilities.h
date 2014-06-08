// MathUtilities.h

#ifndef MATH_UTILITIES_H
#define MATH_UTILITIES_H

#include <cmath>
#include <iostream>

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

// Conversions d'angles degrés <-> radians
double DegToRad(double angleDeg);

double RadToDeg(double angleRad);
// ramene un angle dans [-M_PI , M_PI]
double ToMoinsPiPi(double angle);

template<typename T>
T minNonNul(T a, T b) {
	return max(min(a, b), 0);
}

template<typename T>
bool estDansIntervalOuvert(T val, T bMin, T bMax) {
	return (val > bMin) && (val < bMax);
}

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
 * La première interprète la valeur du paramètre en radians, la 3e va le convertir de degrés vers radians.
 *
 * class angleRad {
 *	angleRad(angleRad const &);
 *	static angleRad makeFromRad(nombre n);
 *	static angleRad makeFromDeg(nombre n);
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
 *};
 *
 */

/*
 * Distances :
 *
 * La classe distanceMm (voir ci-dessous) encapsule une distance en millimètres. On peut y faire toutes les opérations usuelles
 * (+, -, *, /, %).
 *
 * On ne peut pas directement créer un objet angleRad avec une valeur numérique, de manière à éviter les erreurs d'unités.
 * On peut par contre en créer un en ajoutant un des suffixes _m, _cm ou _mm à une valeur numérique :
 *		3_m est une distanceMm de valeur 3000 mm
 *		3.456_cm est une distanceMm de valeur 34.56 mm
 *		4.567_mm est une distanceMm de valeur 4.567 mm
 *
 * Dernière façon de créer une distanceMm : appeler une des fonctions static distanceMm::makeFromMm, distanceMm::makeFromCm, distanceMm::makeFromDm ou distanceMm::makeFromM.
 * La 1ère interprète le paramètre en millimètres, la 2e en centimètres, la 3e en décimètres et la 4e en mètres.
 *
 * class distanceMm {
 *	distanceMm(distanceMm const &);
 *	static distanceMm makeFromMm(nombre n);
 *	static distanceMm makeFromCm(nombre n);
 *	static distanceMm makeFromDm(nombre n);
 *	static distanceMm makeFromM(nombre n);
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
 *};
 *
 */

template<typename T, typename ValType>
class numericValue {
public:
	numericValue(T const &v) {
		_val = v._val;
	}

	template<typename U>
	constexpr operator U() const {
		return _val;
	}

	friend void swap(T &v1, T const &v2) {
		using std::swap;
		swap(v1._val, v2._val);
	}

	T &operator=(T v) {
		std::swap(static_cast<T &>(*this), v);
		return static_cast<T &>(*this);
	}

	T &operator+=(T const &val) {
		_val += val._val;
		return static_cast<T &>(*this);
	}
	friend T operator+(T v1, T const &v2) { return v1 += v2; }

	T &operator-=(T const &val) {
		_val -= val._val;
		return static_cast<T &>(*this);
	}
	friend T operator-(T v1, T const &v2) { return v1 -= v2; }

	template<typename U>
	typename std::enable_if<std::is_arithmetic<U>::value, T &>::type
	operator*=(U val) {
		_val *= val;
		return static_cast<T &>(*this);
	}
	template<typename U>
	friend typename std::enable_if<std::is_arithmetic<U>::value, T>::type
	operator*(T v1, U val) { return v1 *= val; }

	template<typename U>
	typename std::enable_if<std::is_arithmetic<U>::value, T &>::type
	operator/=(U val) {
		_val /= val;
		return static_cast<T &>(*this);
	}
	template<typename U>
	friend typename std::enable_if<std::is_arithmetic<U>::value, T>::type
	operator/(T v1, U val) { return v1 /= val; }

	T &operator%=(T const &val) {
		_val = std::fmod(_val, val._val);
		return static_cast<T &>(*this);
	}
	friend T operator%(T v1, T const &v2) { return v1 %= v2; }


protected:
	constexpr numericValue(ValType val) : _val(val) { }
	ValType _val;
};

class angleRad : public numericValue<angleRad, long double> {
public:
	friend std::ostream &operator<<(std::ostream &stream, angleRad const &v) {
		return stream << v._val << " rad";
	}

	static inline constexpr angleRad makeFromRad(long double rad) { return angleRad(rad); }
	static inline constexpr angleRad makeFromDeg(long double deg) {
		return angleRad(deg / 180 * M_PI);
	}

	angleRad angleMod2Pi() const {
		return *this % angleRad::makeFromRad(2 * M_PI);
	}

private:
	using numericValue::numericValue;
};

class distanceMm : public numericValue<distanceMm, long double> {
public:
	friend std::ostream &operator<<(std::ostream &stream, distanceMm const &v) {
		return stream << v._val << " mm";
	}

	static constexpr distanceMm makeFromMm(long double mm) { return distanceMm(mm); }
	static constexpr distanceMm makeFromCm(long double cm) { return distanceMm(cm * 10); }
	static constexpr distanceMm makeFromDm(long double dm) { return distanceMm(dm * 100); }
	static constexpr distanceMm makeFromM(long double m) { return distanceMm(m * 1000); }

private:
	using numericValue::numericValue;
};

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

#endif

