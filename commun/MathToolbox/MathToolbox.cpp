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
* On utilise une méthode 'bourrine' qui consiste à calculer
* la distance entre les 2 points formés par les 2 angles à comparer
* sur le cercle rigo, le seuil donné (que l'on projette sur le cercle).
*/
bool equals(Angle a1, Angle a2, Angle threshold)
{
	float x1 = cos(a1.toRad());
	float y1 = sin(a1.toRad());
	float x2 = cos(a2.toRad());
	float y2 = sin(a2.toRad());
	float dx = x2 - x1;
	float dy = y2 - y1;
	// On calcule le carré de la distance entre les 2 points.
	float distanceSquared = dx * dx + dy * dy;
	// On calule notre seuil angulaire
	float thresh = 2 * (1 - cos(threshold.toRad()));
	return distanceSquared <= thresh;
}
