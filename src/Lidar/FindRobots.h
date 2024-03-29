#pragma once
/*****************************************************************************/
/* robot/Lidar/trouveRobots.h                                     2015-05-11 */
/* Club Robot INSA Toulouse                                     Félix Poisot */
/*****************************************************************************/
#include "MathToolbox/Repere.h"
#include "lidarsToGrid.h"
/*****************************************************************************/

// Filtre en ligne suivant les robots potentiels dans la suite de grilles
// d'occupations
class FindRobots {
public:
	FindRobots() {}

	void accumulate(const OccupGrid&);

	// Dans le même référentiel que celui de OccupGrid.
	std::vector<repere::Position> get_results() const;

private:
	std::vector<repere::Position> _candidats;
	std::vector<int> _nbWin;
};
