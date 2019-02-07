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

struct FrameLidar {
	// donnés dans le sens trigo
	std::vector<Distance> points;
	Time timestamp;
	Angle angularResolution;
	Angle begin, end;

	void save_to_file(std::string const& file);

	FrameLidar() = default;
	FrameLidar(std::string const& filename);
};

class Lidar {
public:
	enum LidarType { Hokuyo, Sick, Any, None };

	Lidar(LidarType t) : type(t) {}
	virtual ~Lidar() = default;

	// Tente d'ouvrir un des lidars supportés.
	static std::unique_ptr<Lidar> open_lidar(LidarType lidar = LidarType::Any);

	virtual Time get_time_origin() = 0;
	virtual FrameLidar get_frame() = 0;

	const LidarType type;
};

inline std::ostream& operator<<(std::ostream& os, Lidar::LidarType type) {
	switch(type) {
		case Lidar::Hokuyo:
			return os << "Hokuyo";
		case Lidar::Sick:
			return os << "Sick";
		case Lidar::Any:
			return os << "Any";
		case Lidar::None:
			return os << "None";
		default:
			return os;
	}
}
