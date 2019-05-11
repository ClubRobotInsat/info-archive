//
// Created by terae on 15/02/19.
//

#ifndef ROOT_COMMUNICATORPARSING_H
#define ROOT_COMMUNICATORPARSING_H

#include "Protocol.h"
#include "log/Log.h"
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
			explicit ArgumentsLocal(const std::vector<std::string>&);

			ArgumentsLocal();

			std::unique_ptr<Protocol> make_protocol() override;

			std::type_index get_protocol_type() const override;
		};

		class ArgumentsNull final : public AbstractArguments {
		public:
			explicit ArgumentsNull(const std::vector<std::string>&);

			ArgumentsNull();

			std::unique_ptr<Protocol> make_protocol() override;

			std::type_index get_protocol_type() const override;
		};

		class ArgumentsPipes final : public AbstractArguments {
			std::string _rx;
			std::string _tx;

		public:
			explicit ArgumentsPipes(const std::vector<std::string>& args);

			ArgumentsPipes(std::string rx, std::string tx);

			ArgumentsPipes();

			std::unique_ptr<Protocol> make_protocol() override;

			std::type_index get_protocol_type() const override;
		};

		class ArgumentsRS232 final : public AbstractArguments {
			std::string _peripheral;

		public:
			explicit ArgumentsRS232(const std::vector<std::string>& args);

			explicit ArgumentsRS232(std::string peripheral);

			std::unique_ptr<Protocol> make_protocol() override;

			std::type_index get_protocol_type() const override;
		};

		class ArgumentsTCPIP final : public AbstractArguments {
			std::string _address;
			uint16_t _port;

		public:
			explicit ArgumentsTCPIP(const std::vector<std::string>& args);

			ArgumentsTCPIP(std::string address, uint16_t port);

			std::unique_ptr<Protocol> make_protocol() override;

			std::type_index get_protocol_type() const override;
		};

		class ArgumentsUDP final : public AbstractArguments {
			std::string _address;
			uint16_t _local_port;
			uint16_t _remote_port;
			/// Add a size field to the message to control it arrived completely
			bool _size_control = false;

		public:
			explicit ArgumentsUDP(const std::vector<std::string>& args);

			ArgumentsUDP(const std::string& address, uint16_t local_port, uint16_t remote_port);

			std::unique_ptr<Protocol> make_protocol() override;

			std::type_index get_protocol_type() const override;
		};

		class ArgumentsEthernet final : public AbstractArguments {
			std::vector<protocol_ethernet::UDPConnection> _connections;

		public:
			explicit ArgumentsEthernet(const std::vector<std::string>& args);

			ArgumentsEthernet(const std::vector<protocol_ethernet::UDPConnection>& connections);

			void add_connection(uint8_t id, std::string address, uint16_t local_port, uint16_t remote_port);

			std::unique_ptr<Protocol> make_protocol() override;

			std::type_index get_protocol_type() const override;
		};

		struct Parser {
			static std::pair<std::type_index, std::unique_ptr<Protocol>>
			    make_protocol(const std::initializer_list<std::string>& args);

			static std::pair<std::type_index, std::unique_ptr<Protocol>> make_protocol(const std::vector<std::string>& args);

			static std::pair<std::type_index, std::unique_ptr<Protocol>>
			    make_protocol(const Constants::RobotInitializationData& robot);
		};
	} // namespace Arguments
} // namespace Communication

#endif // ROOT_COMMUNICATORPARSING_H
