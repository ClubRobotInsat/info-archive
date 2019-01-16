#pragma once
/******************************************************************************/
/* robot/Lidar/filtre.h                                            2015-03-17 */
/* Club Robot INSA Toulouse                                      Félix Poisot */
/******************************************************************************/
#include "Driver/lidar.h"
/******************************************************************************/

namespace Lidar {
	// Filtre en ligne pour les relevés du Lidar
	class Filtre {
	public:
		const FrameLidar& get_frame(const FrameLidar& input);

	private:
		FrameLidar _state;
		std::vector<double> _variances;
	};
} // namespace Lidar
