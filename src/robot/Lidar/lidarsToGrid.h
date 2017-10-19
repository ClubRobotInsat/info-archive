#pragma once
/*****************************************************************************/
/* robot/Lidar/lidarsToGrid.h                                     2015-04-27 */
/* Club Robot INSA Toulouse                                     Félix Poisot */
/*****************************************************************************/
#include "Driver/lidar.h"
#include <MathToolbox/MathToolbox.h>
/*****************************************************************************/

using Vec2 = Vector2f;
using IVec2 = Vector2i;

// Grille d'occupation. Origine en bas à gauche. Distances en mètres.
class OccupGrid {
public:
	OccupGrid(double width, double height, int resX, int resY);

	// remet à l'état complètement innocupé
	void reset();

	// Les obstacles en vue de ce lidar passent des cellules en 'occupé'
	// Donner la pose du lidar dans l'espace de la grille, en mètres.
	void accumulate(TrameLidar t, Vec2 pos, Angle orient);

	// Accès individuel aux cellules
	// Peut pas retourner bool&, car vector<bool> compresse avec des masques pour avoir une cellule par bit.
	char& operator()(int x, int y) {
		return _mat[y * _resX + x];
	}
	char operator()(int x, int y) const {
		return _mat[y * _resX + x];
	}

	IVec2 resolution() const {
		return {_resX, _resY};
	}
	Vec2 size() const {
		return {_width, _height};
	}

private:
	std::vector<char> _mat; // ne pas utiliser bool: on a droit à la version fancy à bitmasks, qui compresse pour avoir
	                        // une cellule par bit.
	double _width;
	double _height;
	int _resX;
	int _resY;

	void _rsLine(IVec2 a, IVec2 b);
};
