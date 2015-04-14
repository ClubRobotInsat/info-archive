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
Vector3m translationPourRotationExcentree(Vector3m centreAbsolu, Vector3m centreVoulu, Vector3d axeRotation, Angle angleRotation);

#endif // MATH_TOOLBOX_H
