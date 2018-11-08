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
		addr.sin_addr.s_addr = INADDR_ANY; // inet_addr("127.0.0.1");
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
		_connected = true;
	}

	void UDP::write_bytes(const uint8_t* bytes, std::size_t bytes_number) {
		if(_connected) {
			if(send(_fd, reinterpret_cast<const char*>(bytes), bytes_number, 0) < static_cast<ssize_t>(bytes_number)) {
				close_socket();
			}
		}
	}

	void UDP::read_bytes(uint8_t* bytes, std::size_t bytes_number) {
		// recvfrom : on peut récupérer le port de l'envoyeur pour lui répondre ensuite (utilisation du même socket envoi & réception)
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
			// logInfo("Socket closure.");
			_connected = false;
		}
	}
} // namespace Communication
