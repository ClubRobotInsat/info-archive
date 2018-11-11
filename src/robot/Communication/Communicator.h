//
// Created by terae on 16/10/18.
//

/// Cette classe a un double objectif :
/// * définir le protocole de communication entre le `ModuleManager` et l'électronique
/// * assurer cette communication en s'appuyant sur une connexion série

#ifndef ROOT_COMMUNICATOR_H
#define ROOT_COMMUNICATOR_H

#include "ParsingClassChecker.hpp"

// Connexions séries disponibles
#include "NamedPipe.h"
#include "NullCommunicator.h"
#include "RS232.h"
#include "Serial.h"
#include "TCPIP.h"
#include "UDP.h"

#include <Units/Time.h>                // Duration
#include <communication/GlobalFrame.h> // GlobalFrame

#include <atomic>             // atomic_bool
#include <condition_variable> // condition_variable
#include <memory>             // unique_ptr
#include <mutex>              // mutex, lock_guard
#include <variant>            // variant

namespace Communication {

	/// @tparam ParsingClass N'importe quelle classe qui lit et écrit des flux d'octets
	template <typename ParsingClass>
	class Communicator {
	public:
		/// Initialise et définit la connexion série
		explicit Communicator(std::shared_ptr<ParsingClass> parser);

		/// Finalise la connexion
		~Communicator();

		/// Connexion au médiumm spécifié et début de la communication
		bool connect(const std::vector<std::string>& args);

		/// Arrêt de la communication et déconnexion du médium
		void disconnect();

		/// Notification par le constructeur du robot physique que la communication peut démarrer
		void set_modules_initialized();

		///----- Paramétrisation de la connexion physique -----///

		/// Définit le temps d'attente entre chaque envoi de trame
		void set_delay(Duration);

		/// Active ou désactive l'affichage de débug des trames envoyées et reçues
		void set_debug(bool active);

	private:
		/// Envoi une trame sur le médium de communication
		void send_frame(const GlobalFrame& f);

		/// Attends l'arrivée dune trame complète tant que la communication est maintenue
		/// Cette fonction n'est utilisée que pour une communication série, pas pour de l'ethernet
		GlobalFrame recv_frame_blocking(const std::atomic_bool& running_execution);

		/// Pour chaque communication UDP, attends l'arrivée d'une trame puis mets à jour le module correspondant à l'id
		void ethernet_recv(const std::atomic_bool& running_execution, uint8_t id);

		/// Valeurs constantes de certains octets des trames
		enum {
			BYTE_BEGIN_FRAME_1 = 0xac,
			BYTE_BEGIN_FRAME_2 = 0xdc,
			BYTE_BEGIN_FRAME_3 = 0xab,
			BYTE_BEGIN_FRAME_4_NORMAL = 0xba,
			BYTE_BEGIN_FRAME_4_PING = 0xbb,
		};

		/// Chronomètre initialisé au début de la connexion série, utile pour débuguer les messages
		StopWatch _chrono;

		/// Mutex permettant d'assurer la serialisation correcte des donnees
		std::mutex _mutex_write;
		/// Une unique instance de communication peut être lancée à la fois
		std::mutex _mutex_communication;
		/// Processus de réception des trames
		std::thread _reception;

		/// Protocole de communication avec l'électronique
		enum CommunicationProtocol {
			// Communications séries en one-to-one, avec un header `0xAC DC AB BA` devant les trames
			SERIAL_LOCAL,
			SERIAL_NULL,
			SERIAL_PIPES,
			SERIAL_RS232,
			SERIAL_TCPIP,
			SERIAL_UDP,

			// Communication sur un réseau IP en UDP, la couche 3 s'occupe de l'addressage des cartes
			ETHERNET,
		} _protocol;

		/// Médium de communication avec l'électronique ; le `variant` dépend du protocol en vigueur
		/// Si on est en ETHERNET, cette variable représente le mapping entre les IDs des modules et la connexion UDP
		std::variant<std::unique_ptr<Serial>, std::map<uint8_t, std::unique_ptr<UDP>>> _serial;

		/// Helper pour récupérer une connexion série
		Serial& get_serial() const;

		/// Helper pour récupérer la connexion UDP du réseau ethernet associée à l'id
		UDP& get_udp(uint8_t id) const;

		/// Instance d'une classe qui lit et écrit des trames de communication pour y appliquer les messages
		std::shared_ptr<ParsingClass> _parser;

		std::atomic_bool _connected, _running_execution;
		std::condition_variable _modules_initialized;
		std::atomic_bool _modules_init_notified;

		/// Temps à attendre maximum entre chaque envoi de trame
		Duration _delay;

		/// Mode débug actif ou non
		bool _debug_active;

		/// Si `ParsingClass`  ne possède pas les fonctions `[read|write]_frame`,
		/// la communication avec les élecs est désactivée
		/// (le code ne compile pas si on appelle cette méthode dans un thread)
		/// Cette fonction possède définit le protocole de communication avec les élecs
		/// (savoir quand envoyer les trames, s'il y a du rejeu, ...)
		template <typename = typename std::enable_if_t<parses_frames<ParsingClass>::value>>
		void communicate_with_elecs();
	};
} // namespace Communication

#include "Communicator.hpp"

#endif // ROOT_COMMUNICATOR_H
