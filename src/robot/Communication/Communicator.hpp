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
				}

				_serial = std::make_unique<RS232>(args[i + 1]);
				this->set_delay(10_ms); // Temporisation de 10 ms entre chaque trame pour le bus CAN
				// électronique (pas de tempo pour le simu)
				_connected = true;
				_protocol = CommunicationProtocol::SERIAL_RS232;

				break;
			}

			// - TCPIP :
			else if(args[i] == "TCPIP") {
				if((args.size() - 1) - i < 2) {
					logError("Utilisation avec TCPIP : \"", args[0], " TCPIP 127.0.0.1 1234\"\n");
					exit(EXIT_FAILURE);
				}
				_serial = std::make_unique<TCPIP>(args[i + 1], std::stoi(args[i + 2]));
				_connected = true;
				_protocol = CommunicationProtocol::SERIAL_TCPIP;

				break;
			}

			// - UDP :
			else if(args[i] == "UDP") {
				if((args.size() - 1) - i < 3) {
					logError("Utilisation avec UDP : \"", args[0], " UDP [@IP] [port local] [port distant]\"\n");
					exit(EXIT_FAILURE);
				}
				_serial = std::make_unique<UDP>(args[i + 1], std::stoi(args[i + 2]), std::stoi(args[i + 3]));
				_connected = true;
				_protocol = CommunicationProtocol::SERIAL_UDP;

				break;
			}

			// - PIPES :
			else if(args[i] == "PIPES") {
				logDebug9("Initialisation de la connection au CAN local par pipes nommés");
				_serial = std::make_unique<NamedPipe>("/tmp/read.pipe", "/tmp/write.pipe");
				_connected = true;
				_protocol = CommunicationProtocol::SERIAL_PIPES;

				break;
			}

			// - ETHERNET :
			// TODO : voir comment ajouter les connexions UDP ; garder ce vector<string> ou utiliser une struct d'init ?
			else if(args[i] == "ETHERNET") {
				if((args.size() - 1) - i < 4) {
					// FIXME : on doit fournir une unique connexion UDP actuellement
					logError("Utilisation **temporaire** avec ETHERNET : \"", args[0], " ETHERNET [ID] [@IP] [port local] [port distant]\"\n");
					exit(EXIT_FAILURE);
				}
				_serial = std::map<uint8_t, std::unique_ptr<UDP>>();
				std::get<std::map<uint8_t, std::unique_ptr<UDP>>>(_serial)[std::stoi(args[i + 1])] =
				    std::make_unique<UDP>(args[i + 2], std::stoi(args[i + 3]), std::stoi(args[i + 4]));

				_connected = true;
				_protocol = CommunicationProtocol::ETHERNET;

				break;
			}

			// - LOCAL :
			else if(args[i] == "LOCAL") {
				logDebug9("Initialisation de la connexion au CAN local");
				_serial = std::make_unique<TCPIP>("127.0.0.1", GLOBAL_CONSTANTS.get_default_TCPIP_port());
				_connected = true;
				_protocol = CommunicationProtocol::SERIAL_LOCAL;
			}

			// - NULL :
			else if(args[i] == "NULL") {
				logDebug9("Initialisation de la connexion au CAN local par le NullCommunicator");
				_serial = std::make_unique<NullCommunicator>();
				_connected = true;
				_protocol = CommunicationProtocol::SERIAL_NULL;
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

		/// * Si on communique en ethernet, le linkage du module se fait par la connexion UDP
		/// donc on envoie une trame composée de {size, data . . .}
		/// * Sinon, on envoie une trame composée de {0xAC.DC.AB.BA, ID, size, data . . .}
		if(_protocol == ETHERNET) {
			// Suppression de l'octet 0 qui correspond à l'ID ; envoi de tout le reste
			uint8_t msg_size = static_cast<uint8_t>(f.getNbDonnees() - 1);
			get_udp(f.getDonnee(0)).write_bytes(f.getDonnees() + 1, msg_size);
		} else {
			// Dans le cas d'une liaison série par UDP, on ne peut pas écrire octet par octet (envoi d'un seul message)
			uint16_t msg_size = f.getNbDonnees();
			GlobalFrame msg{BYTE_BEGIN_FRAME_1, BYTE_BEGIN_FRAME_2, BYTE_BEGIN_FRAME_3, BYTE_BEGIN_FRAME_4_NORMAL, static_cast<uint8_t>(msg_size)};
			msg += f;
			get_serial().write_bytes(msg.getDonnees(), msg.getNbDonnees());

			/*
			TODO : supprimer ce bloc ; ça ne marche que pour des liaisons séries en flux d'octets, pas en mode datagrams
			get_serial().write_byte(BYTE_BEGIN_FRAME_1); // Debut de trame
			get_serial().write_byte(BYTE_BEGIN_FRAME_2);
			get_serial().write_byte(BYTE_BEGIN_FRAME_3);
			get_serial().write_byte(BYTE_BEGIN_FRAME_4_NORMAL);

			uint16_t msg_size = f.getNbDonnees();
			get_serial().write_byte(static_cast<uint8_t>(msg_size));
			//_serial->write_bytes(reinterpret_cast<uint8_t*>(&msg_size), 2);

			get_serial().write_bytes(f.getDonnees(), msg_size);*/

			// Temporisation pour ne pas saturer l'électronique
			sleep(_delay);
		}
	}

	template <typename ParsingClass>
	GlobalFrame Communicator<ParsingClass>::recv_frame_blocking(const std::atomic_bool& running_execution) {
		if(_protocol == ETHERNET) {
			throw std::runtime_error("Ethernet communication set up, not serial-only communication.");
		}

		static uint8_t buf[GlobalFrame::DONNEES_TRAME_MAX];

		while(running_execution) {
			if(_protocol == CommunicationProtocol::SERIAL_UDP) {
				size_t msg_size = get_serial().read_bytes(buf, GlobalFrame::DONNEES_TRAME_MAX);
				// Les 5 premiers octets correspondent à `0xAC DC AB BA` <size>
				if(msg_size < 5) {
					continue;
				}
				return GlobalFrame{static_cast<uint8_t>(msg_size - 5), buf + 5};
			} else {
				// Début de la trame
				while(get_serial().read_byte() != BYTE_BEGIN_FRAME_1) {
					if(!running_execution)
						break;
				}

				if(get_serial().read_byte() == BYTE_BEGIN_FRAME_2) {
					if(get_serial().read_byte() == BYTE_BEGIN_FRAME_3) {
						if(uint8_t frame_type = get_serial().read_byte(); frame_type == BYTE_BEGIN_FRAME_4_NORMAL) {
							/*uint16_t msg_size;
							_serial->lireOctets(reinterpret_cast<uint8_t*>(&msg_size), 2);*/
							uint8_t msg_size = get_serial().read_byte();

							get_serial().read_bytes(buf, msg_size);
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
		}
		throw std::runtime_error("Abort of the reception of frames.");
	}

	template <typename ParsingClass>
	void Communicator<ParsingClass>::ethernet_recv(const std::atomic_bool& running_execution, uint8_t id) {
		if(_protocol != ETHERNET) {
			throw std::runtime_error("Serial communication set up, not ethernet communication.");
		}

		static uint8_t buf[GlobalFrame::DONNEES_TRAME_MAX];

		auto& udp = get_udp(id);

		while(running_execution) {
			try {
				// Lecture du message
				uint8_t msg_size = static_cast<uint8_t>(udp.read_bytes(buf, GlobalFrame::DONNEES_TRAME_MAX));
				GlobalFrame received_frame{msg_size, buf};
				logDebug("Communicator.ethernet.RECV ", received_frame, "\ntime: ", _chrono.getElapsedTime(), "\n");

				_parser->read_frame(received_frame);
			} catch(std::runtime_error& e) {
				logError("Failed to recv an ethernet message to update the module ", id);
				logError("Exception caught: ", e.what());
			}
		}
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
					this->send_frame(frame.value());
				}
			}
		};

		std::thread out = std::thread(output_function, std::ref(_connected));

		if(_protocol == ETHERNET) {
			const std::size_t nbr_serials = std::get<std::map<uint8_t, std::unique_ptr<UDP>>>(_serial).size();
			std::thread in[nbr_serials];
			std::size_t i = 0;
			for(auto& v : std::get<std::map<uint8_t, std::unique_ptr<UDP>>>(_serial)) {
				in[i++] = std::thread(&Communicator<ParsingClass>::ethernet_recv, this, std::ref(_running_execution), v.first);
			}

			for(i = 0; i < nbr_serials; ++i) {
				in[i].join();
			}
		} else {
			auto input_function = [this](std::atomic_bool& running_execution) {
				while(running_execution) {
					try {
						auto frame = this->recv_frame_blocking(running_execution);
						_parser->read_frame(frame);
					} catch(std::runtime_error& e) {
						logError("Failed to update the state of the module manager!!");
						logError("Exception caught: ", e.what());
					}
				}
			};

			std::thread in = std::thread(input_function, std::ref(_connected));

			in.join();
		}
		out.join();
	}

	template <typename ParsingClass>
	Serial& Communicator<ParsingClass>::get_serial() const {
		if(_protocol == ETHERNET) {
			throw std::invalid_argument("The communicator is set up to an ethernet communication.");
		}
		return *std::get<std::unique_ptr<Serial>>(_serial);
	}

	template <typename ParsingClass>
	UDP& Communicator<ParsingClass>::get_udp(uint8_t id) const {
		if(_protocol != ETHERNET) {
			throw std::invalid_argument("The communicator isn't set up to an ethernet communication.");
		}
		return *std::get<std::map<uint8_t, std::unique_ptr<UDP>>>(_serial).find(id)->second;
	}
} // namespace Communication
