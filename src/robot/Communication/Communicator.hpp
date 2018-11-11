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
	        : _parser(std::move(parser)), _connected(false), _modules_init_notified(false), _debug_active(false) {
		_chrono.reset();
	}

	template <typename ParsingClass>
	Communicator<ParsingClass>::~Communicator() {
		disconnect();
	}

	template <typename ParsingClass>
	bool Communicator<ParsingClass>::connect(const std::vector<std::string>& args) {
		using std::stoi;

		logInfo("Initialisation de la communication élec/info.");

		for(size_t i = 1; i < args.size() && !_connected; ++i) {
			// - RS232 :
			if(args[i] == "RS232") {
				if((args.size() - 1) - i < 1) {
					logError("Utilisation avec RS232 : \"", args[0], " RS232 /dev/ttyUSB0\"\n");
					exit(EXIT_FAILURE);
				}

				_protocol = std::make_unique<protocol_rs232>(args[i + 1]);
				break;
			}

			// - TCPIP :
			else if(args[i] == "TCPIP") {
				if((args.size() - 1) - i < 2) {
					logError("Utilisation avec TCPIP : \"", args[0], " TCPIP 127.0.0.1 1234\"\n");
					exit(EXIT_FAILURE);
				}

				_protocol = std::make_unique<protocol_tcpip>(args[i + 1], stoi(args[i + 2]));
				break;
			}

			// - UDP :
			else if(args[i] == "UDP") {
				if((args.size() - 1) - i < 3) {
					logError("Utilisation avec UDP : \"", args[0], " UDP [@IP] [port local] [port distant]\"\n");
					exit(EXIT_FAILURE);
				}

				_protocol = std::make_unique<protocol_udp>(args[i + 1], stoi(args[i + 2]), stoi(args[i + 3]));
				break;
			}

			// - PIPES :
			else if(args[i] == "PIPES") {
				logDebug9("Initialisation de la connection au CAN local par pipes nommés");

				_protocol = std::make_unique<protocol_pipes>("/tmp/read.pipe", "/tmp/write.pipe");
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
				    protocol_ethernet::UDPConnection{stoi(args[i + 1]), args[i + 2], stoi(args[i + 3]), stoi(args[i + 4])});
				break;
			}

			// - LOCAL :
			else if(args[i] == "LOCAL") {
				logDebug9("Initialisation de la connexion au CAN local");
				_protocol = std::make_unique<protocol_local>();
			}

			// - NULL :
			else if(args[i] == "NULL") {
				logDebug9("Initialisation de la connexion au CAN local par le NullCommunicator");
				_protocol = std::make_unique<protocol_null>();
			}

			// - option SIMU pour les connexions LOCAL ou NULL
			else if(args[i] == "SIMU") {
				logDebug9("Initialisation socket client côté robot");
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
			_running_execution.store(false);
			_communication.join();
		}
	}

	/// Notification par le constructeur du robot physique que la communication peut démarrer
	template <typename ParsingClass>
	void Communicator<ParsingClass>::set_modules_initialized() {
		_modules_initialized.notify_all();
		_modules_init_notified = true;
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

		// On attends que la phase d'initialisation du robot soit finie
		if(!_modules_init_notified) {
			_modules_initialized.wait(lk);
		}

		auto output_function = [this](std::atomic_bool& running_execution) {
			while(running_execution) {
				std::optional<GlobalFrame> frame = _parser->write_frame();
				if(frame == std::nullopt) {
					/*if(_debug_active) {
					    logDebug0("None(GlobalFrame) to send; wait.\ntime: ", _chrono.getElapsedTime(), "\n");
					}*/
					// La classe de parsing peut ne rien avoir à envoyer, donc temporisation sur l'électronique
					sleep(GLOBAL_CONSTANTS.get_default_communication_delay());
				} else {
					// Trame à envoyer
					_protocol->send_frame(frame.value());
				}
			}
		};

		auto input_function = [this](std::atomic_bool& running_execution) {
			_protocol->recv_frame(running_execution, [this](const GlobalFrame& f) { _parser->read_frame(f); });
		};

		std::thread out = std::thread(output_function, std::ref(_connected));
		std::thread in = std::thread(input_function, std::ref(_connected));

		in.join();
		out.join();
	}
} // namespace Communication
