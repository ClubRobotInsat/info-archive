#pragma once
/******************************************************************************/
/* robot/Lidar/Driver/sickLidar.h                                  2015-03-03 */
/* Club Robot INSA Toulouse                                      Félix Poisot */
/******************************************************************************/
#include "lidar.h"
#include <atomic>
#include <exception>
#include <mutex>
#include <thread>
/******************************************************************************/

namespace Lidar {
	class SickLidar final : public Lidar {
	public:
		static const int VID = 0x19a2;
		static const int PID = 0x5001;

		explicit SickLidar(libusb_device_handle* hnd);

		~SickLidar() override;

		Time get_time_origin() override;

		FrameLidar get_frame() override;

	private:
		libusb_device_handle* _hDev;
		std::vector<uint8_t> _recv;
		int _cRecv;
		uint8_t* _cur;
		std::thread _acquisition_thread;
		std::mutex _acquisition_mutex;
		FrameLidar _lastTrame;
		std::atomic_bool _hasTrame = {false};
		std::exception_ptr _except;

	private:
		void readTelegram(FrameLidar& out);

		int readHex(); // lit un entier du flux.
		void acquire();
	};
} // namespace Lidar
