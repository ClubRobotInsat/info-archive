#pragma once
/*****************************************************************************/
/* robot/Lidar/affiche.h                                          2015-05-25 */
/* Club Robot INSA Toulouse                                     Félix Poisot */
/*****************************************************************************/
#include "../../simulateur/_simulateur/Texture.h"
#include "lidarsToGrid.h"
#include <Clock.h>
/*****************************************************************************/

using Vec3 = Vector3f;
using Vec4 = Vector4f;

// Sortie graphique représentant la table et les données à plusieurs étapes du
// traitement, pour faciliter la mise au point.
class Affiche {
public:
	Affiche(Vec2 tailleTable);
	~Affiche();

	bool isClosed(); // passe à vrai quand on clique sur la croix.

	// Toutes les commandes doivent être dans une paire de b/e(). Pas droit de
	// nester les paires. L'écran n'est rafraichit que sur end().
	void begin();
	void end();

	// Toutes les positions sont données en mètres, avec le 0 en bas à gauche de
	// la table.

	// Mollo sur l'alpha, qu'on puisse encore voir la table au travers.
	void trameLidar(const TrameLidar& mesure, Vec2 orig, Angle orient, Vec4 couleur);

	void grille(const OccupGrid& occ, Vec3 color);

	void candidats(std::vector<Vec2> pts, Vec3 couleur);

private:
	Texture _textureTable; // GL name
	Vec2 _size;
	TimePoint _t0;
	TimePoint _fpsT; // dernière actualisation des FPS dans la barre de titre
	int _frames;     // depuis _fpsT
};
