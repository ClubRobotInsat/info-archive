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
	virtual ~Lidar() = default;

	enum LidarType { Hokuyo, Sick, Any, None };

	// Tente d'ouvrir un des lidars supportés.
	static std::unique_ptr<Lidar> open_lidar(LidarType lidar = LidarType::Any);

	virtual Time get_time_origin() = 0;
	virtual FrameLidar get_frame() = 0;
};
