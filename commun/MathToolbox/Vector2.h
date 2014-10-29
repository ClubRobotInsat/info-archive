// Vector2.h
// Vector en 2 dimensions
// 2008 par Funto
// Version 1.0

#ifndef VECTOR2_H
#define VECTOR2_H

#include <iostream>

#define VECTOR2_OPENGL_SUPPORT

#include <iostream>

template <class T>
class Vector2 {
public:
	union {
		struct{T r, g;};
		struct{T s, t;};
		struct{T x, y;};
	};
	
	T &operator[](std::size_t i) {
		return (&x)[i];
	}
	T const &operator[](std::size_t i) const {
		return (&x)[i];
	}
	
public:
	// Constructeurs
	Vector2(); // par défaut
	
	Vector2(const T& x, const T& y); // à partir des coordonnées
	
	template <class T_scalar>
	Vector2(const T_scalar& s); // à partir d'un seul scalaire
	
	Vector2(const Vector2<T>& ref); // de copie 1
	
	template <class T_2>
	Vector2(const Vector2<T_2>& ref); // de copie 2
	
	~Vector2() = default;
	
	// Affectation suivant un autre vecteur
	template <class T_2>
	inline Vector2<T>& operator=(const Vector2<T_2>& ref);
	
	// Affectation suivant un autre vecteur du même type (sinon, ça compile pas...)
	Vector2<T>& operator=(const Vector2<T>& ref);
	
	// Affectation suivant un scalaire
	template <class T_scalar>
	inline Vector2<T>& operator=(const T_scalar& s);
	
	// Affectation-addition
	template <class T_2>
	inline Vector2<T>& operator+=(const Vector2<T_2>& v);
	
	// Affectation-soustraction
	template <class T_2>
	inline Vector2<T>& operator-=(const Vector2<T_2>& v);
	
	// Affectation-multiplication par un scalaire
	template <class T_scalar>
	inline Vector2<T>& operator*=(const T_scalar& s);
	
	// Affectation-division par un scalaire
	template <class T_scalar>
	inline Vector2<T>& operator/=(const T_scalar& s);
	
	// Négation
	inline Vector2<T> operator-() const;
	
	// Addition
	template <class T_2>
	inline Vector2<T> operator+(const Vector2<T_2>& v) const;
	
	// Soustraction
	template <class T_2>
	inline Vector2<T> operator-(const Vector2<T_2>& v) const;
	
	// Multiplication par un scalaire (v * s)
	template <class T_scalar>
	inline Vector2<T> operator*(const T_scalar& s) const;
	
	// Division par un scalaire (v / s)
	template <class T_scalar>
	inline Vector2<T> operator/(const T_scalar& s) const;
	
	// Produit scalaire
	// Bug de Clang dans sa version 3.5, qui butte sur la résolution automatique de type de fonctions (avec génération des symboles de debug), faut lui expliquer
	auto operator*(const Vector2<T>& v) const
#if __clang__
	// Bug de g++ dans sa version 4.9, segfault à la compilation avec cette ligne
 		-> decltype(x * v.x)
#endif
	{
		return this->x * v.x + this->y * v.y;
	}

	// Normalisation + renvoi de la valeur de la norme
	inline void normalize();
	
	// Calcul de la norme
	inline T norm() const;
	
	// Calcul de la norme au carré (plus rapide)
	inline auto squaredNorm() -> decltype(x * x) const {
		return *this * *this;
	}
	
	// Calcul de l'angle
	// Bug de Clang dans sa version 3.5 au moins, qui butte sur la résolution automatique de type de fonctions (avec génération des symboles de debug), faut lui expliquer
	auto angle() -> decltype(atan2(y, x)) const {
		return atan2(y, x);
	}

	// Rotation du vecteur
	template<typename U>
	inline Vector2<T> &rotate(U angle);
};

template <typename T>
bool operator==(Vector2<T> const &v1, Vector2<T> const &v2) {
	return v1.x == v2.x && v1.y == v2.y;
}

template <typename T>
bool operator!=(Vector2<T> const &v1, Vector2<T> const &v2) {
	return !(v1 == v2);
}

// Multiplication par un scalaire (s * v)
template <class T, class T_scalar>
Vector2<T> operator*(const T_scalar& s, const Vector2<T>& v);

// Division par un scalaire (s / v)
template <class T, class T_scalar>
Vector2<T> operator/(const T_scalar& s, const Vector2<T>& v);

// Fonction pour le produit scalaire
template <class T>
inline T dot(const Vector2<T>& v1, const Vector2<T>& v2) {
	return v1 * v2;
}

// Affichage avec iostream
template <class T>
inline std::ostream& operator<<(std::ostream& os, const Vector2<T>& v) {
	os << "[" << v.x << ", " << v.y << "]";
	return os;
}

typedef Vector2<double> Vector2d;
typedef Vector2<float> Vector2f;
typedef Vector2<unsigned int> Vector2ui;
typedef Vector2<int> Vector2i;
typedef Vector2<short> Vector2s;
typedef Vector2<unsigned char> Vector2ub;

#include "Vector2.hpp"

#endif // VECTOR2_H
