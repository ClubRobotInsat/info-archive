#ifndef _SERIE_H_
#define _SERIE_H_

#include <Commun.h>

namespace Commun {

	EXCEPTION_CLASS(ErreurEOF);

	/// Fournit une interface générique de communication série
	class Serie {
	public:
		/// Permet une gestion correcte de la suppression d'instances polymorphes des classes dérivées
		virtual ~Serie() {}

		/// Ecrit un octet sur le port
		virtual void ecrireOctet(uint8_t octet) final;

		/// Ecrit plusieurs octets sur le port
		virtual void ecrireOctets(uint8_t const* octets, std::size_t nombre) = 0;

		/// Attend un octet sur le port et retourne le premier arrivé - BLOQUANT
		virtual uint8_t lireOctet() final;

		/// Attend plusieurs octets sur le port et retourne lorsque le nombre demandé a été reçu - BLOQUANT
		virtual void lireOctets(uint8_t* octets, std::size_t nombre) = 0;
	};
}

#endif
