// Communicateur.cpp

#include "Communicateur.h"

#include "log/Log.h"

// constructeur
Communicateur::Communicateur(const std::string& port, ITrameListener& robot)
        : _robot(robot), _connecte(false), _port(port) {
	// lève ErreurOuvertureCommunication si impossible d'ouvrir la connexion
	_cleaningUp = false;
}

// destructeur
Communicateur::~Communicateur() {}

// signaler une connexion
void Communicateur::connexion() {
	_connecte = true;
}

// signaler une deconnexion
void Communicateur::deconnexion() {
	_connecte = false;
}

// lance le thread de réception (appelé par les classes filles)
void Communicateur::commencerReception() {
	_reception = std::thread(std::bind(&Communicateur::threadReception, this));
}

// attend le thread de réception
void Communicateur::attendreFinReception() {
	_reception.join();
}

// lors de l'envoi d'une trame l'envoyer octet par octet dans le canal de communication
void Communicateur::onEnvoye(Trame const& t, bool isAck) {
	// cette trame sera détruite apres traitement
	if(this->estConnecte()) {
		std::lock_guard<std::mutex> lockGuard(_mutexTrame);
		try {
			this->envoyer(Trame::OCTET_DEBUT_TRAME_1); // Octet : début de trame
			this->envoyer(Trame::OCTET_DEBUT_TRAME_2); // Octet : début de trame
			this->envoyer(Trame::OCTET_DEBUT_TRAME_3); // Octet : début de trame
			if(!isAck) {
				this->envoyer(Trame::OCTET_DEBUT_TRAME_4_NORMAL); // Octet : début de trame
			} else {
				this->envoyer(Trame::OCTET_DEBUT_TRAME_4_ACK); // Octet : début de trame
			}

			if(isAck) {
				this->envoyer(t.getNumPaquet());
			}

			auto val = Trame::multiplexIdAndCmd(t.getId(), t.getCmd());
			this->envoyer(val.first);        // Octet : ID poids faible + Commande
			this->envoyer(val.second);       // Octet : ID poids fort + Commande
			this->envoyer(t.getNbDonnees()); // Octet : taille des données

			for(int i = 0; i < t.getNbDonnees(); i++) // Octets : données
				this->envoyer(t.getDonnee(i));

			// logDebug2("Envoi de la trame : ", t->toString());
		} catch(ErreurEmissionCommunication e) {
			logError(e.what());
		}
	} else
		logError("erreur d'envoi de trame : le client n'est pas encore connecte");
}

bool Communicateur::lireOctets(uint8_t& id_cmd, uint8_t& idFort, uint8_t& nbDonnees, uint8_t* donnees, uint8_t& num_paquet) {
	uint8_t numDonnee, octet;

	// NB : un appel à that->lire() est susceptible de changer l'état de "connecté"
	// à "déconnecté". Si l'on est déconnecté, lire() renvoie 0.

	// attendre l'octet début de trame (ignorer les autres)
	while(this->lire() != Trame::OCTET_DEBUT_TRAME_1 && this->estConnecte())
		;

	if(this->lire() != Trame::OCTET_DEBUT_TRAME_2 || !this->estConnecte())
		return false;
	if(this->lire() != Trame::OCTET_DEBUT_TRAME_3 || !this->estConnecte())
		return false;
	if(this->lire() != Trame::OCTET_DEBUT_TRAME_4_NORMAL || !this->estConnecte())
		return false;

	num_paquet = this->lire(); // numPaquet
	// printf("Num paquet : %d\n", num_paquet);

	// lire la trame octet par octet
	id_cmd = this->lire();
	idFort = this->lire();
	nbDonnees = this->lire(); // NB : au pire, si l'on s'est déconnecté au milieu, nbDonnees == 0
	for(numDonnee = 0; numDonnee < nbDonnees; numDonnee++) {
		octet = this->lire();
		donnees[numDonnee] = octet;
	}
	return true;
}

// thread de reception des trames
void Communicateur::threadReception() {
	uint8_t id_cmd, idFort, nbDonnees, donnees[255], num_paquet;
	setThreadName("Réception messages");

	while(true) {
		logInfo("attente de la demande de connexion...");

		try {
			this->attendreClient();
		} catch(std::runtime_error& e) {
			// Si l'on est en train de forcer la déconnexion, c'est une "bonne excuse" pour lever
			// une exception (ce n'est pas une vraie erreur...).
			if(this->_cleaningUp)
				return;
			else
				throw;
		}

		while(this->estConnecte()) {
			if(!lireOctets(id_cmd, idFort, nbDonnees, donnees, num_paquet)) {
				continue;
			}

			// si le client est toujours connecté, on a normalement reçu une trame correcte,
			// donc on l'envoie au robot.
			if(this->estConnecte()) {
				// envoi de la trame au robot
				Trame trame(Trame::demultiplexId(id_cmd, idFort), Trame::demultiplexCmd(id_cmd, idFort));
				trame.setDonnees(nbDonnees, donnees);
				trame.setNumPaquet(num_paquet);

				// logDebug1("Réception de la trame : ", trame->toString());

				if(this->_robot.onRecu(trame, true)) {
					// logWarn("La trame \"", trame, "\" n'a pas été traitée, ID 0");
					// logDebug3("La trame \"", trame, " \" a été traitée.");
				}
			}
		}
		logInfo("le client s'est déconnecté");
	}
}

// force une déconnexion, et attend la fin du thread de réception.
// Réimplémenté dans les classes filles.
void Communicateur::cleanUp() {
	_cleaningUp = true;
	attendreFinReception();
	deconnexion();
}
