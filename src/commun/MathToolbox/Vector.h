// Vecteur.h :
// - Vecteur<N, T> : vecteur à N dimensions d'éléments de type T
// - Vecteur<2, T> : vecteur à 2 dimensions d'éléments de type T, avec certaines fonctions
// en plus
// - Vecteur<3, T> : vecteur à 3 dimensions d'éléments de type T, avec certaines fonctions
// en plus
// + des typedefs pour les vecteurs 2D et 3D (b : byte, s : short, f : float, d : double,
// u : unsigned) :
// - Vecteur2ub, Vecteur2i, Vecteur2s, Vecteur2us, Vecteur2ui, Vecteur2f, Vecteur2d
// - Vecteur3ub, Vecteur3i, Vecteur3s, Vecteur3us, Vecteur3ui, Vecteur3f, Vecteur3d

// NB : pour pouvoir utiliser le constructeur de copie, il faut que l'opérateur = soit
// défini pour le type des scalaires contenus dans le vecteur.

// NB : l'unité par défaut des angles est le radian, des fonctions de conversion existent
// dans MathsUtilities.h. Par défaut, les valeurs renvoyées sont situées entre -PI et PI.

#ifndef VECTEUR_H
#define VECTEUR_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#pragma GCC diagnostic pop

#endif // VECTEUR_H
