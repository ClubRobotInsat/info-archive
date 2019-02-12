//
//  main.cpp
//  Club Robot
//
//  Created by Rémi on 24/02/2015.
//

#include "lidar.h"
#include "hokuyoLidar.h"
#include "sickLidar.h"
#include <fstream>


void FrameLidar::save_to_file(std::string const& file) {
	std::ofstream out(file);

	out << (timestamp * 1e6).toS<uint32_t>() << '\n';
	out << (angularResolution * 1e6).toRad<uint32_t>() << '\n';
	out << (begin * 1e6).toRad<int32_t>() << '\n';
	out << (end * 1e6).toRad<int32_t>() << '\n';
	out << points.size() << '\n';

	for(auto const& d : points) {
		out << d.toMm<std::uint32_t>() << '\n';
	}
}

FrameLidar::FrameLidar(std::string const& filename) {
	std::ifstream in(filename);
	uint32_t tmp;
	int32_t stmp;

	in >> tmp;
	timestamp = Time::makeFromUs(tmp);
	in >> tmp;
	angularResolution = Angle::makeFromMilliRad(tmp) / 1000;
	in >> stmp;
	begin = Angle::makeFromMilliRad(tmp) / 1000;
	in >> stmp;
	end = Angle::makeFromMilliRad(tmp) / 1000;

	in >> tmp;
	points.resize(tmp);
	for(auto& d : points) {
		in >> tmp;
		d = Distance::makeFromMm(tmp);
	}
}

#define EP_FROMLIDAR 0x81
#define EP_TOLIDAR 0x02

class LibUsbCtx {
public:
	LibUsbCtx() {
		if(libusb_init(nullptr))
			throw std::runtime_error("at libusb_init");
	}
	~LibUsbCtx() {
		libusb_exit(nullptr);
	}
};

namespace {
	LibUsbCtx const usbContext;
}

std::unique_ptr<Lidar> Lidar::open_lidar(LidarType lidarType) {
	if(lidarType == Lidar::None) {
		return nullptr;
	}

	// en énumérant à la main, on a un meilleur diagnostic qu'avec l'utilitaire
	// libusb openByVidPid.

	libusb_device** devices;
	ssize_t cUsbDevs = libusb_get_device_list(nullptr, &devices);

	libusb_device* sick = nullptr;
	for(int i = 0; i < cUsbDevs; i++) {
		libusb_device_descriptor desc;
		libusb_get_device_descriptor(devices[i], &desc);

		if(desc.idVendor == SickLidar::VID && desc.idProduct == SickLidar::PID) {
			sick = devices[i];
			break;
		}
	}

	if(sick && lidarType != LidarType::Hokuyo) {
		libusb_device_handle* hnd;
		int rc;
		if(sick)
			rc = libusb_open(sick, &hnd);

		libusb_free_device_list(devices, 1);

		if(rc == LIBUSB_ERROR_ACCESS)
			throw std::runtime_error("Permissions insuffisantes pour accéder au périphérique.");
		if(!hnd)
			throw std::runtime_error("Erreur inconnue.");

		return std::make_unique<SickLidar>(hnd);
	} else if(lidarType != LidarType::Sick) {
// peut apparaitre sous plusieurs noms. on essaie des noms connus dans
// l'ordre
#define TRY(name)                                   \
	try {                                           \
		return std::make_unique<HokuyoLidar>(name); \
	} catch(...) {                                  \
	}
		TRY("/dev/ttyACM0");
		TRY("/dev/ttyACM1");
		TRY("/dev/tty.usbmodem1451");
		TRY("/dev/tty.usbmodem1411");
#undef TRY

		throw std::runtime_error("Lidar Hokuyo non trouvé");
	}

	return {};
}

/*void Lidar::sendData(uint8_t const *data, size_t dataLength) {
    size_t sent = 0;
    int w;

    do {
        int rc = libusb_bulk_transfer(_hDev, EP_TOLIDAR, const_cast<uint8_t *>(data) + sent, dataLength - sent, &w, 0);
        if(rc)
            throw Erreur("USB Write failed");
        sent += w;
    } while(sent < dataLength);
}

void Lidar::receiveData(uint8_t *buf, size_t wanted) {
    size_t received = 0;
    int got;

    do {
        int rc = libusb_bulk_transfer(_hDev, EP_FROMLIDAR, &buf[received], wanted - received, &got, 0);
        if(rc)
            throw Erreur("USB read failed !");
        received += got;
    } while(received < wanted);
}*/
