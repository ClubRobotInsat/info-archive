#ifndef _SERIAL_H_
#define _SERIAL_H_

#include <Commun.h>

namespace Communication {

	EXCEPTION_CLASS(ErreurEOF)

	/// Fournit une interface générique de communication série
	class Serial {
	public:
		/// Permet une gestion correcte de la suppression d'instances polymorphes des classes dérivées
		virtual ~Serial() = default;

		/// Ecrit un octet sur le port
		virtual void write_byte(uint8_t byte) final;

		/// Ecrit plusieurs octets sur le port
		virtual void write_bytes(const uint8_t* bytes, std::size_t bytes_number) = 0;

		/// Attend un octet sur le port et retourne le premier arrivé - BLOQUANT
		virtual uint8_t read_byte() final;

		/// Attend plusieurs octets sur le port et retourne lorsque le nombre demandé a été reçu - BLOQUANT
		virtual void read_bytes(uint8_t* octets, std::size_t nombre) = 0;
	};
} // namespace Communication

#endif
