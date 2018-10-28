#include "TCPIP.h"
#include <cstdlib>
#include <cstring>
#include <log/Log.h>

namespace Communication {

	/// Ouvre la connexion
	TCPIP::TCPIP(std::string const& address, uint16_t port) : _connected(false) {
#ifdef WIN32
		WSAData data;
		WSAStartup(MAKEWORD(2, 2), &data);
#endif

		_fd = socket(AF_INET, SOCK_STREAM, 0); // Domaine Internet, par flux, protocole TCP

#ifdef WIN32
		if(_fd == INVALID_SOCKET)
#else
		if(_fd < 0)
#endif
		{
			logError("Impossible de creer le socket\n");
			exit(EXIT_FAILURE);
		}

		sockaddr_in addr;
		hostent* hostinfo;

		hostinfo = gethostbyname(address.c_str());
		if(hostinfo == nullptr) {
			logError("Impossible de trouver l'adresse du serveur '", address, "'\n");
			exit(EXIT_FAILURE);
		}

		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr = *(reinterpret_cast<struct in_addr*>(hostinfo->h_addr));
		memset(&(addr.sin_zero), 0, 8);

		if(connect(_fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(struct sockaddr_in)) < 0) {
			logError("Impossible de se connecter au serveur ", address, ":", port);
			throw(34);
		}

		_connected = true;
	}

	/// Ferme la connexion
	TCPIP::~TCPIP() {
		close_socket();
	}

	/// Ecrit plusieurs octets sur le port
	void TCPIP::write_bytes(unsigned char const* octets, std::size_t nombre) {
		if(_connected) {
#ifdef WIN32
			int flags = 0;
#elif __APPLE__
			int flags = SO_NOSIGPIPE;
#else
			int flags = MSG_NOSIGNAL;
#endif
			if(send(_fd, reinterpret_cast<const char*>(octets), nombre, flags) < static_cast<ssize_t>(nombre))
				close_socket();
		}
	}

	/// Attend plusieurs octets sur le port et retourne lorsque le nombre demandé a été reçu - BLOQUANT
	void TCPIP::read_bytes(uint8_t* bytes, std::size_t bytes_number) {
		if(_connected) {
#ifdef WIN32 // MSG_WAITALL pas avant Win Server 2003 (donc pas sous XP par exemple) : on fait sans
			unsigned char recu;
			unsigned char lus = 0;
			while(lus < nombre) {
				recu = recv(_fd, (char*)(octets + lus), nombre - lus, 0);
				if(recu <= 0)
					FermerSocket();
				else
					lus += recu;
			}
#else
			if(recv(_fd, reinterpret_cast<char*>(bytes), bytes_number, MSG_WAITALL) < static_cast<ssize_t>(bytes_number))
				close_socket();
#endif
		}
	}

	/// Indique si la liaison TCP/IP est connectée ou pas
	bool TCPIP::is_connected() {
		return _connected;
	}

	// Ferme le socket
	void TCPIP::close_socket() {
		if(_connected) {
			logInfo("Fermeture du socket\n");

			shutdown(_fd, 2);
#ifdef WIN32
			closesocket(_fd);
			WSACleanup();
#else
			close(_fd);
#endif
		}

		_connected = false;
	}
} // namespace Communication
