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

#include "../../commun/Commun.h"

#include "Serie.h"

namespace Commun {

	/// Interface de communication série via socket TCP/IP
	class TCPIP : public Serie {
	public:
		/// Ouvre la connexion
		TCPIP(std::string const& adresse, uint16_t port);

		/// Ferme la connexion
		~TCPIP();

		/// Ecrit plusieurs octets sur la liaison
		virtual void ecrireOctets(uint8_t const* octets, std::size_t nombre) override;

		/// Attend plusieurs octets sur la liaison et retourne lorsque le nombre demandé a été reçu - BLOQUANT
		virtual void lireOctets(uint8_t* octets, std::size_t nombre) override;

		/// Indique si la liaison TCP/IP est connectée ou pas
		bool estConnecte();

	private:
/// Descripteur du socket TCP/IP
#ifdef WIN32
		SOCKET _fd;
#else
		int _fd;
#endif

		/// Indique si la liaison TCP/IP est connectée ou pas
		bool _connecte;

		// Ferme le socket
		void fermerSocket();
	};
}

#endif
