#ifndef _TCPIP_H_
#define _TCPIP_H_

#ifdef WIN32
#include <windows.h>
#include <winsock2.h>
#else
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

#include "Commun.h"

#include "Serial.h"

namespace Communication {

	/// Interface de communication série via socket TCP/IP
	class TCPIP : public Serial {
	public:
		/// Ouvre la connexion
		TCPIP(std::string const& address, uint16_t port);

		/// Ferme la connexion
		~TCPIP() override;

		/// Ecrit plusieurs octets sur la liaison
		void write_bytes(const uint8_t* bytes, std::size_t bytes_number) override;

		/// Attend plusieurs octets sur la liaison et retourne lorsque le nombre demandé a été reçu - BLOQUANT
		size_t read_bytes(uint8_t* bytes, std::size_t bytes_number) override;

		/// Indique si la liaison TCP/IP est connectée ou pas
		bool is_connected();

	private:
/// Descripteur du socket TCP/IP
#ifdef WIN32
		SOCKET _fd;
#else
		int _fd;
#endif

		/// Indique si la liaison TCP/IP est connectée ou pas
		bool _connected;

		// Ferme le socket
		void close_socket();
	};
} // namespace Communication

#endif
