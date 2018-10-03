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

struct TrameLidar {
	// donnés dans le sens trigo
	std::vector<Distance> points;
	Time timestamp;
	Angle angularResolution;
	Angle begin, end;

	void saveToFile(std::string const& file);

	TrameLidar() = default;
	TrameLidar(std::string const& filename);
};

class Lidar {
public:
	virtual ~Lidar() = default;

	enum LidarType { Hokuyo, Sick, Any };

	// Tente d'ouvrir un des lidars supportés.
	static std::unique_ptr<Lidar> openLidar(LidarType lidar = LidarType::Any);

	virtual Time getTimeOrigin() = 0;
	virtual TrameLidar getTrame() = 0;
};
