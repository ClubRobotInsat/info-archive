//
// Created by terae on 6/24/18.
//

#ifndef ROOT_NAMEDPIPE_H
#define ROOT_NAMEDPIPE_H

#include "Commun.h"

#include "Serie.h"

#include <fcntl.h>

namespace Communication {
	class NamedPipe : public Serie {
	public:
		/// Crée et configure les descripteurs de communication
		NamedPipe(const std::string& pathRead, const std::string& pathWrite);
		~NamedPipe() override;

		void ecrireOctets(uint8_t const*, std::size_t) override;

		void lireOctets(uint8_t* octets, std::size_t nombre) override;

		// erreur si la connection n'est pas ouverte
		class ErreurOuverturePipe : public std::runtime_error {
		public:
			explicit ErreurOuverturePipe(std::string msg) : std::runtime_error(msg) {}
		};

	private:
		/// Descripteur de communication (FIFO)
		int _fd_read;
		int _fd_write;

		std::string _path_read;
		std::string _path_write;

		bool creerDescripteur(const std::string path);
	};
} // namespace Communication

#endif // ROOT_NAMEDPIPE_H
