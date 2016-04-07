// Vector3.hpp
// Vector en 3 dimensions
// 2008 par Funto
// Version 1.0

#include <cmath>

template <class T>
constexpr Vector3<T>::Vector3() : x(T()), y(T()), z(T()) {}

template <class T>
constexpr Vector3<T>::Vector3(const T& x, const T& y, const T& z) : x(x), y(y), z(z) {}

template <class T>
template <class T_scalar>
Vector3<T>::Vector3(const T_scalar& s) : x(s), y(s), z(s) {}

template <class T>
template <class T_scalar>
Vector3<T>::Vector3(T_scalar const* s) : x(s[0]), y(s[1]), z(s[2]) {}

template <class T>
Vector3<T>::Vector3(const Vector3<T>& ref) : x(ref.x), y(ref.y), z(ref.z) {}

template <class T>
template <class T_2>
Vector3<T>::Vector3(const Vector3<T_2>& ref) : x(ref.x), y(ref.y), z(ref.z) {}

// Affectation suivant un autre vecteur
template <class T>
template <class T_2>
Vector3<T>& Vector3<T>::operator=(const Vector3<T_2>& ref) {
	this->x = ref.x;
	this->y = ref.y;
	this->z = ref.z;
	return *this;
}

// Affectation suivant un autre vecteur du même type (sinon, ça compile pas...)
template <class T>
Vector3<T>& Vector3<T>::operator=(const Vector3<T>& ref) {
	this->x = ref.x;
	this->y = ref.y;
	this->z = ref.z;
	return *this;
}

// Affectation suivant un scalaire
template <class T>
template <class T_scalar>
Vector3<T>& Vector3<T>::operator=(const T_scalar& s) {
	this->x = s;
	this->y = s;
	this->z = s;
	return *this;
}

// Affectation-addition
template <class T>
template <class T_2>
Vector3<T>& Vector3<T>::operator+=(const Vector3<T_2>& v) {
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
	return *this;
}

// Affectation-soustraction
template <class T>
template <class T_2>
Vector3<T>& Vector3<T>::operator-=(const Vector3<T_2>& v) {
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
	return *this;
}

// Affectation-multiplication par un scalaire
template <class T>
template <class T_scalar>
Vector3<T>& Vector3<T>::operator*=(const T_scalar& s) {
	this->x *= s;
	this->y *= s;
	this->z *= s;
	return *this;
}

// Affectation-division par un scalaire
template <class T>
template <class T_scalar>
Vector3<T>& Vector3<T>::operator/=(const T_scalar& s) {
	this->x /= s;
	this->y /= s;
	this->z /= s;
	return *this;
}

// Négation
template <class T>
inline Vector3<T> Vector3<T>::operator-() const {
	return Vector3<T>(-this->x, -this->y, -this->z);
}

// Addition
template <class T>
template <class T_2>
Vector3<T> Vector3<T>::operator+(const Vector3<T_2>& v) const {
	return Vector3<T>(this->x + v.x, this->y + v.y, this->z + v.z);
}

// Soustraction
template <class T>
template <class T_2>
Vector3<T> Vector3<T>::operator-(const Vector3<T_2>& v) const {
	return Vector3<T>(this->x - v.x, this->y - v.y, this->z - v.z);
}

// Division par un scalaire
template <class T>
template <class T_scalar>
Vector3<T> Vector3<T>::operator/(const T_scalar& s) const {
	return Vector3<T>(this->x / s, this->y / s, this->z / s);
}

// Normalisation
template <class T>
void Vector3<T>::normalize() {
	double norm = sqrtf(x * x + y * y + z * z);
	x = T(double(x) / norm);
	y = T(double(y) / norm);
	z = T(double(z) / norm);
}

// Calcul de la norme
template <class T>
T Vector3<T>::norm() const {
	return sqrt(x * x + y * y + z * z);
}

// Division par un scalaire (s / v)
template <class T, class T_scalar>
Vector3<T> operator/(const T_scalar& s, const Vector3<T>& v) {
	return Vector3<T>(v.x / s, v.y / s, v.z / s);
}

template <typename T>
template <typename U, typename V>
Vector3<T> Vector3<T>::rotate(Vector3<U> const& axis, V const& angle) const {
	// axis must be a unit lenght vector

	auto o = dot(axis, *this) * axis;
	auto x = *this - o;

	auto y = cross(axis, *this);

	return o + cos(angle) * x + sin(angle) * y;
}
