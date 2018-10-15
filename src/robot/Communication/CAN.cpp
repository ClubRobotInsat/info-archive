#include "CAN.h"
#include <algorithm>
#include <iostream>
#include <log/Log.h>

namespace Communication {

	const Duration CAN::TIMEOUT_MESSAGE = 40_ms;
	const Duration CAN::PERIODE_CHECK_TIMEOUT = 120_ms;
	/// Initialise et definit la connexion reliant au bus CAN
	CAN::CAN(std::unique_ptr<Serie> connexion)
	        : _serie(std::move(connexion)), _temporisation(3_ms), _debugActive(false) {
		reinitialiserNumSequence();
		reinitialiserFenetre();
		_acquittements = std::thread(std::bind(&CAN::gestionAcquittements, this));
	}

	/// Finalise le bus CAN
	CAN::~CAN() {
		_executionEnCours.store(false);
		if(_acquittements.joinable()) {
			_acquittements.join();
		}
	}

	void CAN::gestionAcquittements() {
		setThreadName("Gestion acquittements");
		while(_executionEnCours) {
			sleep(PERIODE_CHECK_TIMEOUT);

			// Renvoie les messages non acquittés
			marquerTimeoutAck();
		}
	}

	bool CAN::ajouterAttenteAcquittement(GlobalFrame frame, bool rejeu) {
		frame.setNumPaquet(_compteurPaquets);

		// Si on ne veut pas renvoyer le message malgré un timout d'acquittement,
		// on renverra un ping.
		if(rejeu == false) {
			// frame.setCmd(0_b); // on met 0 pour le ping
			// frame.setDonnee(0x55_b);
		}

		// On l'ajoute dans la fenêtre
		return saveForAck(std::move(frame));
	}

	void CAN::envoyerTrame(GlobalFrame t, bool rejeu) {
		std::lock_guard<std::mutex> lk(_mutexVector);

		envoyerTrameSansAcquittement(_compteurPaquets, t);
		//~logDebug("[CarteCAN] : Trame envoyée");

		// Attention, on ne peut envoyer la trame qui si la fenêtre glissante
		// n'est pas pleine => l'appel à ajouterAttenteAcquittement est bloquant
		// tant qu'on a pas pu ajouter le message voulu dans la fenêtre
		//~logDebug9("[CarteCAN] : Empilement d'une demande de ack pour le message ", (int)numPaquet);
		ajouterAttenteAcquittement(std::move(t), rejeu);

		//~logDebug9("[CarteCAN] : Demande empilée pour le message ", (int)numPaquet);	//~logDebug("[CarteCAN] :
		// EnvoyerTrame");

		_compteurPaquets = (_compteurPaquets + 1) % RANGE_MSG_NUMBER;
	}

	/// Envoie une trame de commande avec l'ID et les donnees fournis
	void CAN::envoyerTrameSansAcquittement(uint8_t numPaquet, GlobalFrame const& t) {
		if(_debugActive) {
			// logDebug("SEND ", t, "\n\ttime : ", _canClock.getElapsedTime());
		}

		std::lock_guard<std::mutex> lk(_mutexEcriture);

		_serie->ecrireOctet(GlobalFrame::OCTET_DEBUT_TRAME_1); // Debut de trame
		_serie->ecrireOctet(GlobalFrame::OCTET_DEBUT_TRAME_2);
		_serie->ecrireOctet(GlobalFrame::OCTET_DEBUT_TRAME_3);
		_serie->ecrireOctet(GlobalFrame::OCTET_DEBUT_TRAME_4_NORMAL);

		_serie->ecrireOctet(numPaquet);

		// auto muxedIdAndCmd = Trame::multiplexIdAndCmd(t.getId(), t.getCmd());
		//_serie->ecrireOctet(muxedIdAndCmd.first);  // Poids faible de l'ID
		//_serie->ecrireOctet(muxedIdAndCmd.second); // Poids fort de l'ID
		uint16_t nbDonnees = t.getNbDonnees(); // Taille des données
		_serie->ecrireOctets(reinterpret_cast<uint8_t*>(&nbDonnees), 2);
		_serie->ecrireOctets(t.getDonnees(), t.getNbDonnees()); // Données

		sleep(_temporisation); // Temporisation pour ne pas saturer l'électronique
	}

	/*void CAN::envoyerAcquittement(uint8_t numPaquet, Trame const &t) {
	    std::lock_guard<std::mutex> lk(_mutexEcriture);

	    _serie->ecrireOctet(GlobalFrame::OCTET_DEBUT_TRAME_1); // Debut de trame
	    _serie->ecrireOctet(GlobalFrame::OCTET_DEBUT_TRAME_2);
	    _serie->ecrireOctet(GlobalFrame::OCTET_DEBUT_TRAME_3);
	    _serie->ecrireOctet(GlobalFrame::OCTET_DEBUT_TRAME_4_ACK);

	    _serie->ecrireOctet(numPaquet);

	    _serie->ecrireOctet(id & 0x00ff); // Poids faible de l'ID
	    _serie->ecrireOctet(id & 0xff00); // Poids fort de l'ID
	    _serie->ecrireOctet(t.getNbDonnees()); // Taille des donnees

	    msleep(_temporisation); // Temporisation pour ne pas saturer l'electronique
	}*/


	GlobalFrame CAN::recevoirTrameBloquant() {
		std::atomic_bool always_false(false);
		while(true) {
			try {
				return this->recevoirTrame(always_false);
			} catch(GlobalFrame::ErreurTropDeDonnees& t) {
				// logError("Exception attrapée : ", t.what());
			}
		}
	}

	/// Attend l'arrivee d'une trame complete et la retourne - BLOQUANT
	/// Retourne FALSE si la trame est un ack !
	GlobalFrame CAN::recevoirTrame(const std::atomic_bool& abandonner) {
		static uint8_t donnees[GlobalFrame::DONNEES_TRAME_MAX];
		while(!abandonner) {
			// Debut de trame - BLOQUANT !!!
			while(_serie->lireOctet() != GlobalFrame::OCTET_DEBUT_TRAME_1)
				if(abandonner)
					break;

			if(_serie->lireOctet() == GlobalFrame::OCTET_DEBUT_TRAME_2) {
				if(_serie->lireOctet() == GlobalFrame::OCTET_DEBUT_TRAME_3) {
					uint8_t typeTrame = _serie->lireOctet();

					if(typeTrame == GlobalFrame::OCTET_DEBUT_TRAME_4_NORMAL) {
						// TODO : gérer le Ping (module spécifique ?)
						const uint8_t size = _serie->lireOctet();
						const uint8_t id = _serie->lireOctet();
						_serie->lireOctets(donnees, size);
						GlobalFrame f{size, id};
						f += GlobalFrame{size, donnees};
						return f;

						// Trame::MuxedIdAndCmd muxedIdAndCmd = {_serie->lireOctet(),  // Poids faible de l'ID
						//                                      _serie->lireOctet()}; // Poids fort de l'ID
						/*uint16_t taille; // Taille des donnees
						_serie->lireOctets(reinterpret_cast<uint8_t*>(&taille), 2);

						if(taille > GlobalFrame::DONNEES_TRAME_MAX) {
						    // logError("id trame :", int(Trame::demultiplexId(muxedIdAndCmd)), ":",
						int(Trame::demultiplexCmd(muxedIdAndCmd))); throw GlobalFrame::ErreurTropDeDonnees(taille);
						}
						_serie->lireOctets(donnees, taille); // Donnees

						GlobalFrame trameRecue; (Trame::demultiplexId(muxedIdAndCmd),
						Trame::demultiplexCmd(muxedIdAndCmd)) trameRecue.setDonnees(taille, donnees);

						if(_debugActive) {
						    // logDebug("RECV ", trameRecue, "\n\ttime : ", _canClock.getElapsedTime());
						}

						return trameRecue; // La trame est bien formée, on sort de la boucle bloquante*/
					} else if(typeTrame == GlobalFrame::OCTET_DEBUT_TRAME_4_ACK) {
						// TODO : supprimer les ACKs
						uint8_t num_paquet = _serie->lireOctet(); // Numéro de paquet

						// Trame::MuxedIdAndCmd muxedIdAndCmd = {_serie->lireOctet(),  // Poids faible de l'ID
						//                                      _serie->lireOctet()}; // Poids fort de l'ID

						// uint8_t taille = _serie->lireOctet(); // Taille des donnees
						if(_debugActive) {
							/*logDebug("ACK ",
							         num_paquet,
							         " | id ",
							         //Trame::demultiplexId(muxedIdAndCmd),
							         " | cmd ",
							         //Trame::demultiplexCmd(muxedIdAndCmd),
							         " | size ",
							         taille,
							         "\n\ttime : ",
							         _canClock.getElapsedTime());*/
						}

						marquerAckRecu(num_paquet);
						// this->enleverAttenteAcquittement(num_paquet, id, taille);
					}
				}
			}
		}
		throw std::runtime_error("Abandon de la réception d'une trame.");
	}

	/// Definit le temps a attendre (en ms) entre chaque envoi de trame
	void CAN::setTemporisation(Duration duree) {
		_temporisation = duree;
	}

	/// Active ou desactive l'affichage de debug des trames envoyees et recues
	void CAN::setDebug(bool active) {
		_debugActive = active;
	}

	//////////////////////////// Gestion de la fenêtre glissante
	// TODO : attention, cette fonction doit toujours être appelée quand
	// on est en possession du winMutex
	void CAN::decalerFenetre() {
		// On incrémente la première case après la fenêtre de la taille de celle-ci
		// puis on augmente de 1 le pointeur d'entrée dans le buffer circulaire.
		// De cette façon, on s'assure qu'à la fin de la fenêtre on ajoute le prochain numéro
		// de paquet à recevoir
		while(!_tableauAttentesAcks.empty() && _tableauAttentesAcks.front()._ackRecu) {
			//~logDebug9("[CarteCAN] : décalage de la fenêtre de 1 pour intégrer le numéro de séquence ",
			//~		(int)(_window[_entreeWindow].message.numPaquet + WIN_SIZE)%RANGE_MSG_NUMBER,
			//~		"à la place du numéro ",(int) _window[_entreeWindow].message.numPaquet);
			_tableauAttentesAcks.pop_front();
		}
	}

	void CAN::marquerAckRecu(uint8_t numPaquet) {
		//~logDebug9("[CarteCAN] : Ack numéro", (int)numPaquet, " reçu à l'instant ", _canClock.getElapsedTime());
		//~logDebug9("Fenêtre Actuelle : [", (int)numPaquet," : ", (int)(numPaquet+WIN_SIZE-1)%RANGE_MSG_NUMBER,"]");

		bool found = false;
		size_t i = 0;
		std::lock_guard<std::mutex> lk(_mutexVector);

		while(!found && i < _tableauAttentesAcks.size()) {
			if(_tableauAttentesAcks[i]._message.getNumPaquet() == numPaquet) {
				_tableauAttentesAcks[i]._ackRecu = true;
				found = true;
			}
			i++;
		}

		// Si i = 1, on s'est arrêté à la première itération, donc on est en
		// tête de fenêtre et on peut évacuer la demande
		if(i == 1) {
			decalerFenetre();
			//~logDebug9("On décale la fenêtre : [", (int)numPaquet," : ",
			//(int)(numPaquet+WIN_SIZE-1)%RANGE_MSG_NUMBER,"]");
		}
		if(!found && _debugActive) {
			// logWarn("On reçoit un ack pour le message ", (int)numPaquet, ", déjà évacué de la fenêtre");
		}
	}

	void CAN::marquerTimeoutAck() {
		size_t i = 0;
		bool done = false;
		//~logDebug9("[CarteCAN] : Début du parcours pour vérifier les timeout");
		std::lock_guard<std::mutex> lk(_mutexVector);
		while(!done && i < _tableauAttentesAcks.size()) {
			// On est dans une file, donc les messages les plus vieux sont en tête,
			// c'est pour cela qu'on parcours de la tête vers la queue
			if(!_tableauAttentesAcks[i]._ackRecu && _tableauAttentesAcks[i]._sent) {
				// Quand le ack n'a pas été reçu, il faut vérifier si
				// c'est normal ou pas => on vérifie que le temps écoulé
				// depuis l'envoi n'est pas trop grand ou si le timeout
				// n'a pas été déjà déclaré
				if(_tableauAttentesAcks[i]._timeout && !_tableauAttentesAcks[i]._ackRecu) {
					// Pour des raisons de rapidité, on vérifie d'abord si
					// le timeout n'a pas déjà été signalé
					envoyerTrameSansAcquittement(_tableauAttentesAcks[i]._message.getNumPaquet(),
					                             _tableauAttentesAcks[i]._message);

					/*logWarn("[CarteCAN] : Le message ", (int)_tableauAttentesAcks[i]._message._numPaquet,
					        " est en attente inhabituelle de ack à l'instant ",
					        _canClock.getElapsedTime(),", ayant été envoyé à l'instant ",
					        _tableauAttentesAcks[i]._startDate);*/
				} else if(((_canClock.getElapsedTime() - _tableauAttentesAcks[i]._startDate) > TIMEOUT_MESSAGE) &&
				          _tableauAttentesAcks[i]._sent && !_tableauAttentesAcks[i]._ackRecu) {
					// on compare ensuite le temps écoulé depuis l'émission du message au temps de RTT maximum
					_tableauAttentesAcks[i]._timeout = true;
					envoyerTrameSansAcquittement(_tableauAttentesAcks[i]._message.getNumPaquet(),
					                             _tableauAttentesAcks[i]._message);
					/*logWarn("[CarteCAN] : Le message ", (int)_tableauAttentesAcks[i]._message._numPaquet,
					        " passe en attente inhabituelle de ack à l'instant ",
					        _canClock.getElapsedTime(), ", ayant été envoyé à l'instant ",
					        _tableauAttentesAcks[i]._startDate);*/
				} else {
					// dès qu'on rencontre le premier timer qui n'est pas en timeout,
					// puisqu'on est en filo, tous ceux qui suivent sont plus récents
					// et donc pas en timeout, on arrête le bouclage.
					// break;
					done = true;
				}
			}
			i++;
		}
	}

	bool CAN::saveForAck(GlobalFrame m) {
		// On insère l'attente de ack dans la queue de messages en attente

		// Todo : trouver un fix moins déguelasse
		// On sait que l'ID de la carte déplacement est 1 dans nos robots
		// et que la commande pour l'arrêt d'urgence estla commande numéro 5
		/*if(m.getId() == 1 && m.getCmd() == 5) {
		    // dans ce cas, on a envoyé une commande d'arrêt d'urgence
		    logDebug2("Arrêt d'urgence reçu, flush du pipe de messages du CAN");
		    _tableauAttentesAcks.clear();
		}*/
		_tableauAttentesAcks.emplace_back(std::move(m), false, _canClock.getElapsedTime(), false, true);

		return true;
	}

	void CAN::reinitialiserNumSequence() {
		_compteurPaquets = 0;
	}

	void CAN::reinitialiserFenetre() {
		std::lock_guard<std::mutex> lk(_mutexVector);
		_tableauAttentesAcks.clear();
		_canClock.reset();
	}
} // namespace Communication
