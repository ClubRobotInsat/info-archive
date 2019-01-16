#pragma once
/******************************************************************************/
/* robot/Lidar/Driver/hokuyoLidar.h                                2015-03-05 */
/* Club Robot INSA Toulouse                                      Félix Poisot */
/******************************************************************************/
#include "../../robot/Communication/RS232.h"
#include "lidar.h"
#include <libusb-1.0/libusb.h>
/******************************************************************************/

namespace Lidar {
	class HokuyoLidar final : public Lidar {
	public:
		static const int VID = 0x15d1;

		explicit HokuyoLidar(const std::string& device);

		~HokuyoLidar() override;

		FrameLidar get_frame() override;

		Time get_time_origin() override;

		void start();

		void stop();

	private:
		Time _timestamp;
		std::string _command;
		std::unique_ptr<Communication::RS232> _serial;
	};
} // namespace Lidar
