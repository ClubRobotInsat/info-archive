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

/*
 * Masses :
 * La même que celles au-dessus, gère les suffixe _g et _kg.
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
	CONDITIONAL_CONSTEXPR std::enable_if_t<std::is_arithmetic<U>::value, T &>
	operator*=(U val) {
		_val *= val;
		return static_cast<T &>(*this);
	}
	template<typename U>
	constexpr friend std::enable_if_t<std::is_arithmetic<U>::value, T>
	operator*(T const &v1, U val) {
		return T(v1._val * val);
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

	ValueType _val;
};

template <typename T, typename Scalar>
inline std::enable_if_t<std::is_base_of<numericValue<T, typename T::ValueType>, T>::value && std::is_scalar<Scalar>::value, T>
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

class distanceMm : public numericValue<distanceMm, long double> {
	friend class numericValue<distanceMm, long double>;
public:
	using numericValue::ValueType;

	friend std::ostream &operator<<(std::ostream &stream, distanceMm const &v) {
		return stream << v._val << " mm";
	}

	template<typename Rep = ValueType>
	constexpr Rep toMm() const {
		return (*this).value<Rep>();
	}

	template<typename Rep = ValueType>
	constexpr Rep toM() const {
		return (*this / 1000).value<Rep>();
	}

	template<typename Rep = ValueType>
	constexpr Rep toDm() const {
		return (*this / 100).value<Rep>();
	}

	friend angleRad atan2(distanceMm const &y, distanceMm const &x);

	static constexpr distanceMm makeFromMm(long double mm) { return distanceMm(mm); }
	static constexpr distanceMm makeFromCm(long double cm) { return distanceMm(cm * 10); }
	static constexpr distanceMm makeFromDm(long double dm) { return distanceMm(dm * 100); }
	static constexpr distanceMm makeFromM(long double m) { return distanceMm(m * 1000); }

private:
	using numericValue::numericValue;
};

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

	template<typename Rep = ValueType>
	constexpr Rep toM2() const {
		return (*this / 1000000).value<Rep>();
	}

	template<typename Rep = ValueType>
	constexpr Rep toMm2() const {
		return (*this).value<Rep>();
	}

	friend distanceMm sqrt(distanceMm2 const &d);

	static constexpr distanceMm2 makeFromMm2(long double mm2) { return distanceMm2(mm2); }
	static constexpr distanceMm2 makeFromCm2(long double cm2) { return distanceMm2(cm2 * 100); }
	static constexpr distanceMm2 makeFromDm2(long double dm2) { return distanceMm2(dm2 * 10000); }
	static constexpr distanceMm2 makeFromM2(long double m2) { return distanceMm2(m2 * 1000000); }

private:
	using numericValue::numericValue;
};

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

class vitesseMm_s : public numericValue<vitesseMm_s, long double> {
	friend class numericValue<vitesseMm_s, long double>;
public:
	using numericValue::ValueType;

	friend inline std::ostream &operator<<(std::ostream &s, vitesseMm_s const &v) {
		return s << v._val << " mm/s";

		return s;
	}

	template<typename Rep = ValueType>
	constexpr Rep toMm_s() const {
		return (*this).value<Rep>();
	}

	template<typename Rep = ValueType>
	constexpr Rep toM_s() const {
		return (*this / 1000).value<Rep>();
	}

	template<typename Rep = ValueType>
	constexpr Rep toDm_s() const {
		return (*this / 100).value<Rep>();
	}

	static constexpr vitesseMm_s makeFromM_s(long double m_s) { return vitesseMm_s(m_s * 1000); }
	static constexpr vitesseMm_s makeFromDm_s(long double dm_s) { return vitesseMm_s(dm_s * 100); }
	static constexpr vitesseMm_s makeFromCm_s(long double cm_s) { return vitesseMm_s(cm_s * 10); }
	static constexpr vitesseMm_s makeFromMm_s(long double mm_s) { return vitesseMm_s(mm_s); }

private:
	using numericValue::numericValue;
};

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

inline vitesseMm_s operator/(distanceMm const &d, dureeS const &t) {
	return vitesseMm_s::makeFromM_s(d.toM() / t.toS());
}

inline dureeS operator/(distanceMm const &d, vitesseMm_s const &v) {
	return dureeS::makeFromS(d.toM() / v.toM_s());
}

inline distanceMm operator*(vitesseMm_s const &v, dureeS const &t) {
	return distanceMm::makeFromMm(v.toM_s() * t.toS());
}

inline distanceMm operator*(dureeS const &t, vitesseMm_s const &v) {
	return distanceMm::makeFromMm(v.toM_s() * t.toS());
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

inline distanceMm sqrt(distanceMm2 const &d) {
	return distanceMm::makeFromM(std::sqrt(d.toM2()));
}

inline distanceMm2 operator*(distanceMm const &d1, distanceMm const &d2) {
	return distanceMm2::makeFromM2(d1.toM() * d2.toM());
}

namespace std {
	template<>
	struct is_scalar<distanceMm> : public std::integral_constant<bool, true> {};
	template<>
	struct is_scalar<angleRad> : public std::integral_constant<bool, true> {};
	template<>
	struct is_scalar<masseKg> : public std::integral_constant<bool, true> {};
	template<>
	struct is_scalar<dureeS> : public std::integral_constant<bool, true> {};
	template<>
	struct is_scalar<vitesseMm_s> : public std::integral_constant<bool, true> {};
	template<>
	struct is_scalar<vitesseRad_s> : public std::integral_constant<bool, true> {};
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

inline constexpr vitesseMm_s operator"" _mm_s(long double v) {
	return vitesseMm_s::makeFromMm_s(v);
}

inline constexpr vitesseMm_s operator"" _mm_s(unsigned long long v) {
	return vitesseMm_s::makeFromMm_s(v);
}

inline constexpr vitesseMm_s operator"" _cm_s(long double v) {
	return vitesseMm_s::makeFromCm_s(v);
}

inline constexpr vitesseMm_s operator"" _cm_s(unsigned long long v) {
	return vitesseMm_s::makeFromCm_s(v);
}

inline constexpr vitesseMm_s operator"" _dm_s(long double v) {
	return vitesseMm_s::makeFromDm_s(v);
}

inline constexpr vitesseMm_s operator"" _dm_s(unsigned long long v) {
	return vitesseMm_s::makeFromDm_s(v);
}

inline constexpr vitesseMm_s operator"" _m_s(long double v) {
	return vitesseMm_s::makeFromM_s(v);
}

inline constexpr vitesseMm_s operator"" _m_s(unsigned long long v) {
	return vitesseMm_s::makeFromM_s(v);
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

