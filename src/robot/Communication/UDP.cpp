//
// Created by terae on 07/11/18.
//

#include "UDP.h"

#include <arpa/inet.h> // inet_addr
#include <cstring>
#include <netdb.h>      // hostent
#include <netinet/in.h> // sockaddr_in
#include <sys/socket.h> // socket
#include <unistd.h>

#include <log/Log.h>

void fail_with(const std::string& msg) {
	logError(msg);
	exit(EXIT_FAILURE);
}

namespace Communication {
	sockaddr_in addr{};
	hostent* host_info;

	UDP::UDP(const std::string& address, uint16_t port) {
		// Construction du socket associé à la communication
		_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // Domaine Internet, datagrammes UDP
		if(_fd < 0) {
			fail_with("Could not create socket.");
		}

		// Assignation des valeurs locales
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		memset(&(addr.sin_zero), 0, 8);

		// Binding de la connection
		if(bind(_fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(struct sockaddr_in)) < 0) {
			fail_with("Binding error to " + address + ":" + std::to_string(port));
		}

		// Obtention de l'adresse IP distante
		host_info = gethostbyname(address.c_str());
		if(host_info == nullptr) {
			fail_with("Impossible to get the address of '" + address + "'.");
		}

		// Assignation de l'addresse distante
		addr.sin_addr = *(reinterpret_cast<struct in_addr*>(host_info->h_addr));

		if(connect(_fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(struct sockaddr_in)) < 0) {
			fail_with("Connection error to " + address + ":" + std::to_string(port));
		}
	}

	void UDP::write_bytes(const uint8_t* bytes, std::size_t bytes_number) {
		if(_connected) {
			if(send(_fd, reinterpret_cast<const char*>(bytes), bytes_number, 0) < static_cast<ssize_t>(bytes_number)) {
				close_socket();
			}
		}
	}

	void UDP::read_bytes(uint8_t* bytes, std::size_t bytes_number) {
		if(recv(_fd, reinterpret_cast<char*>(bytes), bytes_number, 0) < static_cast<ssize_t>(bytes_number)) {
			close_socket();
		}
	}

	UDP::~UDP() {
		close_socket();
	}

	bool UDP::is_connected() {
		return _connected;
	}

	void UDP::close_socket() {
		if(is_connected()) {
			close(_fd);
			logInfo("Socket closure.");
			_connected = false;
		}
	}
} // namespace Communication


#include <cstdlib>
#include <cstring>
#include <log/Log.h>

namespace Communication {

	/// Ouvre la connexion
	TCPIP::TCPIP(std::string const& address, uint16_t port) : _connected(false) {

		sockaddr_in addr{};
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
* /