//
// Created by terae on 15/02/19.
//

#ifndef ROOT_COMMUNICATORPARSING_H
#define ROOT_COMMUNICATORPARSING_H

#include "Protocol.h"
#include <typeindex>

namespace Communication {
	namespace Arguments {

		EXCEPTION_CLASS(ParsingError);

		struct AbstractArguments {
			virtual std::unique_ptr<Protocol> make_protocol() = 0;

			virtual std::type_index get_protocol_type() const = 0;

			virtual ~AbstractArguments() = default;
		};

		class ArgumentsLocal final : public AbstractArguments {
		public:
			ArgumentsLocal(const std::vector<std::string>&) {}

			ArgumentsLocal() {}


			std::unique_ptr<Protocol> make_protocol() override {
				return std::make_unique<protocol_local>();
			}

			std::type_index get_protocol_type() const override {
				return typeid(protocol_local);
			}
		};

		class ArgumentsNull final : public AbstractArguments {
		public:
			ArgumentsNull(const std::vector<std::string>&) {}

			ArgumentsNull() {}


			std::unique_ptr<Protocol> make_protocol() override {
				return std::make_unique<protocol_null>();
			}

			std::type_index get_protocol_type() const override {
				return typeid(protocol_null);
			}
		};

		class ArgumentsPipes final : public AbstractArguments {
			std::string _rx;
			std::string _tx;

		public:
			ArgumentsPipes(const std::vector<std::string>& args) {
				if(args.size() < 2) {
					throw ParsingError("Utilisation avec PIPES : \"<program_name> PIPES [rx.link] [tx.link]\"");
				}
				_rx = args[0];
				_tx = args[1];
			}

			ArgumentsPipes() : _rx("/tmp/read.pipe"), _tx("/tmp/write.pipe") {}

			std::unique_ptr<Protocol> make_protocol() override {
				return std::make_unique<protocol_pipes>(_rx, _tx);
			}

			std::type_index get_protocol_type() const override {
				return typeid(protocol_pipes);
			}
		};

		class ArgumentsRS232 final : public AbstractArguments {
			std::string _peripheral;

		public:
			ArgumentsRS232(const std::vector<std::string>& args) {
				if(args.size() < 1) {
					throw ParsingError("Utilisation avec RS232 : \"<program_name> RS232 /dev/ttyUSB0\"");
				}
				_peripheral = args[0];
			}

			ArgumentsRS232(std::string peripheral) : _peripheral(peripheral) {}

			std::unique_ptr<Protocol> make_protocol() override {
				return std::make_unique<protocol_rs232>(_peripheral);
			}

			std::type_index get_protocol_type() const override {
				return typeid(protocol_rs232);
			}
		};

		class ArgumentsTCPIP final : public AbstractArguments {
			std::string _address;
			uint16_t _port;

		public:
			ArgumentsTCPIP(const std::vector<std::string>& args) {
				if(args.size() < 2) {
					throw ParsingError("Utilisation avec TCPIP : \"<program_name> TCPIP 127.0.0.1 1234\"");
				}
				_address = args[0];
				_port = static_cast<uint16_t>(stoi(args[1]));
			}

			ArgumentsTCPIP(std::string address, uint16_t port) : _address(address), _port(port) {}

			std::unique_ptr<Protocol> make_protocol() override {
				return std::make_unique<protocol_tcpip>(_address, _port);
			}

			std::type_index get_protocol_type() const override {
				return typeid(protocol_tcpip);
			}
		};

		class ArgumentsUDP final : public AbstractArguments {
			std::string _address;
			uint16_t _local_port;
			uint16_t _remote_port;

		public:
			ArgumentsUDP(const std::vector<std::string>& args) {
				if(args.size() < 3) {
					throw ParsingError(
					    "Utilisation avec UDP : \"<program_name> UDP [@IP] [port local] [port distant]\"");
				}
				_address = args[0];
				_local_port = static_cast<uint16_t>(stoi(args[1]));
				_remote_port = static_cast<uint16_t>(stoi(args[2]));
			}

			ArgumentsUDP(std::string address, uint16_t local_port, uint16_t remote_port)
			        : _address(address), _local_port(local_port), _remote_port(remote_port) {}

			std::unique_ptr<Protocol> make_protocol() override {
				return std::make_unique<protocol_udp>(_address, _local_port, _remote_port);
			}

			std::type_index get_protocol_type() const override {
				return typeid(protocol_udp);
			}
		};

		class ArgumentsEthernet final : public AbstractArguments {
			std::vector<protocol_ethernet::UDPConnection> _connections;

		public:
			ArgumentsEthernet(const std::vector<std::string>& args) {
				if(args.size() % 4 != 0) {
					throw ParsingError(
					    "Utilisation avec ETHERNET : \"<program_name> <[ID] [@IP] [local port] [remote port]>...\"");
				}

				int i = 0;
				do {
					int id = static_cast<uint8_t>(stoi(args[i++]));
					std::string address = args[i++];
					uint16_t local_port = static_cast<uint16_t>(stoi(args[i++]));
					uint16_t remote_port = static_cast<uint16_t>(stoi(args[i++]));
					_connections.emplace_back(id, address, local_port, remote_port);
				} while(i < args.size());
			}

			ArgumentsEthernet(const std::vector<protocol_ethernet::UDPConnection>& connections)
			        : _connections(connections) {}

			void add_connection(uint8_t id, std::string address, uint16_t local_port, uint16_t remote_port) {
				_connections.emplace_back(id, address, local_port, remote_port);
			}

			std::unique_ptr<Protocol> make_protocol() override {
				return std::make_unique<protocol_ethernet>(_connections);
			}

			std::type_index get_protocol_type() const override {
				return typeid(protocol_ethernet);
			}
		};

		struct Parser {
			static std::pair<std::type_index, std::unique_ptr<Protocol>>
			    make_protocol(const std::initializer_list<std::string>& args) {
				return make_protocol(std::vector<std::string>(args));
			}

			static std::pair<std::type_index, std::unique_ptr<Protocol>> make_protocol(const std::vector<std::string>& args) {
				using Utils::split_vector;
				std::unique_ptr<AbstractArguments> arguments;

				for(size_t i = 0; i < args.size(); ++i) {
					// - LOCAL :
					if(args[i] == "LOCAL") {
						arguments = std::make_unique<ArgumentsLocal>();
					}

					// - NULL :
					else if(args[i] == "NULL") {
						arguments = std::make_unique<ArgumentsNull>();
					}

					// - PIPES :
					else if(args[i] == "PIPES") {
						arguments = std::make_unique<ArgumentsPipes>(split_vector(args, i + 1));
					}

					// - RS232 :
					if(args[i] == "RS232") {
						arguments = std::make_unique<ArgumentsRS232>(split_vector(args, i + 1));
					}

					// - TCPIP :
					else if(args[i] == "TCPIP") {
						arguments = std::make_unique<ArgumentsTCPIP>(split_vector(args, i + 1));
					}

					// - UDP :
					else if(args[i] == "UDP") {
						arguments = std::make_unique<ArgumentsUDP>(split_vector(args, i + 1));
					}

					// - Ethernet :
					else if(args[i] == "ETHERNET") {
						arguments = std::make_unique<ArgumentsEthernet>(split_vector(args, i + 1));
					}

					else {
						continue;
					}
					break;
				}

				if(arguments == nullptr) {
					return std::make_pair<std::type_index, std::unique_ptr<Protocol>>(typeid(void), nullptr);
				} else {
					return std::make_pair(arguments->get_protocol_type(), arguments->make_protocol());
				}
			}
		};
	} // namespace Arguments
} // namespace Communication

#endif // ROOT_COMMUNICATORPARSING_H
