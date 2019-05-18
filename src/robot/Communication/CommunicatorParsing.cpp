//
// Created by terae on 09/05/19.
//

#include "CommunicatorParsing.h"

namespace Communication {
	namespace Arguments {

		ArgumentsLocal::ArgumentsLocal(const std::vector<std::string>&) : ArgumentsLocal() {}

		ArgumentsLocal::ArgumentsLocal() {}

		std::unique_ptr<Protocol> ArgumentsLocal::make_protocol() {
			return std::make_unique<protocol_local>();
		}

		std::type_index ArgumentsLocal::get_protocol_type() const {
			return typeid(protocol_local);
		}

		ArgumentsNull::ArgumentsNull(const std::vector<std::string>&) : ArgumentsNull() {}

		ArgumentsNull::ArgumentsNull() {}

		std::unique_ptr<Protocol> ArgumentsNull::ArgumentsNull::make_protocol() {
			return std::make_unique<protocol_null>();
		}

		std::type_index ArgumentsNull::get_protocol_type() const {
			return typeid(protocol_null);
		}

		ArgumentsPipes::ArgumentsPipes(const std::vector<std::string>& args) {
			if(args.size() < 2) {
				throw ParsingError("Utilisation avec PIPES : \"<program_name> PIPES [rx.link] [tx.link]\"");
			}
			_rx = args[0];
			_tx = args[1];
		}

		ArgumentsPipes::ArgumentsPipes(std::string rx, std::string tx) : _rx(std::move(rx)), _tx(std::move(tx)) {}

		ArgumentsPipes::ArgumentsPipes() : ArgumentsPipes("/tmp/read.pipe", "/tmp/write.pipe") {}

		std::unique_ptr<Protocol> ArgumentsPipes::make_protocol() {
			return std::make_unique<protocol_pipes>(_rx, _tx);
		}

		std::type_index ArgumentsPipes::get_protocol_type() const {
			return typeid(protocol_pipes);
		}

		ArgumentsRS232::ArgumentsRS232(const std::vector<std::string>& args) {
			if(args.size() < 1) {
				throw ParsingError("Utilisation avec RS232 : \"<program_name> RS232 /dev/ttyUSB0\"");
			}
			_peripheral = args[0];
		}

		ArgumentsRS232::ArgumentsRS232(std::string peripheral) : _peripheral(std::move(peripheral)) {}

		std::unique_ptr<Protocol> ArgumentsRS232::make_protocol() {
			return std::make_unique<protocol_rs232>(_peripheral);
		}

		std::type_index ArgumentsRS232::get_protocol_type() const {
			return typeid(protocol_rs232);
		}

		ArgumentsTCPIP::ArgumentsTCPIP(const std::vector<std::string>& args) {
			if(args.size() < 2) {
				throw ParsingError("Utilisation avec TCPIP : \"<program_name> TCPIP 127.0.0.1 1234\"");
			}
			_address = args[0];
			_port = static_cast<uint16_t>(stoi(args[1]));
		}

		ArgumentsTCPIP::ArgumentsTCPIP(std::string address, uint16_t port) : _address(address), _port(port) {}

		std::unique_ptr<Protocol> ArgumentsTCPIP::make_protocol() {
			return std::make_unique<protocol_tcpip>(_address, _port);
		}

		std::type_index ArgumentsTCPIP::get_protocol_type() const {
			return typeid(protocol_tcpip);
		}

		ArgumentsUDP::ArgumentsUDP(const std::vector<std::string>& args) {
			if(args.size() < 3) {
				throw ParsingError("Utilisation avec UDP : \"<program_name> UDP [@IP] [port local] [port distant]\"");
			}
			_address = args[0];
			_local_port = static_cast<uint16_t>(stoi(args[1]));
			_remote_port = static_cast<uint16_t>(stoi(args[2]));
		}

		ArgumentsUDP::ArgumentsUDP(const std::string& address, uint16_t local_port, uint16_t remote_port)
		        : _address(address), _local_port(local_port), _remote_port(remote_port) {}

		std::unique_ptr<Protocol> ArgumentsUDP::make_protocol() {
			return std::make_unique<protocol_udp>(_address, _local_port, _remote_port);
		}

		std::type_index ArgumentsUDP::get_protocol_type() const {
			return typeid(protocol_udp);
		}

		ArgumentsEthernet::ArgumentsEthernet(const std::vector<std::string>& args) {
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

		ArgumentsEthernet::ArgumentsEthernet(const std::vector<protocol_ethernet::UDPConnection>& connections)
		        : _connections(connections) {}

		void ArgumentsEthernet::add_connection(uint8_t id, std::string address, uint16_t local_port, uint16_t remote_port) {
			_connections.emplace_back(id, address, local_port, remote_port);
		}

		std::unique_ptr<Protocol> ArgumentsEthernet::make_protocol() {
			return std::make_unique<protocol_ethernet>(_connections);
		}

		std::type_index ArgumentsEthernet::get_protocol_type() const {
			return typeid(protocol_ethernet);
		}

		std::pair<std::type_index, std::unique_ptr<Protocol>> Parser::make_protocol(const std::initializer_list<std::string>& args) {
			return make_protocol(std::vector<std::string>(args));
		}

		std::pair<std::type_index, std::unique_ptr<Protocol>> Parser::make_protocol(const std::vector<std::string>& args) {
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
				return std::pair<std::type_index, std::unique_ptr<Protocol>>(typeid(void), nullptr);
			} else {
				return std::make_pair(arguments->get_protocol_type(), arguments->make_protocol());
			}
		}

		std::pair<std::type_index, std::unique_ptr<Protocol>> Parser::make_protocol(const Constants::RobotInitializationData& robot) {
			const std::string protocol = robot.get_protocol_type();
			auto map_arguments = robot.get_communication_arguments();
			std::unique_ptr<AbstractArguments> arguments;

			auto check_arguments_v = [&map_arguments](const std::vector<std::string>& fields) -> bool {
				for(std::string field : fields) {
					if(map_arguments.find(field) == map_arguments.cend()) {
						return false;
					}
				}
				return true;
			};

			auto check_arguments = [&check_arguments_v](const std::initializer_list<std::string>& fields) -> bool {
				return check_arguments_v(std::vector(fields));
			};

			if(protocol == "local" && map_arguments.empty()) {
				arguments = std::make_unique<ArgumentsLocal>();
			} else if(protocol == "null" && map_arguments.empty()) {
				arguments = std::make_unique<ArgumentsNull>();
			} else if(protocol == "pipes" && map_arguments.size() == 2) {
				if(check_arguments({"rx", "tx"})) {
					arguments = std::make_unique<ArgumentsPipes>(map_arguments["rx"], map_arguments["tx"]);
				} else {
					throw ParsingError("Impossible to find the fields 'rx' and 'tx for a Pipes' communication.");
				}
			} else if(protocol == "pipes" && map_arguments.empty()) {
				arguments = std::make_unique<ArgumentsPipes>();
			} else if(protocol == "rs232") {
				if(check_arguments({"peripheral"})) {
					arguments = std::make_unique<ArgumentsRS232>(map_arguments["peripheral"]);
				} else {
					throw ParsingError("Impossible to find the field 'peripheral' for a RS232 communication.");
				}
			} else if(protocol == "tcpip" && map_arguments.size() == 2) {
				if(check_arguments({"address", "port"})) {
					arguments = std::make_unique<ArgumentsTCPIP>(map_arguments["address"], std::stoi(map_arguments["port"]));
				} else {
					throw ParsingError("Impossible to find the fields 'address' and 'port' for a TCPIP communication.");
				}
			} else if(protocol == "udp" && map_arguments.size() == 3) {
				if(check_arguments({"address", "local_port", "remote_port"})) {
					arguments = std::make_unique<ArgumentsUDP>(map_arguments["address"],
					                                           std::stoi(map_arguments["local_port"]),
					                                           std::stoi(map_arguments["remote_port"]));
				} else {
					throw ParsingError("Impossible to find the fields 'address', 'local_port' and 'remote_port' "
					                   "for an UDP communication.");
				}
			} else if(protocol == "ethernet" && map_arguments.size() % 4 == 0) {
				const std::size_t nb_modules = map_arguments.size() / 4;
				std::vector<std::string> expected_fields;
				for(std::size_t i = 0; i < nb_modules; ++i) {
					const std::string prefix = std::to_string(i) + '_';
					expected_fields.emplace_back(prefix + "id");
					expected_fields.emplace_back(prefix + "address");
					expected_fields.emplace_back(prefix + "local_port");
					expected_fields.emplace_back(prefix + "remote_port");
				}

				if(check_arguments_v(expected_fields)) {
					std::vector<protocol_ethernet::UDPConnection> args;
					for(std::size_t i = 0; i < nb_modules; ++i) {
						const std::string prefix = std::to_string(i) + '_';

						const uint8_t id = static_cast<uint8_t>(std::stoi(map_arguments[prefix + "id"]));
						const std::string address = map_arguments[prefix + "address"];
						const uint16_t local_port =
						    static_cast<uint16_t>(std::stoi(map_arguments[prefix + "local_port"]));
						const uint16_t remote_port =
						    static_cast<uint16_t>(std::stoi(map_arguments[prefix + "remote_port"]));

						args.emplace_back(id, address, local_port, remote_port);
					}
					arguments = std::make_unique<ArgumentsEthernet>(args);
				} else {
					throw ParsingError("Impossible to find the fields '<N>_id', '<N>_address', '<N>_local_port' "
					                   "and '<N>_remote_port' for an Ethernet communication.");
				}
			}

			if(arguments == nullptr) {
				return std::pair<std::type_index, std::unique_ptr<Protocol>>(typeid(void), nullptr);
			} else {
				return std::make_pair(arguments->get_protocol_type(), arguments->make_protocol());
			}
		}
	} // namespace Arguments
} // namespace Communication
