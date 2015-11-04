// Vector3.h
// Vector en 3 dimensions
// 2008 par Funto
// Version 1.0

#ifndef VECTOR_3_H
#define VECTOR_3_H

#include <iostream>
#include "../Units/Unit.h"

template <class T>
class Vector3 {
public:
	typedef T value_type;

	union {
		struct {
			T r, g, b;
		};
		struct {
			T s, t, p;
		};
		struct {
			T x, y, z;
		};
	};

	T &operator[](std::size_t i) {
		return (&x)[i];
	}
	T const &operator[](std::size_t i) const {
		return (&x)[i];
	}

public:
	// Constructeurs
	constexpr Vector3(); // par défaut

	constexpr Vector3(const T &x, const T &y, const T &z); // à partir des coordonnées

	template <class T_scalar>
	Vector3(const T_scalar &s); // à partir d'un seul scalaire

	template <class T_scalar>
	Vector3(T_scalar const *s); // à partir d'un vecteur

	Vector3(const Vector3<T> &ref); // de copie 1

	template <class T_2>
	Vector3(const Vector3<T_2> &ref); // de copie 2

	// Destructeur
	~Vector3() = default;

	// Affectation suivant un autre vecteur
	template <class T_2>
	inline Vector3<T> &operator=(const Vector3<T_2> &ref);

	// Affectation suivant un autre vecteur du même type (sinon, ça compile pas...)
	Vector3<T> &operator=(const Vector3<T> &ref);

	// Affectation suivant un scalaire
	template <class T_scalar>
	inline Vector3<T> &operator=(const T_scalar &s);

	// Affectation-addition
	template <class T_2>
	inline Vector3<T> &operator+=(const Vector3<T_2> &v);

	// Affectation-soustraction
	template <class T_2>
	inline Vector3<T> &operator-=(const Vector3<T_2> &v);

	// Affectation-multiplication par un scalaire
	template <class T_scalar>
	inline Vector3<T> &operator*=(const T_scalar &s);

	// Affectation-division par un scalaire
	template <class T_scalar>
	inline Vector3<T> &operator/=(const T_scalar &s);

	// Négation
	inline Vector3<T> operator-() const;

	// Addition
	template <class T_2>
	inline Vector3<T> operator+(const Vector3<T_2> &v) const;

	// Soustraction
	template <class T_2>
	inline Vector3<T> operator-(const Vector3<T_2> &v) const;

	// Division par un scalaire (v / s)
	template <class T_scalar>
	inline Vector3<T> operator/(const T_scalar &s) const;

	// Normalisation
	inline void normalize();

	// Calcul de la norme
	inline T norm() const;

	// Calcul de la norme au carré (plus rapide)
	inline decltype(x *x) squaredNorm() const {
		return dot(*this, *this);
	}

	// axis must be a unit lenght vector
	template <typename U, typename V>
	Vector3<T> rotate(Vector3<U> const &axis, V const &angle) const;
};

// Multiplication par un scalaire (v * s)
template <class T_scalar, typename T>
inline auto operator*(const T_scalar &s, const Vector3<T> &v)
	-> std::enable_if_t<Units::is_unit<T_scalar>::value || std::is_scalar<T_scalar>::value, Vector3<decltype(v.x *s)>> {
	return Vector3<decltype(s *v.x)>(v.x * s, v.y * s, v.z * s);
}

template <class T_scalar, typename T>
inline auto operator*(const Vector3<T> &v, const T_scalar &s)
	-> std::enable_if_t<Units::is_unit<T_scalar>::value || std::is_scalar<T_scalar>::value, Vector3<decltype(v.x *s)>> {
	return Vector3<decltype(s *v.x)>(v.x * s, v.y * s, v.z * s);
}


template <typename T>
bool operator==(Vector3<T> const &v1, Vector3<T> const &v2) {
	return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
}

template <typename T>
bool operator!=(Vector3<T> const &v1, Vector3<T> const &v2) {
	return !(v1 == v2);
}

// Division par un scalaire (s / v)
template <class T, class T_scalar>
Vector3<T> operator/(const T_scalar &s, const Vector3<T> &v);

// Fonction pour le produit scalaire
template <class T1, class T2>
inline auto dot(const Vector3<T1> &v1, const Vector3<T2> &v2) -> decltype(v1.x *v2.x) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

// Fonction pour le produit vectoriel
template <class T, class U>
inline Vector3<decltype(std::declval<T>() * std::declval<U>())> cross(const Vector3<T> &v1, const Vector3<U> &v2) {
	Vector3<decltype(v1.x *v2.x)> resultat;
	resultat.x = v1.y * v2.z - v1.z * v2.y;
	resultat.y = -(v1.x * v2.z - v1.z * v2.x);
	resultat.z = v1.x * v2.y - v1.y * v2.x;
	return resultat;
}

// Affichage avec iostream
template <class T>
inline std::ostream &operator<<(std::ostream &os, const Vector3<T> &v) {
	os << "[" << v.x << ", " << v.y << ", " << v.z << "]";
	return os;
}

typedef Vector3<double> Vector3d;
typedef Vector3<float> Vector3f;
typedef Vector3<unsigned int> Vector3ui;
typedef Vector3<int> Vector3i;
typedef Vector3<short> Vector3s;
typedef Vector3<unsigned short> Vector3us;
typedef Vector3<unsigned char> Vector3ub;

#include "Vector3.hpp"

#endif // VECTOR_3_H
