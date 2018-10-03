#pragma once
/******************************************************************************/
/* robot/Lidar/filtre.h                                            2015-03-17 */
/* Club Robot INSA Toulouse                                      Félix Poisot */
/******************************************************************************/
#include "Driver/lidar.h"
/******************************************************************************/

// Filtre en ligne pour les relevés du Lidar
class Filtre {
public:
	const TrameLidar& getTrame(const TrameLidar& input);

private:
	TrameLidar _state;
	std::vector<double> _variances;
};
