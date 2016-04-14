//
//  MathToolbox.cpp
//  Club Robot
//
//  Created by Rémi on 24/03/2015.
//

#include "MathToolbox.h"

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

Vector2m toVec2(Vector3m vec3) {
	return Vector2m(vec3.x, vec3.y);
}

/**
 * Retourne vrai si la différence angulaire entre l'angle a1
 * et l'angle 'a2' est inférieure à l'angle 'threshold'.
 *
 */
bool equals(Units::Angle a1, Units::Angle a2, Units::Angle threshold) {
	return abs((a1 - a2).toMinusPiPi()) < abs(threshold.toMinusPiPi());
}
