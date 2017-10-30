// Matrix4.h

#ifndef MATRIX4_H
#define MATRIX4_H

#include "MathUtilities.h"
#include "Matrix3.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#include "Vector3.h"
#include "Vector4.h"
#pragma GCC diagnostic pop

#include <iostream>

template <class T>
class Matrix4 {
public:
	T values[16]; // NB : les valeurs sont enregistrées "façon OpenGL", i.e. "column-major order" :
	              // de l'élément 0 au 3 : c'est la 1ère colonne, etc...

public:
	// Constructeur, matrice identité
	Matrix4();

	// Constructeur à partir de 4 vecteurs
	template <class T_scalar>
	Matrix4(const Vector4<T_scalar>& v1,
	        const Vector4<T_scalar>& v2,
	        const Vector4<T_scalar>& v3,
	        const Vector4<T_scalar>& v4,
	        bool column_vectors = true);

	// Constructeur à partir d'un tableau
	template <class T_scalar>
	Matrix4(T_scalar* values, bool transpose = false);

	// Constructeur à partir de la sous-matrice 3x3 supérieure gauche et du vecteur de translation
	template <class T_scalar>
	Matrix4(const Matrix3<T_scalar>& sub_matrix, const Vector3<T_scalar>& translation);

	// Constructeur de copie
	template <class T_scalar>
	Matrix4(const Matrix4<T_scalar>& ref);

	// Destructeur
	~Matrix4() = default;

	// Opérateur d'affectation
	template <class T_scalar>
	Matrix4<T>& operator=(const Matrix4<T_scalar>& ref);

	// Affectation-addition
	template <class T_scalar>
	Matrix4<T>& operator+=(const Matrix4<T_scalar>& ref);

	// Affectation-soustraction
	template <class T_scalar>
	Matrix4<T>& operator-=(const Matrix4<T_scalar>& ref);

	// Affectation-multiplication
	template <class T_scalar>
	Matrix4<T>& operator*=(const Matrix4<T_scalar>& ref);

	// Affectation-multiplication par un scalaire
	template <class T_scalar>
	Matrix4<T>& operator*=(T_scalar s);

	// Affectation-division par un scalaire
	template <class T_scalar>
	Matrix4<T>& operator/=(T_scalar s);

	// Négation
	Matrix4<T> operator-() const;

	// Addition
	template <class T_scalar>
	Matrix4<T> operator+(const Matrix4<T_scalar>& ref) const;

	// Soustraction
	template <class T_scalar>
	Matrix4<T> operator-(const Matrix4<T_scalar>& ref) const;

	// Multiplication
	Matrix4<T> operator*(const Matrix4<T>& ref) const;

	// Multiplication par un scalaire
	template <class T_scalar>
	Matrix4<T> operator*(T_scalar s) const;

	// Multiplication par un vecteur
	Vector4<T> operator*(const Vector4<T>& v) const;

	// Division par un scalaire
	template <class T_scalar>
	Matrix4<T> operator/(T_scalar s) const;

	// Accès à un élément
	T& operator[](int index);
	const T& operator[](int index) const;

	T& get(int row, int column);
	const T& get(int row, int column) const;

	// Identité
	void setIdentity();

	// Zéro
	void setZero();

	T determinant();

	// Transposition
	Matrix4<T> getTranspose() const;
	void transpose();

	// Inversion
	Matrix4<T> getInverse() const;
	void invert();

	// Transposée de l'inverse
	Matrix4<T> getInverseTranspose() const;
	void invertAndTranspose();

	// Getters/Setters
	Vector4<T> getRow(int i) const;
	Vector4<T> getColumn(int j) const;

	template <class T_scalar>
	void setRow(const Vector4<T_scalar>& v, int i);

	template <class T_scalar>
	void setColumn(const Vector4<T_scalar>& v, int j);

	template <class U>
	static Matrix4<T> translationMatrix(Vector3<U> const& vec) {
		Matrix4<T> mat;
		mat[12] = vec.x;
		mat[13] = vec.y;
		mat[14] = vec.z;

		return mat;
	}

	template <class U>
	static Matrix4<T> scaleMatrix(Vector3<U> const& vec) {
		Matrix4<T> mat;
		mat[0] = vec.x;
		mat[5] = vec.y;
		mat[10] = vec.z;

		return mat;
	}

	template <class U>
	static Matrix4<T> rotationMatrix(Vector3<U> axis, U angleDegrees) {
		axis.normalize();
		T a = angleDegrees * M_PI / 180;

		T c = cos(a);
		T s = sin(a);
		Matrix4<T> m;

		Vector3<T> temp = (T(1) - c) * axis;
		Matrix4<T> Rotate;
		Rotate.setZero();
		Rotate[0] = c + temp[0] * axis[0];
		Rotate[1] = 0 + temp[0] * axis[1] + s * axis[2];
		Rotate[2] = 0 + temp[0] * axis[2] - s * axis[1];

		Rotate[4] = 0 + temp[1] * axis[0] - s * axis[2];
		Rotate[5] = c + temp[1] * axis[1];
		Rotate[6] = 0 + temp[1] * axis[2] + s * axis[0];

		Rotate[8] = 0 + temp[2] * axis[0] + s * axis[1];
		Rotate[9] = 0 + temp[2] * axis[1] - s * axis[0];
		Rotate[10] = c + temp[2] * axis[2];

		Matrix4<T> Result;
		Result.setZero();
		Vector3<T> c1 = Vector3<T>(1, 0, 0) * Rotate[0] + Vector3<T>(0, 1, 0) * Rotate[1] + Vector3<T>(0, 0, 1) * Rotate[2];
		Vector3<T> c2 = Vector3<T>(1, 0, 0) * Rotate[4] + Vector3<T>(0, 1, 0) * Rotate[5] + Vector3<T>(0, 0, 1) * Rotate[6];
		Vector3<T> c3 = Vector3<T>(1, 0, 0) * Rotate[8] + Vector3<T>(0, 1, 0) * Rotate[9] + Vector3<T>(0, 0, 1) * Rotate[10];
		Result[0] = c1.x;
		Result[1] = c1.y;
		Result[2] = c1.z;

		Result[4] = c2.x;
		Result[5] = c2.y;
		Result[6] = c2.z;

		Result[8] = c3.x;
		Result[9] = c3.y;
		Result[10] = c3.z;

		Result[15] = 1;

		return Result;
	}
};

// Multiplication par un scalaire (s * m)
// IMPORTANT : pas de T_scalar, sinon impossible de compiler avec GCC 3 (soit-disant
// que l'ISO est pas d'accord...)
// Donc : le scalaire est du même type que la matrice.
template <class T>
Matrix4<T> operator*(T s, const Matrix4<T>& mat) {
	return mat * s;
}

// Affichage
template <class T>
std::ostream& operator<<(std::ostream& os, const Matrix4<T>& mat);

typedef Matrix4<int> Matrix4i;
typedef Matrix4<float> Matrix4f;
typedef Matrix4<double> Matrix4d;

#include "Matrix4.hpp"

#endif // MATRIX4_H
