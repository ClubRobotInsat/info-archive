//
//  MathToolbox.cpp
//  Club Robot
//
//  Created by Rémi on 24/03/2015.
//

#include "MathToolbox.h"

Vector3m translationPourRotationExcentree(Vector3m centreAbsolu, Vector3m centreVoulu, Vector3d axeRotation, Angle angleRotation)
{
	// On converti tout en dimension 4 pour profiter des capacités de calcul des rotations en 4D
	Vector4d centreAbsolu4 = {centreAbsolu.x.value(), centreAbsolu.y.value(), centreAbsolu.z.value(), 0.0};
	Vector4d centreVoulu4 = {centreVoulu.x.value(), centreVoulu.y.value(), centreVoulu.z.value(), 0.0};
	Matrix4d rotation = Matrix4d::rotationMatrix(axeRotation, angleRotation.toDeg());
	Vector4d imageCentreVoulu4 = rotation*centreVoulu4;
	Vector4d translation = imageCentreVoulu4 - centreVoulu4;
	return {Distance::makeFromM(translation.x), Distance::makeFromM(translation.y), Distance::makeFromM(translation.z)};
}

/**
 * Retourne vrai si la différence angulaire entre l'angle a1
 * et l'angle 'a2' est inférieure à l'angle 'threshold'.
 *
 */
bool equals(Angle a1, Angle a2, Angle threshold) {
	return abs((a1 - a2).toMoinsPiPi()) < abs(threshold.toMoinsPiPi());
}
