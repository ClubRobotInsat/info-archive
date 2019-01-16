#pragma once
/*****************************************************************************/
/* robot/Lidar/affiche.h                                          2015-05-25 */
/* Club Robot INSA Toulouse                                     Félix Poisot */
/*****************************************************************************/
//#include "../../simulateur/_simulateur/Texture.h"
#include "Constants.h"
#include "MathToolbox/Repere.h"
#include "lidarsToGrid.h"
#include <Clock.h>
/*****************************************************************************/

using Vec3 = Vector3f;
using Vec4 = Vector4f;

// Sortie graphique représentant la table et les données à plusieurs étapes du
// traitement, pour faciliter la mise au point.
class Display {
public:
	explicit Display(Vector2m table_size = toVec2(GLOBAL_CONSTANTS().get_table_size()));
	~Display();

	bool isClosed(); // passe à vrai quand on clique sur la croix.

	// Toutes les commandes doivent être dans une paire de b/e(). Pas droit de
	// nester les paires. L'écran n'est rafraichit que sur end().
	void begin();
	void end();

	// Toutes les positions sont données en mètres, avec le 0 en bas à gauche de
	// la table.

	// Mollo sur l'alpha, qu'on puisse encore voir la table au travers.
	void frame_lidar(const Lidar::FrameLidar& mesure, const repere::Coordinates& lidar_coords, Vec4 color);

	void grid(const Lidar::OccupGrid& occ, Vec3 color);

	void candidates(const std::vector<repere::Position>& pts, Vec3 color);

private:
	// Texture _textureTable; // GL name
	Vector2m _table_size;
	TimePoint _t0;
	TimePoint _fpsT; // dernière actualisation des FPS dans la barre de titre
	int _frames;     // depuis _fpsT
};
