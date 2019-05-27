//
// Created by terae on 6/24/18.
//

#ifndef ROOT_NAMEDPIPE_H
#define ROOT_NAMEDPIPE_H

#include "Commun.h"
#include "Serial.h"

#include <fcntl.h>

namespace Communication {

	class NamedPipe : public Serial {
	public:
		/// Crée et configure les descripteurs de communication
		NamedPipe(const std::string& path_read, const std::string& path_write);
		~NamedPipe() override;

		void write_bytes(const uint8_t* bytes, std::size_t bytes_number) override;

		size_t read_bytes(uint8_t* bytes, std::size_t bytes_number) override;

		// erreur si la connection n'est pas ouverte
		EXCEPTION_CLASS(ErrorPipeOpening);

	private:
		/// Descripteur de communication (FIFO)
		int _fd_read;
		int _fd_write;

		std::string _path_read;
		std::string _path_write;

		bool create_descriptor(const std::string& path);
	};

} // namespace Communication

#endif // ROOT_NAMEDPIPE_H
