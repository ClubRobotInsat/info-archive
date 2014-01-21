// Vector3.h
// Vector en 3 dimensions
// 2008 par Funto
// Version 1.0

#ifndef VECTOR_3_H
#define VECTOR_3_H

#include <iostream>

template <class T>
class Vector3 {
public:
	union {
		struct{T r, g, b;};
		struct{T s, t, p;};
		struct{T x, y, z;};
	};
	
	T &operator[](std::size_t i) {
		return (&x)[i];
	}
	T const &operator[](std::size_t i) const {
		return (&x)[i];
	}
public:
	// Constructeurs
	Vector3(); // par défaut
	
	Vector3(const T& x, const T& y, const T& z); // à partir des coordonnées
	
	template <class T_scalar>
	Vector3(const T_scalar& s); // à partir d'un seul scalaire
	
	Vector3(const Vector3<T>& ref); // de copie 1
	
	template <class T_2>
	Vector3(const Vector3<T_2>& ref); // de copie 2
	
	// Destructeur
	~Vector3() = default;
	
	// Affectation suivant un autre vecteur
	template <class T_2>
	inline Vector3<T>& operator=(const Vector3<T_2>& ref);
	
	// Affectation suivant un autre vecteur du même type (sinon, ça compile pas...)
	Vector3<T>& operator=(const Vector3<T>& ref);
	
	// Affectation suivant un scalaire
	template <class T_scalar>
	inline Vector3<T>& operator=(const T_scalar& s);
	
	// Affectation-addition
	template <class T_2>
	inline Vector3<T>& operator+=(const Vector3<T_2>& v);
	
	// Affectation-soustraction
	template <class T_2>
	inline Vector3<T>& operator-=(const Vector3<T_2>& v);
	
	// Affectation-produit vectoriel
	template <class T_2>
	inline Vector3<T>& operator^=(const Vector3<T_2>& v);
	
	// Affectation-multiplication par un scalaire
	template <class T_scalar>
	inline Vector3<T>& operator*=(const T_scalar& s);
	
	// Affectation-division par un scalaire
	template <class T_scalar>
	inline Vector3<T>& operator/=(const T_scalar& s);
	
	// Négation
	inline Vector3<T> operator-() const;
	
	// Addition
	template <class T_2>
	inline Vector3<T> operator+(const Vector3<T_2>& v) const;
	
	// Soustraction
	template <class T_2>
	inline Vector3<T> operator-(const Vector3<T_2>& v) const;
	
	// Produit vectoriel
	template <class T_2>
	inline Vector3<T> operator^(const Vector3<T_2>& v) const;
	
	// Multiplication par un scalaire (v * s)
	template <class T_scalar>
	inline Vector3<T> operator*(const T_scalar& s) const;
	
	// Division par un scalaire (v / s)
	template <class T_scalar>
	inline Vector3<T> operator/(const T_scalar& s) const;
	
	// Produit scalaire
	inline T operator*(const Vector3<T>& v) const;
	
	// Normalisation + renvoi de la valeur de la norme
	inline void normalize();
	
	// Calcul de la norme
	inline double norm() const;
	
	// Calcul de la norme au carré (plus rapide)
	inline T squaredNorm() const;
};

template <typename T>
bool operator==(Vector3<T> const &v1, Vector3<T> const &v2) {
	return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
}

template <typename T>
bool operator!=(Vector3<T> const &v1, Vector3<T> const &v2) {
	return !(v1 == v2);
}

// Multiplication par un scalaire (s * v)
template <class T, class T_scalar>
Vector3<T> operator*(const T_scalar& s, const Vector3<T>& v);

// Division par un scalaire (s / v)
template <class T, class T_scalar>
Vector3<T> operator/(const T_scalar& s, const Vector3<T>& v);

// Fonction pour le produit scalaire
template <class T>
inline T dot(const Vector3<T>& v1, const Vector3<T>& v2) {
	return v1 * v2;
}

// Fonction pour le produit vectoriel
template <class T>
inline Vector3<T> cross(const Vector3<T>& v1, const Vector3<T>& v2) {
	return v1 ^ v2;
}

// Affichage avec iostream
template <class T>
inline std::ostream& operator<<(std::ostream& os, const Vector3<T>& v) {
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
