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

/**
* Retourne vrai si la différence angulaire entre l'angle a1
* et l'angle 'a2' est inférieure à l'angle 'threshold'.
*
* On utilise une méthode 'bourrine' qui consiste à calculer
* la distance entre les 2 points formés par les 2 angles à comparer
* sur le cercle rigo, le seuil donné (que l'on projette sur le cercle).
*/
bool equals(Angle a1, Angle a2, Angle threshold);

#endif // MATH_TOOLBOX_H
