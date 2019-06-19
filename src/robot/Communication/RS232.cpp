#include "RS232.h"
#include <Clock.h>
#include <Units/Units.h>
#include <log/Log.h>
#include <string.h>
#include <unistd.h>

#ifndef WIN32
#include <fcntl.h>
#include <termios.h>
#endif

namespace Communication {

	/// Ouvre le port série indiqué et le configure
	RS232::RS232(const std::string& port) {
#ifdef WIN32

		_fd = CreateFile(port.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, nullptr); // RW, lecture non-bloquante
		if(_fd == INVALID_HANDLE_VALUE) {
			logError("Impossible d'ouvrir le port RS232 '", port, "'\n");
			exit(EXIT_FAILURE);
		}

		PurgeComm(_fd, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
		DCB params;
		params.DCBlength = sizeof(DCB);
		if(!getCommState(_fd, &params)) {
			logError("Impossible d'obtenir la configuration du port RS232 '", port, "'");
			exit(EXIT_FAILURE);
		}

		params.BaudRate = 115200;     // 115200 bauds
		params.ByteSize = 8;          // char de 8 bits
		params.StopBits = ONESTOPBIT; // 1 bit de stop
		params.Parity = NOPARITY;     // pas de bit de parité
		if(!SetCommState(_fd, &params)) {
			logError("Impossible de modifier la configuration du port RS232 '", port, "'");
			exit(EXIT_FAILURE);
		}

		memset(&lecture, 0, sizeof(lecture));
		memset(&ecriture, 0, sizeof(ecriture));

		lecture.hEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr); // évènement asynchrone "1 octet lu" auto-réinitialisé
		ecriture.hEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr); // évènement asynchrone "1 octet lu" auto-réinitialisé

#else // defined WIN32

		// On met le flag O_NDELAY pour que l'ouverture du fd se fasse sans bloquer.
		//_fd = open(port.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
		_fd = open(port.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);

		if(_fd < 0) {
			// logError("Impossible d'ouvrir le port RS232 '", port, "'");
			throw std::runtime_error("Impossible d'ouvrir le port !");
		}

		tcflush(_fd, TCIOFLUSH);
		termios params{};
		memset(&params, 0, sizeof(termios));

#ifdef __APPLE__

		tcgetattr(_fd, &params);
		cfsetispeed(&params, 115200);
		cfsetospeed(&params, 115200);
		params.c_cflag &= ~PARENB;
		params.c_cflag &= ~CSTOPB;
		params.c_cflag &= ~CSIZE;
		params.c_cflag |= CS8;
		params.c_cflag |= (CLOCAL | CREAD);

#else // defined __APPLE__

		params.c_cflag = B115200 | CS8 | CLOCAL |
		                 CREAD; // 115200 bauds, char de 8 bits, ignorer signaux de contrôle, lecture/écriture, 1 bit de stop

#endif // defined __APPLE__

		params.c_iflag = IGNPAR; // pas de bit de parité
		params.c_cc[VMIN] = 1;   // lecture bloquante jusqu'à recevoir 1 octet

		if(tcsetattr(_fd, TCSANOW, &params) == -1) {
			logError("Impossible de modifier la configuration du port RS232 '", port, "'");
			exit(EXIT_FAILURE);
		}

		// On supprime le flag O_NDELAY pour qu'un appel à read soit bloquant tant que la carte CAN USB n'a pas envoyé
		// d'octet.
		int saved_flags = fcntl(_fd, F_GETFL);
		fcntl(_fd, F_SETFL, saved_flags & ~O_NDELAY);

#endif // defined WIN32
	}

	/// Ferme le port
	RS232::~RS232() {
#ifdef WIN32

		CloseHandle(_fd);
		CloseHandle(_lecture.hEvent);
		CloseHandle(_ecriture.hEvent);

#else

		close(_fd);

#endif
	}

	/// Ecrit plusieurs octets sur le port
	void RS232::write_bytes(const uint8_t* bytes, std::size_t bytes_number) {
#ifdef WIN32

		DWORD dummy;
		if(!WriteFile(_fd, octets, nombre, &dummy, &ecriture) &&
		   getLastError() == ERROR_IO_PENDING) // Si l'écriture d'un octet ne retourne pas immédiatement parce qu'elle
		                                       // est en attente... {
			getOverlappedResult(_fd, &ecriture, &dummy, TRUE); // ...alors on attend !

#else

		ssize_t val = write(_fd, bytes, bytes_number);
		if(val == -1) {
			logError("Erreur dans write : ");
			perror("Erreur retournée");
		}

#endif
	}

	/// Attend plusieurs octets sur le port et retourne lorsque le nombre demandé a été reçu - BLOQUANT
	size_t RS232::read_bytes(uint8_t* bytes, std::size_t bytes_number) {
#ifdef WIN32

		DWORD dummy;
		if(!ReadFile(_fd, octets, nombre, &dummy, &_lecture) &&
		   getLastError() == ERROR_IO_PENDING) // Si la lecture d'un octet ne retourne pas immédiatement parce qu'aucun
		                                       // n'est encore arrivé... {
			getOverlappedResult(_fd, &_lecture, &dummy, TRUE); // ...alors on attend l'arrivée d'un octet !

#else

		std::size_t nb_read = 0;
		while(nb_read < bytes_number) {
			ssize_t val = read(_fd, bytes + nb_read, bytes_number - nb_read);
			if(val == -1) {
				logError("Erreur dans read : ");
				perror("Erreur retournée");
				sleep(1_s);
			}
			if(val == 0)
				throw ErreurEOF();
			nb_read += val;
		}

#endif
		return bytes_number;
	}

} // namespace Communication
