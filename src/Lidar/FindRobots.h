#pragma once
/*****************************************************************************/
/* robot/Lidar/trouveRobots.h                                     2015-05-11 */
/* Club Robot INSA Toulouse                                     Félix Poisot */
/*****************************************************************************/
#include "lidarsToGrid.h"
/*****************************************************************************/

// Filtre en ligne suivant les robots potentiels dans la suite de grilles
// d'occupations
class FindRobots {
public:
	FindRobots() {}

	void accumulate(const OccupGrid&);

	// Dans le même référentiel que celui de OccupGrid.
	std::vector<Vec2> get_results() const;

private:
	std::vector<Vec2> _candidats;
	std::vector<int> _nbWin;
};
