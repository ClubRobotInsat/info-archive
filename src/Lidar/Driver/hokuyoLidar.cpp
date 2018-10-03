//
//  main.cpp
//  Club Robot
//
//  Created by Rémi on 24/02/2015.
//

#include "hokuyoLidar.h"
#include <log/Log.h>
#include <string.h>

namespace {
	uint32_t decodeThreeBytes(uint8_t const* val) {
		return ((val[0] - 0x30) << 12) | ((val[1] - 0x30) << 6) | ((val[2] - 0x30) << 0);
	}
	uint32_t decodeFourBytes(uint8_t const* val) {
		return ((val[0] - 0x30) << 18) | ((val[1] - 0x30) << 12) | ((val[2] - 0x30) << 6) | ((val[3] - 0x30) << 0);
	}
} // namespace

HokuyoLidar::HokuyoLidar(std::string const& portSerie) {
	try {
		_serial = std::make_unique<Commun::RS232>(portSerie.c_str());
	} catch(std::exception& e) {
		logError("Impossible d'ouvrir le port série demandé !");
		throw;
	}

	// logDebug("conn Série OK");

	// MonkeyPatch: s'assurer que le lidar a pu démarrer, sinon il ne traite pas
	// la trame 'départ' (il répond qu'il ne la pas traitée parce qu'il démarre,
	// mais la flemme de contrôler correctement)
	sleep(8_s);

	this->start();
	// this->getTrame();
}

HokuyoLidar::~HokuyoLidar() {
	// ne pas envoyer si on détruit ce lidar car la connexion a été perdue.
	// this->stop();
}

// attends et consomme un message ayant l'en-tête donné
static void waitAck(Commun::RS232& serial, const char* c) {
	// Wait for reply as a \n\n terminated string
	bool ok = false;
	while(!ok) {
		char c1 = serial.lireOctet();
		char c2 = serial.lireOctet();
		putchar(c1);
		putchar(c2);

		if(c1 == c[0])
			if(c2 == c[1])
				ok = true;
		for(;;) {
			if(serial.lireOctet() == '\n')
				if(serial.lireOctet() == '\n')
					break;
		}
	}
}

void HokuyoLidar::start() {
	uint8_t buf[100] = {0};
	std::string cmd = "BM;\n";
	_serial->ecrireOctets(reinterpret_cast<uint8_t const*>(cmd.data()), cmd.size());
	// logDebug("Sent BM");
	waitAck(*_serial, "BM");
	// logDebug("got ACK BM");

	_command = "MD";    // 3 bytes encoding
	_command += "0044"; // Start
	_command += "0725"; // End
	_command += "00";   // Cluster
	_command += "0";    // Scan Interval
	                    /*if(count > 10) {
	                        command += std::to_string(count); // Scan count
	                    }
	                    else {
	                        command += "0" + std::to_string(count); // Scan count
	                    }*/
	_command += "00";   // Endless frames
	_command += ";";    // End parameters
	_command += "";     // Custom string;
	_command += "\n";

	_serial->ecrireOctets(reinterpret_cast<uint8_t const*>(_command.data()), _command.size());
	// logDebug("Sent MD");
	waitAck(*_serial, "MD");
	// logDebug("Got ACK MD");
}

void HokuyoLidar::stop() {
	uint8_t buf[9] = {0};
	std::string cmd = "QT;\n";
	_serial->ecrireOctets(reinterpret_cast<uint8_t const*>(cmd.data()), cmd.size());
	_serial->lireOctets(buf, cmd.size() + 2 + 1 + 2);
}

Time HokuyoLidar::getTimeOrigin() {
	uint8_t buf[17] = {0};
	std::string cmd = "TM0;\n";
	_serial->ecrireOctets(reinterpret_cast<uint8_t const*>(cmd.data()), cmd.size());
	_serial->lireOctets(buf, cmd.size() + 5);
	cmd = "TM1;\n";
	_serial->ecrireOctets(reinterpret_cast<uint8_t const*>(cmd.data()), cmd.size());
	_serial->lireOctets(buf, cmd.size() + 3 + 1 + 4 + 3);
	cmd = "TM2;\n";
	_serial->ecrireOctets(reinterpret_cast<uint8_t const*>(cmd.data()), cmd.size());
	_serial->lireOctets(buf, cmd.size() + 5);

	return Duration::makeFromMs(decodeFourBytes(&buf[10]));
}

TrameLidar HokuyoLidar::getTrame() {
	// offset à confirmer, peut-être pas complètement juste.
	const Angle orig = -(384 - 45) * 0.36_deg;

	TrameLidar trame;
	trame.angularResolution = 0.36_deg;

	int count = 1;
	// int start = 128-44, end = 640-44; // 256 points de chaque coté
	const int start = 0;            // pour limiter le nb de points reportés à
	const int end = 725 - (44 - 1); // l'application.

	const int nbPoints = 725 - (44 - 1); // ne pas changer. nécessaire au décodage.

	trame.begin = start * trame.angularResolution + orig;
	trame.end = end * trame.angularResolution + orig;

	std::vector<Distance> mesures;
	mesures.resize((end - start + 1) * count);

	// Wait for a message of the expected type and status code, and skip its
	// header.
	for(;;) {
		char buf[4] = {};
		// printf("header:\n");
		_serial->lireOctets((std::uint8_t*)buf, 2);
		if(strncmp(buf, "MD", 2) == 0) {
			while(_serial->lireOctet() != '\n')
				;
			_serial->lireOctets((std::uint8_t*)buf, 3);
			if(strncmp(buf, "99b", 3) == 0) {
				while(_serial->lireOctet() != '\n')
					;
				while(_serial->lireOctet() != '\n')
					;
				break; // good header read, proceed with payload.
			}
			// else: scan for \n\n
		}
		// else
		// scan for \n\n
		for(;;) {
			if(_serial->lireOctet() == '\n')
				if(_serial->lireOctet() == '\n')
					break; // wait for one more header
		}
	}

	// Timestamp pas lu pour l'instant, il était dans le header.

	// Payload: 3 caractères par point, tout collé, puis découpé en lignes
	// de 64 octets.
	// printf("debut:\n");
	int nextPos = 0;
	uint8_t buf2[3];
	for(int i = 0; i < (nbPoints * 3); ++i) {
		if(i % 64 == 0 && i != 0) {
			_serial->lireOctet(); // checksum
			if(_serial->lireOctet() != '\n')
				printf("Misaligned read. Please debug Hokuyo driver.");
			// putchar('\n');
		}
		int c = _serial->lireOctet();
		buf2[nextPos++] = c;
		// putchar(c);
		if(nextPos == 3) {
			// printf("got point #%d\n", i/3);
			auto val = decodeThreeBytes(buf2);
			if((i / 3) >= start && (i / 3) <= end)
				mesures[(i / 3) - start] = Distance::makeFromMm(val);
			nextPos = 0;
		}
	}
	// printf("[end]");
	// FIN: fin de ligne anticipée (avec checksum), plus \n pour terminer la
	// trame.
	_serial->lireOctet();
	int c = _serial->lireOctet();
	// putchar(c);
	if(c != '\n')
		printf("Misaligned read. Please debug Hokuyo driver.");
	c = _serial->lireOctet();
	// putchar(c);
	if(c != '\n')
		printf("Misaligned read. Please debug Hokuyo driver.");
	// putchar('\n');

	trame.points = std::move(mesures);

	return trame;
}
