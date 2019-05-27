#ifndef _RS232_H_
#define _RS232_H_

#ifdef WIN32
#include <windows.h>
#endif

#include "Serial.h"
#include <string>

namespace Communication {

	/// Interface de communication série via port RS232
	class RS232 : public Serial {
	public:
		/// Ouvre le port série indiqué et le configure
		explicit RS232(const std::string& port);

		/// Ferme le port
		~RS232() override;

		/// Ecrit plusieurs octets sur le port
		void write_bytes(const uint8_t* bytes, std::size_t bytes_number) override;

		/// Attend plusieurs octets sur le port et retourne lorsque le nombre demandé a été reçu - BLOQUANT
		size_t read_bytes(uint8_t* bytes, std::size_t bytes_number) override;

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

} // namespace Communication

#endif
