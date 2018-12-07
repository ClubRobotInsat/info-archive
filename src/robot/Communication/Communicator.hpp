//
// Created by terae on 16/10/18.
//

#include "Communicator.h"
#include "Protocol.h"
#include <Constants.h>
#include <log/Log.h>

namespace Communication {
	template <typename ParsingClass>
	Communicator<ParsingClass>::Communicator(std::shared_ptr<ParsingClass> parser)
	        : _parser(std::move(parser)), _connected(false), _debug_active(false) {
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

		for(size_t i = 1; i < args.size() && !_connected; ++i) {
			// - RS232 :
			if(args[i] == "RS232") {
				if((args.size() - 1) - i < 1) {
					logError("Utilisation avec RS232 : \"", args[0], " RS232 /dev/ttyUSB0\"\n");
					exit(EXIT_FAILURE);
				}

				_protocol = std::make_unique<protocol_rs232>(args[i + 1]);
				_connected.exchange(true);
				break;
			}

			// - TCPIP :
			else if(args[i] == "TCPIP") {
				if((args.size() - 1) - i < 2) {
					logError("Utilisation avec TCPIP : \"", args[0], " TCPIP 127.0.0.1 1234\"\n");
					exit(EXIT_FAILURE);
				}

				_protocol = std::make_unique<protocol_tcpip>(args[i + 1], stoi(args[i + 2]));
				_connected.exchange(true);
				break;
			}

			// - UDP :
			else if(args[i] == "UDP") {
				if((args.size() - 1) - i < 3) {
					logError("Utilisation avec UDP : \"", args[0], " UDP [@IP] [port local] [port distant]\"\n");
					exit(EXIT_FAILURE);
				}

				_protocol = std::make_unique<protocol_udp>(args[i + 1], stoi(args[i + 2]), stoi(args[i + 3]));
				_connected.exchange(true);
				break;
			}

			// - PIPES :
			else if(args[i] == "PIPES") {
				if(_debug_active) {
					logDebug9("Initialisation de la connection au CAN local par pipes nommés");
				}
				_protocol = std::make_unique<protocol_pipes>("/tmp/read.pipe", "/tmp/write.pipe");
				_connected.exchange(true);
				break;
			}

			// - ETHERNET :
			// TODO : voir comment ajouter les connexions UDP depuis le vector ; utiliser directement les constantes ?
			else if(args[i] == "ETHERNET") {
				if((args.size() - 1) - i < 4) {
					// FIXME : on doit fournir une unique connexion UDP actuellement
					logError("Utilisation **temporaire** avec ETHERNET : \"", args[0], " ETHERNET [ID] [@IP] [port local] [port distant]\"\n");
					exit(EXIT_FAILURE);
				}

				_protocol = std::make_unique<protocol_ethernet>(
				    protocol_ethernet::UDPConnection{static_cast<uint8_t>(stoi(args[i + 1])),
				                                     args[i + 2],
				                                     static_cast<uint16_t>(stoi(args[i + 3])),
				                                     static_cast<uint16_t>(stoi(args[i + 4]))});
				_connected.exchange(true);
				break;
			}

			// - LOCAL :
			else if(args[i] == "LOCAL") {
				if(_debug_active) {
					logDebug9("Initialisation de la connexion au CAN local");
				}
				_protocol = std::make_unique<protocol_local>();
				_connected.exchange(true);
			}

			// - NULL :
			else if(args[i] == "NULL") {
				if(_debug_active) {
					logDebug9("Initialisation de la connexion au CAN local par le NullCommunicator");
				}
				_protocol = std::make_unique<protocol_null>();
				_connected.exchange(true);
			}

			// - option SIMU pour les connexions LOCAL ou NULL
			else if(args[i] == "SIMU") {
				if(_debug_active) {
					logDebug9("Initialisation socket client côté robot");
				}
				// TODO : voir si on garde cette architecture
				/*_socketSimu = std::make_unique<Socket>(SockProtocol::TCP);
				_socketSimu->connect("127.0.0.1", _default_port_TCPIP);
				_simuConnected = true;*/
			}
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
			logInfo("- ", args[0], " ETHERNET");
			logInfo("Ajouter SIMU pour établir une connection avec le socket du simu.");
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
				std::optional<std::vector<GlobalFrame>> frames = _parser->write_frame();
				if(frames == std::nullopt) {
					/*if(_debug_active) {
					    logDebug0("None(GlobalFrame) to send; wait.\ntime: ", _chrono.getElapsedTime(), "\n");
					}*/
					// La classe de parsing peut ne rien avoir à envoyer, donc temporisation sur l'électronique
					sleep(GLOBAL_CONSTANTS.get_default_communication_delay());
				} else {
					// Trames à envoyer
					for(const GlobalFrame& f : frames.value()) {
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
