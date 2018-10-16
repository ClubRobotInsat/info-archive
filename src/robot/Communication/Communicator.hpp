//
// Created by terae on 16/10/18.
//

#include "Communicator.h"
#include <Constants.h>
#include <log/Log.h>

namespace Communication {
	template <typename ParsingClass>
	Communicator<ParsingClass>::Communicator(std::shared_ptr<ParsingClass> parser)
	        : _parser(std::move(parser))
	        , _connected(false)
	        , _modules_init_notified(false)
	        , _delay(GLOBAL_CONSTANTS.get_default_communication_delay())
	        , _debug_active(false) {
		_chrono.reset();
	}

	template <typename ParsingClass>
	Communicator<ParsingClass>::~Communicator() {
		disconnect();
	}

	template <typename ParsingClass>
	bool Communicator<ParsingClass>::connect(const std::vector<std::string>& args) {
		logInfo("Initialisation de la communication élec/info.");

		for(size_t i = 1; i < args.size() && !_connected; ++i) {
			// - RS232 :
			if(args[i] == "RS232") {
				if((args.size() - 1) - i < 1) {
					logError("Utilisation avec RS232 : \"", args[0], " RS232 /dev/ttyUSB0\"\n");
					exit(EXIT_FAILURE);
				} else {
					_serial = std::make_unique<RS232>(args[i + 1]);
					this->set_delay(10_ms); // Temporisation de 10 ms entre chaque trame pour le bus CAN
					// électronique (pas de tempo pour le simu)
					_connected = true;
				}

				//_modeConnexion = ModeConnexion::RS232;
				break;
			}

			// - TCPIP :
			else if(args[i] == "TCPIP") {
				if((args.size() - 1) - i < 2) {
					logError("Utilisation avec TCPIP : \"", args[0], " TCPIP 127.0.0.1 1234\"\n");
					exit(EXIT_FAILURE);
				} else {
					_serial = std::make_unique<TCPIP>(args[i + 1], std::stoi(args[i + 2]));
					_connected = true;
				}

				break;
			}
			// - PIPES :
			else if(args[i] == "PIPES") {
				logDebug9("Initialisation de la connection au CAN local par pipes nommés");
				_serial = std::make_unique<NamedPipe>("/tmp/read.pipe", "/tmp/write.pipe");
				//_modeConnexion = ModeConnexion::PIPES;
				_connected = true;
				break;
			}
			// - LOCAL :
			else if(args[i] == "LOCAL") {
				logDebug9("Initialisation de la connexion au CAN local");
				_serial = std::make_unique<TCPIP>("127.0.0.1", GLOBAL_CONSTANTS.get_default_TCPIP_port());
				//_modeConnexion = ModeConnexion::LOCAL;
				_connected = true;
			} else if(args[i] == "NULL") {
				logDebug9("Initialisation de la connexion au CAN local par le NullCommunicator");
				_serial = std::make_unique<NullCommunicator>();
				_connected = true;
			} else if(args[i] == "SIMU") {
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
			logInfo("- ", args[0], " RS232 [peripherique] (ex : \"", args[0], " RS232 /dev/ttyUSB0\")");
			logInfo("- ", args[0], " TCPIP [adresse IP] [port] (ex : \"", args[0], " TCPIP 127.0.0.1 1234\")");
			logInfo("- ", args[0], " LOCAL --color [color]");
			logInfo("- ", args[0], " PIPES");
			logInfo("Ajouter SIMU pour établir une connection avec le socket du simu.");
			return false;
		}

		_reception = std::thread(&Communicator<ParsingClass>::communicate_with_elecs, this);
		return true;
	}

	/// Arrêt de la communication et déconnexion du médium
	template <typename ParsingClass>
	void Communicator<ParsingClass>::disconnect() {
		if(_connected) {
			_running_execution.store(false);
			_reception.join();
		}
	}

	/// Notification par le constructeur du robot physique que la communication peut démarrer
	template <typename ParsingClass>
	void Communicator<ParsingClass>::set_modules_initialized() {
		_modules_initialized.notify_all();
		_modules_init_notified = true;
	}

	template <typename ParsingClass>
	void Communicator<ParsingClass>::send_frame(const GlobalFrame& f) {
		if(_debug_active) {
			logDebug("Communicator.SEND ", f, "\ntime: ", _chrono.getElapsedTime(), "\n");
		}
		std::lock_guard<std::mutex> lk(_mutex_write);

		_serial->ecrireOctet(BYTE_BEGIN_FRAME_1); // Debut de trame
		_serial->ecrireOctet(BYTE_BEGIN_FRAME_2);
		_serial->ecrireOctet(BYTE_BEGIN_FRAME_3);
		_serial->ecrireOctet(BYTE_BEGIN_FRAME_4_NORMAL);

		uint16_t msg_size = f.getNbDonnees();
		_serial->ecrireOctets(reinterpret_cast<uint8_t*>(&msg_size), 2);

		_serial->ecrireOctets(f.getDonnees(), msg_size);

		// Temporisation pour ne pas saturer l'électronique
		sleep(_delay);
	}

	template <typename ParsingClass>
	GlobalFrame Communicator<ParsingClass>::recv_frame_blocking(const std::atomic_bool& running_execution) {
		static uint8_t buf[GlobalFrame::DONNEES_TRAME_MAX];
		while(running_execution) {
			// Début de la trame
			while(_serial->lireOctet() != BYTE_BEGIN_FRAME_1) {
				if(!running_execution)
					break;
			}

			if(_serial->lireOctet() == BYTE_BEGIN_FRAME_2) {
				if(_serial->lireOctet() == BYTE_BEGIN_FRAME_3) {
					if(uint8_t frame_type = _serial->lireOctet(); frame_type == BYTE_BEGIN_FRAME_4_NORMAL) {
						uint16_t msg_size;
						_serial->lireOctets(reinterpret_cast<uint8_t*>(&msg_size), 2);

						_serial->lireOctets(buf, msg_size);
						GlobalFrame received_frame{msg_size, buf};
						if(_debug_active) {
							logDebug("Communicator.RECV ", received_frame, "\ntime: ", _chrono.getElapsedTime(), "\n");
						}
						return received_frame;
					} else if(frame_type == BYTE_BEGIN_FRAME_4_PING) {
						// TODO
						throw std::runtime_error("Ping not implemented yet.");
					}
				}
			}
		}
		throw std::runtime_error("Abort of the reception of frames.");
	}

	template <typename ParsingClass>
	void Communicator<ParsingClass>::set_delay(Duration delay) {
		_delay = delay;
	}

	template <typename ParsingClass>
	void Communicator<ParsingClass>::set_debug(bool active) {
		_debug_active = active;
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

		auto input_function = [this](std::atomic_bool& running_execution) {
			while(running_execution) {
				try {
					auto frame = this->recv_frame_blocking(running_execution);
					if(_debug_active) {
						logDebug0("Communicator.communication.RECV ", frame, "\ntime: ", _chrono.getElapsedTime(), "\n");
					}
					_parser->read_frame(frame);
				} catch(std::runtime_error& e) {
					logError("Failed to update the state of the module manager!!");
					logError("Exception caught: ", e.what());
				}
			}
		};

		auto output_function = [this](std::atomic_bool& running_execution) {
			while(running_execution) {
				std::optional<GlobalFrame> frame = _parser->write_frame();
				if(frame == std::nullopt) {
					if(_debug_active) {
						logDebug0("Communicator.communication.SEND [NOTHING]\ntime: ", _chrono.getElapsedTime(), "\n");
					}
					// La classe de parsing peut ne rien avoir à envoyer, donc temporisation sur l'électronique
					sleep(GLOBAL_CONSTANTS.get_default_communication_delay());
				} else {
					if(_debug_active) {
						logDebug0("Communicator.communication.SEND ", frame.value(), "\ntime: ", _chrono.getElapsedTime(), "\n");
					}
					// Trame à envoyer
					this->send_frame(frame.value());
				}
			}
		};

		std::thread in = std::thread(input_function, std::ref(_connected));
		std::thread out = std::thread(output_function, std::ref(_connected));

		in.join();
		out.join();
	}
} // namespace Communication
