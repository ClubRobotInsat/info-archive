#ifndef _RS232_H_
#define _RS232_H_

#ifdef WIN32
#include <windows.h>
#endif

#include "Serie.h"
#include <string>

namespace Commun {

	/// Interface de communication série via port RS232
	class RS232 : public Serie {
	public:
		/// Ouvre le port série indiqué et le configure
		RS232(std::string const& port);

		/// Ferme le port
		~RS232();

		/// Ecrit plusieurs octets sur le port
		virtual void ecrireOctets(uint8_t const* octets, std::size_t nombre) override;

		/// Attend plusieurs octets sur le port et retourne lorsque le nombre demandé a été reçu - BLOQUANT
		virtual void lireOctets(uint8_t* octets, std::size_t nombre) override;

	private:
#ifdef WIN32
		/// Descripteur du port série
		HANDLE _fd;
		/// Structure d'E/S asynchrone
		OVERLAPPED _lecture;
		OVERLAPPED _ecriture;
#else
		/// Descripteur du port série
		int _fd;
#endif
	};
}

#endif
