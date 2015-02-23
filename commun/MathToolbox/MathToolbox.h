// MathToolbox.h

#ifndef MATH_TOOLBOX_H
#define MATH_TOOLBOX_H

#include "MathUtilities.h"
#include "Vector.h"
#include "Matrix3.h"
#include "Matrix4.h"

#include "../Units/Units.h"

typedef Vector2<Distance> Vector2m;
typedef Vector3<Distance> Vector3m;
typedef Vector4<Distance> Vector4m;

// Retourne la translation qui permet de donner l'impression de faire tourner un élément autour du centreVoulu alors
// qu'il tourne autour du centreAbsolu pour la rotation décrite par axeRotation et angleRotation
// Il faut donc faire tourner la pièce, puis déterminer la translation par cette fonction, et l'appliquer à la pièce.
inline Vector3m translationPourRotationExcentree(Vector3m centreAbsolu, Vector3m centreVoulu, Vector3d axeRotation, Angle angleRotation)
{
	// On converti tout en dimension 4 pour profiter des capacités de calcul des rotations en 4D
	Vector4d centreAbsolu4 = {centreAbsolu.x.value(), centreAbsolu.y.value(), centreAbsolu.z.value(), 0.0};
	Vector4d centreVoulu4 = {centreVoulu.x.value(), centreVoulu.y.value(), centreVoulu.z.value(), 0.0};
	Matrix4d rotation = Matrix4d::rotationMatrix(axeRotation, angleRotation.toDeg());
	Vector4d imageCentreVoulu4 = rotation*centreVoulu4;
	Vector4d translation = imageCentreVoulu4 - centreVoulu4;
	return {Distance::makeFromM(translation.x), Distance::makeFromM(translation.y), Distance::makeFromM(translation.z)};
}

#endif // MATH_TOOLBOX_H
