#include "TCPIP.h"
#include <cstdlib>
#include <cstring>
#include <log/Log.h>

namespace Commun {

	/// Ouvre la connexion
	TCPIP::TCPIP(std::string const& adresse, uint16_t port) : _connecte(false) {
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

		hostinfo = gethostbyname(adresse.c_str());
		if(hostinfo == nullptr) {
			logError("Impossible de trouver l'adresse du serveur '", adresse, "'\n");
			exit(EXIT_FAILURE);
		}

		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr = *(reinterpret_cast<struct in_addr*>(hostinfo->h_addr));
		memset(&(addr.sin_zero), 0, 8);

		if(connect(_fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(struct sockaddr_in)) < 0) {
			logError("Impossible de se connecter au serveur ", adresse, ":", port);
			throw(34);
		}

		_connecte = true;
	}

	/// Ferme la connexion
	TCPIP::~TCPIP() {
		fermerSocket();
	}

	/// Ecrit plusieurs octets sur le port
	void TCPIP::ecrireOctets(unsigned char const* octets, std::size_t nombre) {
		if(_connecte) {
#ifdef WIN32
			int flags = 0;
#elif __APPLE__
			int flags = SO_NOSIGPIPE;
#else
			int flags = MSG_NOSIGNAL;
#endif
			if(send(_fd, reinterpret_cast<const char*>(octets), nombre, flags) < static_cast<ssize_t>(nombre))
				fermerSocket();
		}
	}

	/// Attend plusieurs octets sur le port et retourne lorsque le nombre demandé a été reçu - BLOQUANT
	void TCPIP::lireOctets(uint8_t* octets, std::size_t nombre) {
		if(_connecte) {
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
			if(recv(_fd, reinterpret_cast<char*>(octets), nombre, MSG_WAITALL) < static_cast<ssize_t>(nombre))
				fermerSocket();
#endif
		}
	}

	/// Indique si la liaison TCP/IP est connectée ou pas
	bool TCPIP::estConnecte() {
		return _connecte;
	}

	// Ferme le socket
	void TCPIP::fermerSocket() {
		if(_connecte) {
			logInfo("Fermeture du socket\n");

			shutdown(_fd, 2);
#ifdef WIN32
			closesocket(_fd);
			WSACleanup();
#else
			close(_fd);
#endif
		}

		_connecte = false;
	}
} // namespace Commun
