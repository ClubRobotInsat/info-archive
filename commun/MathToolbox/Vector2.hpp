// Vector2.hpp
// Vector en 2 dimensions
// 2008 par Funto
// Version 1.0

template <class T>
constexpr Vector2<T>::Vector2() : x(T()), y(T()) {
	
}

template <class T>
constexpr Vector2<T>::Vector2(const T& xx, const T& yy) : x(xx), y(yy) {
	
}

template <class T>
template <class T_scalar>
Vector2<T>::Vector2(const T_scalar& s) : x(s), y(s) {
	
}

template <class T>
Vector2<T>::Vector2(const Vector2<T>& ref) : x(ref.x), y(ref.y) {
	
}

template <class T>
template <class T_2>
Vector2<T>::Vector2(const Vector2<T_2>& ref) : x(ref.x), y(ref.y) {
	
}

// Affectation suivant un autre vecteur
template <class T>
template <class T_2>
Vector2<T>& Vector2<T>::operator=(const Vector2<T_2>& ref) {
	this->x = ref.x;
	this->y = ref.y;
	return *this;
}

// Affectation suivant un autre vecteur du même type (sinon, ça compile pas...)
template <class T>
Vector2<T>& Vector2<T>::operator=(const Vector2<T>& ref) {
	this->x = ref.x;
	this->y = ref.y;
	return *this;
}

// Affectation suivant un scalaire
template <class T>
template <class T_scalar>
Vector2<T>& Vector2<T>::operator=(const T_scalar& s) {
	this->x = s;
	this->y = s;
	return *this;
}

// Affectation-addition
template <class T>
template <class T_2>
Vector2<T>& Vector2<T>::operator+=(const Vector2<T_2>& v) {
	this->x += v.x;
	this->y += v.y;
	return *this;
}

// Affectation-soustraction
template <class T>
template <class T_2>
Vector2<T>& Vector2<T>::operator-=(const Vector2<T_2>& v) {
	this->x -= v.x;
	this->y -= v.y;
	return *this;
}

// Affectation-multiplication par un scalaire
template <class T>
template <class T_scalar>
Vector2<T>& Vector2<T>::operator*=(const T_scalar& s) {
	this->x *= s;
	this->y *= s;
	return *this;
}

// Affectation-division par un scalaire
template <class T>
template <class T_scalar>
Vector2<T>& Vector2<T>::operator/=(const T_scalar& s) {
	this->x /= s;
	this->y /= s;
	return *this;
}

// Négation
template <class T>
inline Vector2<T> Vector2<T>::operator-() const {
	return Vector2<T>(-this->x, -this->y);
}

// Addition
template <class T>
template <class T_2>
Vector2<T> Vector2<T>::operator+(const Vector2<T_2>& v) const {
	return Vector2<T>(this->x + v.x, this->y + v.y);
}

// Soustraction
template <class T>
template <class T_2>
Vector2<T> Vector2<T>::operator-(const Vector2<T_2>& v) const {
	return Vector2<T>(this->x - v.x, this->y - v.y);
}

// Multiplication par un scalaire
template <class T>
template <class T_scalar>
Vector2<T> Vector2<T>::operator*(const T_scalar& s) const {
	return Vector2<T>(this->x * s, this->y * s);
}

// Normalisation + renvoi de la valeur de la norme
template <class T>
void Vector2<T>::normalize() {
	auto norm = sqrt(x*x + y*y);
	x = T(x / norm);
	y = T(y / norm);
}

// Calcul de la norme
template <class T>
T Vector2<T>::norm() const {
	return sqrt(x*x + y*y);
}

// Rotation du vecteur
template <class T>
template <class U>
Vector2<T> &Vector2<T>::rotate(U angle) {
	// On multiplie par la matrice :
	// (cos(theta), -sin(theta) )
	// (sin(theta), cos(theta) )
	
	auto cos_angle = cos(angle);
	auto sin_angle = sin(angle);
	T old_x = x;
	
	x = cos_angle*old_x - sin_angle*y;
	y = sin_angle*old_x + cos_angle*y;
	
	return *this;
}

// Multiplication par un scalaire (s * v)
template <class T, class T_scalar>
auto operator*(const T_scalar& s, const Vector2<T>& v) {
	return Vector2<decltype(v.x * s)>(v.x * s, v.y * s);
}

// Division par un scalaire (s / v)
template <class T, class T_scalar>
Vector2<T> operator/(const T_scalar& s, const Vector2<T>& v) {
	return Vector2<T>(v.x / s, v.y / s);
}
