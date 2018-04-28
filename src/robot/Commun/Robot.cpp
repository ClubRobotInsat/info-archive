//
//  Robot.cpp
//  Club Robot
//
//  Created by Rémi on 07/06/2015.
//

#include "Robot.h"
#include "NullCommunicator.h"
#include "Strategie.h"

namespace Commun {
	std::vector<std::string> Robot::makeArgs(const char* typeConnexion, const char* adresse, int port) {
		std::vector<std::string> result = {"Simulateur", typeConnexion};
		if(typeConnexion == "RS232"s) {
			result.emplace_back(adresse);
		} else if(typeConnexion == "TCPIP"s) {
			result.emplace_back(adresse);
			result.push_back(std::to_string(port));
		}

		return result;
	}

	/// Initialise le robot a partir des arguments passes au programme.
	Robot::Robot(std::unique_ptr<ConstantesCommunes> constantesCommunes,
	             std::unique_ptr<ConstantesRobot> constantes,
	             std::vector<std::string> const& args,
	             Speed vitesseDeplacement,
	             AngularSpeed vitesseAngulaire)
	        : _constantesCommunes(std::move(constantesCommunes))
	        , _constantes(std::move(constantes))
	        , _connecte(false)
	        , _modeConnexion(ModeConnexion::OTHERS)
	        , _vitesseDeplacement(vitesseDeplacement)
	        , _vitesseAngulaire(vitesseAngulaire) {
		logInfo("Initialisation robot");

		this->connecter(args);
	}

	/// Finalise le robot
	Robot::~Robot() {
		if(_connecte) {
			_executionEnCours.store(false);
			_reception.join();
		}
	}

	CAN& Robot::getCAN() {
		return *_busCAN;
	}

	/// Connexion a la main a partir des arguments passes au programme.
	void Robot::connecter(std::vector<std::string> const& args) {
		logDebug9("Connection robot...");
		for(size_t i = 1; i < args.size() /*&& !_connecte*/; ++i) {
			// - RS232 :
			if(args[i] == "RS232") {
				if((args.size() - 1) - i < 1) {
					logError("Utilisation avec RS232 : \"", args[0], " RS232 /dev/ttyUSB0\"\n");
					exit(EXIT_FAILURE);
				} else {
					_busCAN.reset(new CAN(std::make_unique<RS232>(args[i + 1])));
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
			// - LOCAL :
			else if(args[i] == "LOCAL") {
				logDebug9("Initialisation de la connexion au CAN local");
				_busCAN = std::make_unique<CAN>(std::make_unique<TCPIP>("127.0.0.1", _constantes->getPortTCPIPDefault()));
				_connecte = true;
				_modeConnexion = ModeConnexion::LOCAL;
			} else if(args[i] == "NULL") {
				logDebug9("Initialisation de la connexion au CAN local");
				_busCAN = std::make_unique<CAN>(std::make_unique<NullCommunicator>());
				_connecte = true;
			} else if(args[i] == "SIMU") {
				logDebug9("Initialisation socket client côté robot");
				_socketSimu = std::make_unique<Socket>(SockProtocol::TCP);
				_socketSimu->connect("127.0.0.1", _constantes->getPortTCPIPDefault());
				_simuConnected = true;
			}
		}

		// Cas où l'on n'a rien trouvé indiquant comment se connecter :
		if(!_connecte) {
			logInfo("Utilisation : \n");
			logInfo("- ", args[0], " RS232 [peripherique] (ex : \"", args[0], " RS232 /dev/ttyUSB0\")");
			logInfo("- ", args[0], " TCPIP [adresse IP] [port] (ex : \"", args[0], " TCPIP 127.0.0.1 1234\")");
			logInfo("- ", args[0], " LOCAL --color [color]");
			logInfo("Ajouter SIMU pour établir une connection avec le socket du simu.");
			exit(EXIT_FAILURE);
		}

		_reception = std::thread(&Robot::traiterMessage, this);
	}

	/// Retourne un pointeur vers la carte d'ID donne
	Carte& Robot::getCarteWithID(uint8_t id) {
		// Exceptions retournées, sinon gros segfault ou comportement imprévisible en vue.

		if(id >= Trame::NB_CARTES_MAX) {
			logError("Demande de la carte ", (int)id, ", ID trop grand !\n");
			throw std::runtime_error("ID trop grand !");
		} else if(_cartes[id] == nullptr) {
			logError("Demande de la carte ", (int)id, ", cette carte n'est pas initialisée !\n");
			throw std::runtime_error("La carte n'existe pas !");
		}

		return *_cartes[id];
	}

	/// Debug du CAN pour les elecs
	void Robot::setDebugCAN(bool active) {
		_busCAN->setDebug(active);
	}

	/// Envoie aux bonnes cartes les messages recus pour traitement
	void Robot::traiterMessage() {
		setThreadName("TraiterMessage");
		std::unique_lock<std::mutex> lk(_mutexTraiterMessages);

		if(!_cartesInitNotified)
			_cartesInitialisees.wait(lk);

		while(_executionEnCours) {
			Trame trame = _busCAN->recevoirTrameBloquant();
			try {
				this->getCarteWithID(trame.getId()).traiterMessage(trame);
			} catch(std::exception& e) {
				logError("Carte invalide !!!");
				logError("Exception rencontrée : ", e.what());
				logError("Id demandé : ", (int)trame.getId());
			}
		}
	}

	Speed Robot::getVitesseDeplacement() {
		std::lock_guard<std::mutex> lk(_mutexVitesse);
		return _vitesseDeplacement;
	}

	AngularSpeed Robot::getVitesseAngulaire() {
		std::lock_guard<std::mutex> lk(_mutexVitesse);
		return _vitesseAngulaire;
	}

	void Robot::ajouterCarte(std::unique_ptr<Carte> carte) {
		if(carte->getId() >= Trame::NB_CARTES_MAX) {
			logError("Demande de la carte ", carte->getId(), ", ID trop grand !\n");
		}

		if(_cartes[carte->getId()]) {
			logError("Double assignation de la carte ", carte->getId(), " !\n");
		}

		_cartes[carte->getId()] = std::move(carte);
	}

	Vector2m Robot::getPositionTourelle() const {
		return {0_m, 0_m};
	}

	bool Robot::isSimuConnected() {
		return this->_simuConnected;
	}

	Socket& Robot::getSocketSimu() {
		return *_socketSimu;
	}
}
