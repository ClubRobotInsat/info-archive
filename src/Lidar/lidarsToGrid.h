#pragma once
/*****************************************************************************/
/* robot/Lidar/lidarsToGrid.h                                     2015-04-27 */
/* Club Robot INSA Toulouse                                     Félix Poisot */
/*****************************************************************************/
#include "Driver/lidar.h"
#include "MathToolbox/Repere.h"
#include <MathToolbox/MathToolbox.h>
/*****************************************************************************/

namespace Lidar {
	using IVec2 = Vector2i;

	// Grille d'occupation. Origine en bas à gauche. Distances en mètres.
	class OccupGrid {
	public:
		OccupGrid(Vector2m size, int resX, int resY);

		OccupGrid(Distance width, Distance height, int resX, int resY);

		// remet à l'état complètement innocupé
		void reset();

		// Les obstacles en vue de ce lidar passent des cellules en 'occupé'
		// Donner la pose du lidar dans l'espace de la grid, en mètres.
		void accumulate(FrameLidar t, const repere::Coordinates& lidar_coords);

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

		Vector2m size() const {
			return {_width, _height};
		}

	private:
		std::vector<char> _mat; // ne pas utiliser bool: on a droit à la version fancy à bitmasks, qui compresse pour avoir
		// une cellule par bit.
		Distance _width;
		Distance _height;
		int _resX;
		int _resY;

		void _rsLine(IVec2 a, IVec2 b);
	};
} // namespace Lidar
