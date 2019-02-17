//
// Created by terae on 16/10/18.
//

#include "Communicator.h"
#include "CommunicatorParsing.hpp"
#include "Protocol.h"
#include <Constants.h>
#include <log/Log.h>

namespace Communication {
	template <typename ParsingClass>
	Communicator<ParsingClass>::Communicator(std::shared_ptr<ParsingClass> parser)
	        : _parser(std::move(parser)), _protocol_type(typeid(void)), _connected(false), _debug_active(false) {
		_chrono.reset();
	}

	template <typename ParsingClass>
	Communicator<ParsingClass>::~Communicator() {
		disconnect();
	}

	template <typename ParsingClass>
	bool Communicator<ParsingClass>::connect(const std::vector<std::string>& args) {
		using std::stoi;

		if(_debug_active) {
			logInfo("Initialisation de la communication élec/info.");
		}

		std::tie(_protocol_type, _protocol) = Arguments::Parser::make_protocol(Utils::split_vector(args, 1));
		if(_protocol_type != typeid(void)) {
			_connected.exchange(true);
		}

		// Cas où l'on n'a rien trouvé indiquant comment se connecter :
		if(!_connected) {
			logInfo("Utilisation : \n");
			logInfo("=== Connexion série one-to-one ===");
			logInfo("- ", args[0], " LOCAL --color [color]");
			logInfo("- ", args[0], " NULL");
			logInfo("- ", args[0], " PIPES");
			logInfo("- ", args[0], " RS232 [peripherique] (ex : \"", args[0], " RS232 /dev/ttyUSB0\")");
			logInfo("- ", args[0], " TCPIP [adresse IP] [port] (ex : \"", args[0], " TCPIP 127.0.0.1 1234\")");
			logInfo("- ", args[0], " UDP [adresse IP] [port local] [port distant] (ex : \"", args[0], " UDP 127.0.0.1 1234 40000");
			logInfo("=== Connexion ethernet sur le LAN ===");
			logInfo("- ", args[0], " ETHERNET <[ID] [@IP] [local port] [remote port]>...");
			// logInfo("Ajouter SIMU pour établir une connection avec le socket du simu.");
			return false;
		}

		_communication = std::thread(&Communicator<ParsingClass>::communicate_with_elecs, this);
		return true;
	}

	/// Arrêt de la communication et déconnexion du médium
	template <typename ParsingClass>
	void Communicator<ParsingClass>::disconnect() {
		if(_connected) {
			_connected.store(false);
			_communication.join();
		}
		_protocol_type = typeid(void);
	}

	template <typename ParsingClass>
	std::type_index Communicator<ParsingClass>::get_protocol_type() const {
		return _protocol_type;
	}

	template <typename ParsingClass>
	void Communicator<ParsingClass>::set_debug(bool active) {
		_debug_active = active;
		if(_protocol != nullptr) {
			_protocol->debug_active = active;
		}
	}

	/// Communication avec les élecs
	template <typename ParsingClass>
	template <typename>
	void Communicator<ParsingClass>::communicate_with_elecs() {
		// Une seule instance de communication peut être lancée à la fois
		std::unique_lock<std::mutex> lk(_mutex_communication);

		auto output_function = [this](std::atomic_bool& running_execution) {
			while(running_execution) {
				std::vector<GlobalFrame> frames = _parser->write_frame();
				if(frames.empty()) {
					/*if(_debug_active) {
					    logDebug0("None(GlobalFrame) to send; wait.\ntime: ", _chrono.getElapsedTime(), "\n");
					}*/
					// La classe de parsing peut ne rien avoir à envoyer, donc temporisation sur l'électronique
					sleep(GLOBAL_CONSTANTS().get_default_communication_delay());
				} else {
					// Trames à envoyer
					for(const GlobalFrame& f : frames) {
						_protocol->send_frame(f);
					}
				}
			}
		};

		auto input_function = [this](std::atomic_bool& running_execution) {
			try {
				_protocol->recv_frame(running_execution, [this](const GlobalFrame& f) { _parser->read_frame(f); });
			} catch(const Protocol::ReceptionAborted&) {
				if(_debug_active) {
					logInfo("communicate_with_elecs::input_function() exited.");
				}
			}
		};

		std::thread out = std::thread(output_function, std::ref(_connected));
		std::thread in = std::thread(input_function, std::ref(_connected));

		in.join();
		out.join();
	}
} // namespace Communication
