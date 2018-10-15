//
// Created by terae on 17/09/18.
//

#include "ElecCommunicator.h"
#include "NullCommunicator.h"

namespace Communication {
	template <typename ParsingClass>
	std::vector<std::string> ElecCommunicator<ParsingClass>::makeArgs(const char* typeConnexion, const char* adresse, int port) {
		std::vector<std::string> result = {"Simulateur", typeConnexion};
		if(typeConnexion == "RS232"s) {
			result.emplace_back(adresse);
		} else if(typeConnexion == "TCPIP"s) {
			result.emplace_back(adresse);
			result.push_back(std::to_string(port));
		}

		return result;
	}

	template <typename ParsingClass>
	ElecCommunicator<ParsingClass>::ElecCommunicator(std::shared_ptr<ParsingClass> parser, uint16_t default_port_TCPIP)
	        : _parser(std::move(parser))
	        , _default_port_TCPIP(default_port_TCPIP)
	        , _connecte(false)
	        , _modeConnexion(ModeConnexion::OTHERS) {}

	template <typename ParsingClass>
	ElecCommunicator<ParsingClass>::ElecCommunicator(ParsingClass& parser, uint16_t default_port_TCPIP)
	        : ElecCommunicator(parser.shared_from_this(), default_port_TCPIP) {}

	template <typename ParsingClass>
	ElecCommunicator<ParsingClass>::~ElecCommunicator() {
		disconnect();
	}

	template <typename ParsingClass>
	CAN& ElecCommunicator<ParsingClass>::getCAN() {
		return *_busCAN;
	}

	/// Debug du CAN pour les elecs
	template <typename ParsingClass>
	void ElecCommunicator<ParsingClass>::setDebugCAN(bool active) {
		_busCAN->setDebug(active);
	}

	/*template <typename ParsingClass>
	bool ElecCommunicator<ParsingClass>::isSimuConnected() {
	    return this->_simuConnected;
	}

	template <typename ParsingClass>
	Socket& ElecCommunicator<ParsingClass>::getSocketSimu() {
	    return *_socketSimu;
	}*/

	/// Connexion a la main à partir des arguments passés au programme.
	template <typename ParsingClass>
	bool ElecCommunicator<ParsingClass>::connect(std::vector<std::string> const& args) {
		logInfo("Initialisation de la communication élec/info.");

		for(size_t i = 1; i < args.size() /*&& !_connecte*/; ++i) {
			// - RS232 :
			if(args[i] == "RS232") {
				if((args.size() - 1) - i < 1) {
					logError("Utilisation avec RS232 : \"", args[0], " RS232 /dev/ttyUSB0\"\n");
					exit(EXIT_FAILURE);
				} else {
					_busCAN = std::make_unique<CAN>(std::make_unique<RS232>(args[i + 1]));
					_busCAN->setTemporisation(10_ms); // Temporisation de 10 ms entre chaque trame pour le bus CAN
					// électronique (pas de tempo pour le simu)
					_connecte = true;
				}

				_modeConnexion = ModeConnexion::RS232;
				break;
			}
			// - TCPIP :
			else if(args[i] == "TCPIP") {
				if((args.size() - 1) - i < 2) {
					logError("Utilisation avec TCPIP : \"", args[0], " TCPIP 127.0.0.1 1234\"\n");
					exit(EXIT_FAILURE);
				} else {
					_busCAN = std::make_unique<CAN>(std::make_unique<TCPIP>(args[i + 1], atoi(args[i + 2].c_str())));
					_connecte = true;
				}

				break;
			}
			// - PIPES :
			else if(args[i] == "PIPES") {
				logDebug9("Initialisation de la connection au CAN local par pipes nommés");
				_busCAN = std::make_unique<CAN>(std::make_unique<NamedPipe>("/tmp/read.pipe", "/tmp/write.pipe"));
				_modeConnexion = ModeConnexion::PIPES;
				_connecte = true;
				break;
			}
			// - LOCAL :
			else if(args[i] == "LOCAL") {
				logDebug9("Initialisation de la connexion au CAN local");
				_busCAN = std::make_unique<CAN>(std::make_unique<TCPIP>("127.0.0.1", _default_port_TCPIP));
				_connecte = true;
				_modeConnexion = ModeConnexion::LOCAL;
			} else if(args[i] == "NULL") {
				logDebug9("Initialisation de la connexion au CAN local par le NullCommunicator");
				_busCAN = std::make_unique<CAN>(std::make_unique<NullCommunicator>());
				_connecte = true;
			} else if(args[i] == "SIMU") {
				logDebug9("Initialisation socket client côté robot");
				// TODO : voir si on garde cette architecture
				/*_socketSimu = std::make_unique<Socket>(SockProtocol::TCP);
				_socketSimu->connect("127.0.0.1", _default_port_TCPIP);
				_simuConnected = true;*/
			}
		}

		// Cas où l'on n'a rien trouvé indiquant comment se connecter :
		if(!_connecte) {
			logInfo("Utilisation : \n");
			logInfo("- ", args[0], " RS232 [peripherique] (ex : \"", args[0], " RS232 /dev/ttyUSB0\")");
			logInfo("- ", args[0], " TCPIP [adresse IP] [port] (ex : \"", args[0], " TCPIP 127.0.0.1 1234\")");
			logInfo("- ", args[0], " LOCAL --color [color]");
			logInfo("- ", args[0], " PIPES");
			logInfo("Ajouter SIMU pour établir une connection avec le socket du simu.");
			return false;
		}

		_reception = std::thread(&ElecCommunicator::communicate_with_elecs, this);
		return true;
	}

	template <typename ParsingClass>
	void ElecCommunicator<ParsingClass>::disconnect() {
		if(_connecte) {
			_running_execution.store(false);
			_reception.join();
		}
	}

	template <typename ParsingClass>
	void ElecCommunicator<ParsingClass>::set_modules_initialized() {
		_modules_initialized.notify_all();
		_modules_init_notified = true;
	}

	/// Communication avec les élecs
	// TODO : améliorer le principe, actuellement on réponds dès qu'on a la réponse
	template <typename ParsingClass>
	template <typename>
	void ElecCommunicator<ParsingClass>::communicate_with_elecs() {
		// setThreadName("Communicate");
		// Une seule instance de communication peut être lancée à la fois
		std::unique_lock<std::mutex> lk(_mutex_communication);

		// TODO : gérer le PING pour savoir quand les élecs sont Ok
		if(!_modules_init_notified) {
			_modules_initialized.wait(lk);
		}

		// La réception et l'envoi de trames est asynchrone, d'où le partage de ce mutex
		std::mutex mutex_medium;


		/*auto output_function = [](std::mutex& mut, std::atomic_bool& running_execution) {
		    while(running_execution) {
		        std::lock_guard<std::mutex> lock(mut);
		        // TODO: output ; gérer la retransmission directe + expiration d'un timer pour chaque module
		    }
		};

		auto out = std::thread(output_function, mutex_medium, _running_execution);*/

		while(_running_execution) {
			std::lock_guard<std::mutex> lock(mutex_medium);

			try {
				// FIXME: fonction bloquante donc le mutex est toujours lock
				auto frame = _busCAN->recevoirTrameBloquant();
				_parser->read_frame(frame);
			} catch(std::runtime_error& e) {
				logError("Échec de la mise à jour du module manager !!");
				logError("Exception rencontrée : ", e.what());
			}

			try {
				_busCAN->envoyerTrame(_parser->write_frame(), true);
			} catch(std::runtime_error& e) {
				logError("Échec de l'envoi de l'état du robot par le module manager !!");
				logError("Exception rencontrée : ", e.what());
			}
		}
	}
} // namespace Communication
