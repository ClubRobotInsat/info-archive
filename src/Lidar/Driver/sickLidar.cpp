/******************************************************************************/
/* robot/Lidar/Driver/sickLidar.cpp                                2015-03-03 */
/* Club Robot INSA Toulouse                                      Félix Poisot */
/******************************************************************************/
#include "sickLidar.h"
#include <Commun.h>
#include <libusb-1.0/libusb.h>
#include <log/Log.h>
#include <string.h>
#include <vector>
/******************************************************************************/

#define EP_FROMLIDAR 0x81
#define EP_TOLIDAR 0x02

typedef unsigned char byte;

SickLidar::SickLidar(libusb_device_handle* hnd) : Lidar(Lidar::Sick), _hDev(hnd), _recv(2048) {
	libusb_claim_interface(_hDev, 0);
	/* Démarre le mode transmission continue */
	const std::string go("\2sEN LMDscandata 1\3");
	int w, rc;
	rc = libusb_bulk_transfer(_hDev, EP_TOLIDAR, (unsigned char*)go.c_str(), static_cast<int>(go.size()), &w, 0);
	if(rc) {
		throw std::runtime_error("USB Write failed");
	}

	_acquisition_thread = std::thread([this]() { this->acquire(); });
}

SickLidar::~SickLidar() {
	libusb_close(_hDev);
	if(_acquisition_thread.joinable()) {
		_acquisition_thread.join();
	}
	// libusb_release_interface(_hDev, 0);
}

FrameLidar SickLidar::get_frame() {
	if(_except) {
		std::rethrow_exception(_except);
	}
	while(!_hasTrame) {
		sleep(1_s / 15);
	}

	std::lock_guard<std::mutex> lk(_acquisition_mutex);
	return _lastTrame;
}

void SickLidar::acquire() {
	try {
		while(true) {
			FrameLidar res;
			// Suffit d'attendre, les mesures arrivent en continu.

			int rc;

			for(int retry = 3; retry > 0; --retry) {
				try {
					for(;;) {
						rc = libusb_bulk_transfer(_hDev, EP_FROMLIDAR, _recv.data(), static_cast<int>(_recv.size()), &_cRecv, 0);
						if(rc) {
							throw std::runtime_error("USB read failed !");
						}

						_cur = _recv.data();
						if(_cRecv < 5) {
							throw std::runtime_error("Lidar Sick: Trame invalide (trop courte)");
						}
						if(strncmp(reinterpret_cast<const char*>(_cur), "\2sSN ", 5) == 0) {
							break; // un seul type de trame compris.
						}
					}

					// trame de type sSN
					_cur += 5;
					readTelegram(res);

				} catch(std::runtime_error& e) {
					logDebug(e.what());
				}
			}
		}
		throw std::runtime_error("Connexion Lidar Sick perdue.");
	} catch(std::exception& e) {
		_except = std::current_exception();
	}
}

Time SickLidar::get_time_origin() {
	return {};
}

#define skip(nb)                  \
	for(int i = 0; i < nb; i++) { \
		readHex();                \
	}

void SickLidar::readTelegram(FrameLidar& out) {
	std::lock_guard<std::mutex> lk(_acquisition_mutex);
	// assume qu'on vient de lire sSN
	_cur += 12; // 11+espace

	// Lire la doc: les champ "uint_x : 2x2" compte pour deux lectures.
	// Les champs grisés ne font pas partie des trames.

	skip(7);
	out.timestamp = Duration::makeFromUs(readHex());
	skip(10); // jusqu'à "# of 16bit channels" (inclus)

	float scale;               // observed to stay at 1.0
	int startAngle, angleStep; // in 10,000th deg.
	int sampleCount;
	_cur += 6; // string "DIST1 "
	*(int*)&scale = readHex();
	skip(1); // offset always zero
	startAngle = readHex();
	angleStep = readHex();
	sampleCount = readHex();

	out.points.resize(static_cast<size_t>(sampleCount));
	out.angularResolution = Angle::makeFromDeg(angleStep / 10000.0);
	out.begin = Angle::makeFromDeg(startAngle / 10000.0);
	out.end = Angle::makeFromDeg((startAngle + angleStep * sampleCount) / 10000.0);

	// convention, convention
	out.begin -= 90_deg;
	out.end -= 90_deg;

	for(auto& d : out.points) {
		d = Distance::makeFromMm(readHex() * scale);
	}

	// Les autres champs ne sont pas utilisés
	_lastTrame = out;
	_hasTrame = true;
}

int SickLidar::readHex() {
	int acc = 0;
	int c;
	for(;;) {
		if((_cur - _recv.data()) >= _cRecv) {
			throw std::runtime_error("Lidar Sick: Trame corrompue (tronquée)");
		}
		c = *_cur;
		if(c == ' ') {
			break;
		}

		acc <<= 4;
		if('0' <= c && c <= '9') {
			acc += c - '0';
		} else if('A' <= c && c <= 'F') {
			acc += (c - 'A') + 10;
		} else {
			throw std::runtime_error("Lidar Sick: Trame Corrompue (Caractère inattendu)");
		}

		_cur++;
	}
	_cur++; // skip space
	return acc;
}
