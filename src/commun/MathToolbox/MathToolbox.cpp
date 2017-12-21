//
//  MathToolbox.cpp
//  Club Robot
//
//  Created by Rémi on 24/03/2015.
//

#include "MathToolbox.h"

#include "Units.h"

Vector3m translationPourRotationExcentree(Vector3m centreAbsolu, Vector3m centreVoulu, Vector3d axeRotation, Units::Angle angleRotation) {
	// On convertit tout en dimension 4 pour profiter des capacités de calcul des rotations en 4D
	Vector4d centreAbsolu4 = {centreAbsolu.x.toM(), centreAbsolu.y.toM(), centreAbsolu.z.toM(), 0.0};
	Vector4d centreVoulu4 = {centreVoulu.x.toM(), centreVoulu.y.toM(), centreVoulu.z.toM(), 0.0};
	Matrix4d rotation = Matrix4d::rotationMatrix(axeRotation, angleRotation.toDeg());
	Vector4d imageCentreVoulu4 = rotation * centreVoulu4;
	Vector4d translation = imageCentreVoulu4 - centreVoulu4;
	return {Units::Distance::makeFromM(translation.x),
	        Units::Distance::makeFromM(translation.y),
	        Units::Distance::makeFromM(translation.z)};
}

Matrix4f getTransformationMatrix(const Vector3f& pos, const Vector3f& rot, const Vector3f& scale) {
	Matrix4f matIdentity;
	matIdentity *= Matrix4f::translationMatrix(pos) * Matrix4f::rotationMatrix({1, 0, 0}, rot.x) *
	               Matrix4f::rotationMatrix({0, 1, 0}, rot.y) * Matrix4f::rotationMatrix({0, 0, 1}, rot.z) *
	               Matrix4f::scaleMatrix(scale);
	return matIdentity;
}

Vector2m toVec2(Vector3m vec3) {
	return Vector2m(vec3.x, vec3.y);
}

Vector3m toVec3(Vector2m vec2) {
	return Vector3m(vec2.x, vec2.y, 0_m);
}

/**
 * Retourne vrai si la différence angulaire entre l'angle a1
 * et l'angle 'a2' est inférieure à l'angle 'threshold'.
 *
 */
bool equals(Units::Angle a1, Units::Angle a2, Units::Angle threshold) {
	return abs((a1 - a2).toMinusPiPi()) < abs(threshold.toMinusPiPi());
}

bool distancesEquals(Vector2m a, Vector2m b, Units::Distance threshold) {
	return pow((a.x.toM() - b.x.toM()), 2) + pow((a.y.toM() - b.y.toM()), 2) < pow(threshold.toM(), 2);
}