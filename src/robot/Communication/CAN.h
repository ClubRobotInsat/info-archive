#ifndef _CAN_H_
#define _CAN_H_
/* Pour augmenter la vitesse de calcul côté microcontrôleur
 * (lequel doit avoir une fenêtre de la même taille que
 * du côté robot), on choisit une taille de fenêtre qui
 * soit un multiple de 2 (le modulo peut être fait par
 * un masque)
 */
#define WIN_SIZE 64
#define RANGE_MSG_NUMBER 256
//#define RTT 120
#define TIMEOUT_CAN 2000

#include <exception>

#include <Commun.h>

#include "Serie.h"
#include <Clock.h>
#include <Units/Time.h>
#include <array>
#include <atomic>
#include <communication/GlobalFrame.h>
#include <deque>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

namespace Communication {

	/// Interface d 'acces au bus CAN
	class CAN {
	public:
		/// Initialise et definit la connexion reliant au bus CAN
		CAN(std::unique_ptr<Serie> connexion);

		/// Finalise le bus CAN
		virtual ~CAN();

		/// Envoie une trame de commande avec l'ID et les donnees fournis
		void envoyerTrame(GlobalFrame t, bool rejeu = true);

		///	Attends l'arrrivée d'une trame complète tant que abandonner vaut false.
		/// S'interromps dès que abandonner vaut true.
		GlobalFrame recevoirTrame(const std::atomic_bool& abandonner);

		/// Attend l'arrivée d'une trame complete et la retourne - BLOQUANT
		/// Appelle recevoirTrame() avec un booléen qui vaut toujours true.
		/// Existe toujours pour des raisons de compatibilité
		GlobalFrame recevoirTrameBloquant();

		/// Definit le temps a attendre (en ms) entre chaque envoi de trame
		void setTemporisation(Duration duree);

		/// Active ou desactive l'affichage de debug des trames envoyees et recues
		void setDebug(bool active);

		// En début de match, on appelle cette fonction pour se synchroniser
		// avec le numéro de séquence de l'élec
		void reinitialiserNumSequence();
		void reinitialiserFenetre();

	private:
		struct AttenteDeAck {
			AttenteDeAck(GlobalFrame t, bool a, Duration start, bool timeout, bool sent)
			        : _message(std::move(t)), _ackRecu(a), _startDate(start), _timeout(timeout), _sent(sent) {}
			GlobalFrame _message;
			bool _ackRecu;
			Duration _startDate;
			bool _timeout;
			bool _sent;
		};

		void envoyerTrameSansAcquittement(uint8_t numPaquet, GlobalFrame const& t);
		// void envoyerAcquittement(Trame const &t);

		// Marque dans la fenêtre le fait que le ack numéro nimPaquet a été reçu
		void marquerAckRecu(uint8_t numPaquet);
		// Marque tous les numéros de paquet dont le ack n'a pas été reçu
		void marquerTimeoutAck();
		// Décale la fenêtre de réception de 1 numéro
		void decalerFenetre();
		// Instancie une structure associée à un numéro de paquet
		bool saveForAck(GlobalFrame m);

		bool ajouterAttenteAcquittement(GlobalFrame t, bool rejeu);
		void gestionAcquittements();

		// Temps au bout duquel la non réception du ack d'un message est chelou (en ms)
		static const Duration TIMEOUT_MESSAGE;
		static const Duration PERIODE_CHECK_TIMEOUT;
		StopWatch _canClock;

		std::atomic_bool _executionEnCours = {true};
		/*
		 ** On n'a pas besoin d'utiliser de mutex pour l'accès à cette variable ;
		 ** int a = _compteurPaquets va lire la valeur ;
		 ** _compteurPaquets = a va écrire dans la variable ;
		 ** Le tout se fait comme si un mutex était utilisé, sans interférence entre threads, et potentiellement plus
		 *rapidement qu'avec un mutex
		 */
		std::atomic_int _compteurPaquets;

		std::mutex _mutexVector;
		/// Mutex permettant d'assurer la serialisation correcte des donnees
		std::mutex _mutexEcriture;
		std::mutex _mutexBuffer;

		std::thread _acquittements;

		/// Port de communication serie utilise pour la communication avec le bus CAN
		std::unique_ptr<Serie> _serie;

		// Queue dans laquelle on stocke les messages envoyés en attente de ack
		std::deque<AttenteDeAck> _tableauAttentesAcks;

		/// Temps a attendre (en ms) entre chaque envoi de trame
		Duration _temporisation;

		/// Etat du mode debug
		bool _debugActive = false;

		std::vector<GlobalFrame> _bufferEnvoi;
	};
} // namespace Communication

#endif
