//
// Created by terae on 11/11/18.
//

#ifndef ROOT_PROTOCOLPARSER_H
#define ROOT_PROTOCOLPARSER_H

#include <functional>
#include <string>
#include <vector>

// Connexions séries disponibles
#include "NamedPipe.h"
#include "NullCommunicator.h"
#include "RS232.h"
#include "Serial.h"
#include "TCPIP.h"
#include "UDP.h"

#include <Constants.h>
#include <log/Log.h>

namespace Communication {
	enum SerialProtocolType {
		// Communications séries en one-to-one, avec un header `0xAC DC AB BA` devant les trames
		SERIAL_LOCAL,
		SERIAL_NULL,
		SERIAL_PIPES,
		SERIAL_RS232,
		SERIAL_TCPIP,
		SERIAL_UDP,
	};

	enum MultiSerialProtocolType {
		// Communication sur un réseau IP en UDP, la couche 3 s'occupe de l'addressage des cartes
		ETHERNET,
	};

	class Protocol {
	public:
		virtual ~Protocol() = default;

		virtual void send_frame(const GlobalFrame& f) = 0;

		virtual void recv_frame(const std::atomic_bool& running_execution,
		                        const std::function<void(const GlobalFrame&)>& handler) = 0;

		/// Mode débug actif ou non
		bool debug_active;
	};

	template <SerialProtocolType>
	class AbstractSerialProtocol : public Protocol {
		/// Mutex permettant d'assurer la serialisation correcte des donnees
		std::mutex _mutex_write;

	protected:
		AbstractSerialProtocol();

		explicit AbstractSerialProtocol(std::shared_ptr<Serial> serial);

		std::shared_ptr<Serial> _serial;

		Duration _delay;

		/// Valeurs constantes de certains octets des trames
		enum {
			BYTE_BEGIN_FRAME_1 = 0xac,
			BYTE_BEGIN_FRAME_2 = 0xdc,
			BYTE_BEGIN_FRAME_3 = 0xab,
			BYTE_BEGIN_FRAME_4_NORMAL = 0xba,
			BYTE_BEGIN_FRAME_4_PING = 0xbb,
		};

		virtual GlobalFrame recv_frame_blocking(const std::atomic_bool& running_execution);

		// Header de la communication série : `0xAC DC AB BA` par exemple
		virtual GlobalFrame generate_header(const GlobalFrame& body) const;

	public:
		~AbstractSerialProtocol() override;

		EXCEPTION_CLASS(ReceptionAborted);

		const SerialProtocolType protocol;

		void set_delay(Duration delay);

		void tempo_delay() const;

		void send_frame(const GlobalFrame& f) final;

		/// @throws `ReceptionAbort` at the end of the communication
		void recv_frame(const std::atomic_bool& running_execution, const std::function<void(const GlobalFrame&)>& handler) final;
	};

	template <SerialProtocolType P>
	class SerialProtocol : public AbstractSerialProtocol<P> {};

	template <>
	class SerialProtocol<SERIAL_LOCAL> final : public AbstractSerialProtocol<SERIAL_LOCAL> {
	public:
		SerialProtocol()
		        : AbstractSerialProtocol(std::make_shared<TCPIP>("127.0.0.1", GLOBAL_CONSTANTS.get_default_TCPIP_port())) {}
	};

	template <>
	class SerialProtocol<SERIAL_NULL> final : public AbstractSerialProtocol<SERIAL_NULL> {
	public:
		SerialProtocol() : AbstractSerialProtocol(std::make_shared<NullCommunicator>()) {}
	};

	template <>
	class SerialProtocol<SERIAL_PIPES> final : public AbstractSerialProtocol<SERIAL_PIPES> {
	public:
		SerialProtocol() : SerialProtocol("/tmp/read.pipe", "/tmp/write.pipe") {}
		SerialProtocol(std::string rx, std::string tx)
		        : AbstractSerialProtocol(std::make_shared<NamedPipe>(std::move(rx), std::move(tx))) {}
	};

	template <>
	class SerialProtocol<SERIAL_RS232> final : public AbstractSerialProtocol<SERIAL_RS232> {
	public:
		explicit SerialProtocol(const std::string& peripheral)
		        : AbstractSerialProtocol(std::make_shared<RS232>(peripheral)) {
			set_delay(10_ms);
		}
	};

	template <>
	class SerialProtocol<SERIAL_TCPIP> final : public AbstractSerialProtocol<SERIAL_TCPIP> {
	public:
		SerialProtocol(std::string address, const std::string& port)
		        : SerialProtocol(std::move(address), static_cast<uint16_t>(std::stoi(port))) {}
		SerialProtocol(std::string address, uint16_t port)
		        : AbstractSerialProtocol(std::make_shared<TCPIP>(std::move(address), port)) {}
	};

	template <>
	class SerialProtocol<SERIAL_UDP> : public AbstractSerialProtocol<SERIAL_UDP> {
		GlobalFrame generate_header(const GlobalFrame&) const final {
			return {};
		}

		GlobalFrame recv_frame_blocking(const std::atomic_bool& running_execution) override {
			while(running_execution) {
				static uint8_t buf[GlobalFrame::DONNEES_TRAME_MAX];

				size_t msg_size = _serial->read_bytes(buf, GlobalFrame::DONNEES_TRAME_MAX);
				return GlobalFrame{static_cast<uint8_t>(msg_size), buf};
			}
			return GlobalFrame{};
		}

	public:
		SerialProtocol(std::string address, const std::string& local_port, const std::string& remote_port)
		        : SerialProtocol(std::move(address),
		                         static_cast<uint16_t>(std::stoi(local_port)),
		                         static_cast<uint16_t>(std::stoi(remote_port))) {}
		SerialProtocol(std::string address, uint16_t local_port, uint16_t remote_port)
		        : AbstractSerialProtocol(std::make_shared<UDP>(std::move(address), local_port, remote_port)) {}
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template <MultiSerialProtocolType M, SerialProtocolType P>
	class AbstractMultiSerialProtocol : public Protocol {
	protected:
		std::map<uint8_t, std::shared_ptr<SerialProtocol<P>>> _serials;

		AbstractMultiSerialProtocol();

	public:
		~AbstractMultiSerialProtocol() override;

		const MultiSerialProtocolType protocol;

		void send_frame(const GlobalFrame& f) final;

		void recv_frame(const std::atomic_bool& running_execution, const std::function<void(const GlobalFrame&)>& handler) final;
	};

	template <MultiSerialProtocolType M, SerialProtocolType P>
	class MultiSerialProtocol : public AbstractMultiSerialProtocol<M, P> {};

	template <>
	class MultiSerialProtocol<ETHERNET, SERIAL_UDP> : public AbstractMultiSerialProtocol<ETHERNET, SERIAL_UDP> {
	public:
		struct UDPConnection {
			uint8_t id_module;
			std::string address;
			uint16_t local_port;
			uint16_t remote_port;

			UDPConnection(uint8_t id, std::string addr, uint16_t local, uint16_t remote)
			        : id_module(id), address(std::move(addr)), local_port(local), remote_port(remote) {}
			UDPConnection(uint8_t id, std::string addr)
			        : UDPConnection(id, std::move(addr), static_cast<uint16_t>(40000 + id), static_cast<uint16_t>(40000 + id)) {}
		};

		explicit MultiSerialProtocol(const UDPConnection& connection) : MultiSerialProtocol({connection}) {}

		MultiSerialProtocol(std::initializer_list<UDPConnection> connections)
		        : MultiSerialProtocol(std::vector<UDPConnection>(connections)) {}

		explicit MultiSerialProtocol(const std::vector<UDPConnection>& connections) : AbstractMultiSerialProtocol() {
			std::for_each(connections.begin(), connections.end(), [this](const UDPConnection& udp) {
				_serials[udp.id_module] = std::make_shared<SerialProtocol<SERIAL_UDP>>(udp.address, udp.local_port, udp.remote_port);
			});
		}
	};

	using protocol_local = SerialProtocol<SERIAL_LOCAL>;
	using protocol_null = SerialProtocol<SERIAL_NULL>;
	using protocol_pipes = SerialProtocol<SERIAL_PIPES>;
	using protocol_rs232 = SerialProtocol<SERIAL_RS232>;
	using protocol_tcpip = SerialProtocol<SERIAL_TCPIP>;
	using protocol_udp = SerialProtocol<SERIAL_UDP>;
	using protocol_ethernet = MultiSerialProtocol<ETHERNET, SERIAL_UDP>;

} // namespace Communication

#include "Protocol.hpp"

#endif // ROOT_PROTOCOLPARSER_H
