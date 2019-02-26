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
#include "Protocol.h"
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
#include <typeindex>          // std::type_index, typeid
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

		bool connect(const Constants::RobotInitializationData&);

		/// Arrêt de la communication et déconnexion du médium
		void disconnect();

		std::type_index get_protocol_type() const;

		/// Active ou désactive l'affichage de débug des trames envoyées et reçues
		void set_debug(bool active);

	private:
		/// Chronomètre initialisé au début de la connexion série, utile pour débuguer les messages
		StopWatch _chrono;

		/// Une unique instance de communication peut être lancée à la fois
		std::mutex _mutex_communication;
		/// Processus de réception des trames
		std::thread _communication;

		/// Instance d'une classe qui lit et écrit des trames de communication pour y appliquer les messages
		std::shared_ptr<ParsingClass> _parser;

		/// Protocole de communication ; contient la logique d'abstraction du médium pour la lecture et l'écriture
		std::unique_ptr<Protocol> _protocol;

		std::type_index _protocol_type;

		std::atomic_bool _connected;

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
