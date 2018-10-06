/**
 * Cette classe permets de communiquer avec les élecs
 * Elle est générale et peut communiquer sur n'importe quel médium
 * en ayant n'importe quel objet capable de lire et écrire des `GlobalFrame`s
 *
 * Si l'objet de type ParsingClasse n'implémente pas les fonctions :
 * 	    - `void read_frame(const GlobalFrame&);`
 *      - `GlobalFrame write_frame() const;`
 * alors la méthode `communicate_with_elecs` sera inaccessible
 */

#ifndef ROOT_ELEC_COMMUNICATOR_H
#define ROOT_ELEC_COMMUNICATOR_H

#include "ParsingClassChecker.hpp"

#include "../Modules/ModuleManager.h"

#include "CAN.h"
#include "NamedPipe.h"
#include "RS232.h"
#include "Serie.h"
#include "TCPIP.h"

#include <condition_variable>
#include <log/Log.h>
#include <sockets/Socket.h>
#include <thread>

namespace Communication {

	enum class ModeConnexion { RS232, LOCAL, PIPES, OTHERS };

	template <typename ParsingClass>
	class ElecCommunicator {
	public:
		static std::vector<std::string> makeArgs(const char* typeConnexion, const char* adresse, int port);

		ElecCommunicator(std::shared_ptr<ParsingClass> parser, uint16_t default_port_TCPIP);
		ElecCommunicator(ParsingClass&, uint16_t default_port_TCPIP);

		~ElecCommunicator();

		// getter pour le CAN -> utile pour réinitialiser le numéro de séquence à la sychronisation avec l'élec
		CAN& getCAN();
		/// Debug du CAN pour les elecs
		void setDebugCAN(bool active);

		// bool isSimuConnected();
		// Socket& getSocketSimu();

		const ModeConnexion& getModeConnexion() const {
			return _modeConnexion;
		}

		/// Connexion au médium spécifié et début de la communication
		bool connect(std::vector<std::string> const& args);

		/// Arrêt de la communication et déconnecion du médium
		void disconnect();

		void set_modules_initialized();

	private:
		std::shared_ptr<ParsingClass> _parser;

		uint16_t _default_port_TCPIP;

		/// Bus CAN à utiliser pour communiquer avec l'électronique
		std::unique_ptr<CAN> _busCAN;

		// std::unique_ptr<Socket> _socketSimu;

		/// État du bus CAN de l'electronique
		bool _connecte;

		/// Mode de connexion du CAN : local / RS232
		ModeConnexion _modeConnexion;

		std::condition_variable _modules_initialized;
		std::atomic_bool _modules_init_notified{false};

		/// Indique si la connection au socket du simulateur est réussie
		bool _simuConnected;

		/// Thread de réception des messages
		std::thread _reception;

		std::atomic_bool _running_execution = {true};

		std::mutex _mutex_communication;

		/// Si `ParsingClass`  ne possède pas les fonctions `[read|write]_frame`,
		/// la communication avec les élecs est désactivée
		/// (le code ne compile pas si on appelle cette méthode dans un thread)
		/// Cette fonction possède définit le protocole de communication avec les élecs
		/// (savoir quand envoyer les trames, s'il y a du rejeu, ...)
		template <typename = typename std::enable_if_t<parses_frames<ParsingClass>::value>>
		void communicate_with_elecs();
	};
} // namespace Communication

#include "ElecCommunicator.hpp"

#endif // ROOT_ELEC_COMMUNICATOR_H
