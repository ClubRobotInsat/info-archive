#pragma once
//
//  main.h
//  Club Robot
//
//  Created by Rémi on 24/02/2015.
//

#include <Units.h>
#include <memory>
#include <vector>

struct libusb_device_handle;

namespace Lidar {

	struct FrameLidar {
		// donnés dans le sens trigo
		std::vector<Distance> points;
		Time timestamp;
		Angle angular_resolution;
		Angle begin, end;

		void save_to_file(std::string const& file);

		FrameLidar() = default;

		explicit FrameLidar(std::string const& filename);
	};

	enum class Type { Hokuyo, Sick, Any };

	class Lidar {
	public:
		virtual ~Lidar() = default;

		// Tente d'ouvrir un des lidars supportés.
		static std::unique_ptr<Lidar> open_lidar(Type lidar = Type::Any);

		virtual Time get_time_origin() = 0;

		virtual FrameLidar get_frame() = 0;
	};
} // namespace Lidar