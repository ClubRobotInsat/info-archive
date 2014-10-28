// Vector4.hpp
// Vector en 4 dimensions
// 2008 par Funto
// Version 1.0

#include <cmath>

template <class T>
Vector4<T>::Vector4() : x(T(0)), y(T(0)), z(T(0)), w(T(1)) {
	
}

// Cas particulier pour les unsigned char : w = 255
template <>
inline Vector4<unsigned char>::Vector4() : x(0), y(0), z(0), w(255) {

}

template <class T>
Vector4<T>::Vector4(const T& xx, const T& yy, const T& zz, const T& ww) : x(xx), y(yy), z(zz), w(ww) {
	
}

template <class T>
template <class T_scalar>
Vector4<T>::Vector4(const T_scalar& s) : x(s), y(s), z(s), w(s) {
	
}

template <class T>
Vector4<T>::Vector4(const Vector4<T>& ref) : x(ref.x), y(ref.y), z(ref.z), w(ref.w) {
	
}

template <class T>
template <class T_2>
Vector4<T>::Vector4(const Vector4<T_2>& ref) : x(ref.x), y(ref.y), z(ref.z), w(ref.w) {
	
}

// Affectation suivant un autre vecteur
template <class T>
template <class T_2>
Vector4<T>& Vector4<T>::operator=(const Vector4<T_2>& ref) {
	this->x = ref.x;
	this->y = ref.y;
	this->z = ref.z;
	this->w = ref.w;
	return *this;
}


// Affectation suivant un autre vecteur du même type (sinon, ça compile pas...)
template <class T>
Vector4<T>& Vector4<T>::operator=(const Vector4<T>& ref) {
	this->x = ref.x;
	this->y = ref.y;
	this->z = ref.z;
	this->w = ref.w;
	return *this;
}


// Affectation suivant un scalaire
template <class T>
template <class T_scalar>
Vector4<T>& Vector4<T>::operator=(const T_scalar& s) {
	this->x = s;
	this->y = s;
	this->z = s;
	this->w = s;
	return *this;
}

// Affectation-addition
template <class T>
template <class T_2>
Vector4<T>& Vector4<T>::operator+=(const Vector4<T_2>& v) {
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
	this->w += v.w;
	return *this;
}

// Affectation-soustraction
template <class T>
template <class T_2>
Vector4<T>& Vector4<T>::operator-=(const Vector4<T_2>& v) {
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
	this->w -= v.w;
	return *this;
}

// Affectation-multiplication par un scalaire
template <class T>
template <class T_scalar>
Vector4<T>& Vector4<T>::operator*=(const T_scalar& s) {
	this->x *= s;
	this->y *= s;
	this->z *= s;
	this->w *= s;
	return *this;
}

// Affectation-division par un scalaire
template <class T>
template <class T_scalar>
Vector4<T>& Vector4<T>::operator/=(const T_scalar& s) {
	this->x /= s;
	this->y /= s;
	this->z /= s;
	this->w /= s;
	return *this;
}

// Négation
template <class T>
inline Vector4<T> Vector4<T>::operator-() const {
	return Vector4<T>(-this->x, -this->y, -this->z, -this->w);
}

// Addition
template <class T>
template <class T_2>
Vector4<T> Vector4<T>::operator+(const Vector4<T_2>& v) const {
	return Vector4<T>(this->x + v.x, this->y + v.y, this->z + v.z, this->w + v.w);
}

// Soustraction
template <class T>
template <class T_2>
Vector4<T> Vector4<T>::operator-(const Vector4<T_2>& v) const {
	return Vector4<T>(this->x - v.x, this->y - v.y, this->z - v.z, this->w - v.w);
}

// Multiplication par un scalaire
template <class T>
template <class T_scalar>
Vector4<T> Vector4<T>::operator*(const T_scalar& s) const {
	return Vector4<T>(this->x * s, this->y * s, this->z * s, this->w * s);
}

// Division par un scalaire
template <class T>
template <class T_scalar>
Vector4<T> Vector4<T>::operator/(const T_scalar& s) const {
	return Vector4<T>(this->x / s, this->y / s, this->z / s, this->w / s);
}

// Produit scalaire
template <class T>
T Vector4<T>::operator*(const Vector4<T>& v) const {
	return this->x * v.x + this->y * v.y + this->z * v.z + this->w * v.w;
}

// Normalisation + renvoi de la valeur de la norme
template <class T>
void Vector4<T>::normalize() {
	double norm = sqrtf(x*x + y*y + z*z + w*w);
	x = T(double(x) / norm);
	y = T(double(y) / norm);
	z = T(double(z) / norm);
	w = T(double(w) / norm);
}

// Calcul de la norme
template <class T>
double Vector4<T>::norm() const {
	return sqrtf(x*x + y*y + z*z + w*w);
}

// Calcul de la norme au carré (plus rapide)
template <class T>
T Vector4<T>::squaredNorm() const {
	return x*x + y*y + z*z + w*w;
}

// Multiplication par un scalaire (s * v)
template <class T, class T_scalar>
Vector4<T> operator*(const T_scalar& s, const Vector4<T>& v) {
	return Vector4<T>(v.x * s, v.y * s, v.z * s, v.w * s);
}

// Division par un scalaire (s / v)
template <class T, class T_scalar>
Vector4<T> operator/(const T_scalar& s, const Vector4<T>& v) {
	return Vector4<T>(v.x / s, v.y / s, v.z / s, v.w / s);
}
