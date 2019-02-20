//
// Created by terae on 07/11/18.
//

#ifndef ROOT_UDP_H
#define ROOT_UDP_H

// Utilisation de `asio` sans `Boost.asio`
#define ASIO_STANDALONE

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#include <asio.hpp>
#pragma GCC diagnostic pop

#include "Serial.h"

namespace Communication {
	/// Interface de communication série via datagramme UDP
	class UDP : public Serial {
	public:
		/// Ouvre la connexion
		UDP(const std::string& address, uint16_t local_port, uint16_t remote_port);

		/// Ferme la connexion
		~UDP() override;

		/// Écrit plusieurs octets sur la liaison
		void write_bytes(const uint8_t* bytes, std::size_t bytes_number) override;

		/// Attend plusieurs octets sur la liaison et retourne lorsque le nombre demandé a été reçu - BLOQUANT
		size_t read_bytes(uint8_t* bytes, std::size_t bytes_number) override;

		/// Indique si la liaison UDP est connectée ou pas
		bool is_connected();

		// Exception si la mise en place de la communication UDP a un problème
		EXCEPTION_CLASS(ErrorUDPCommunication);

	private:
		std::unique_ptr<asio::ip::udp::socket> _send_socket;
		asio::ip::udp::endpoint _send_remote_endpoint;

		std::unique_ptr<asio::ip::udp::socket> _recv_socket;
		asio::ip::udp::endpoint _recv_remote_endpoint;

		/// Indique si la liaison TCP/IP est connectée ou pas
		bool _connected;

		// Ferme le socket
		void close_socket();
	};
} // namespace Communication

#endif // ROOT_UDP_H
