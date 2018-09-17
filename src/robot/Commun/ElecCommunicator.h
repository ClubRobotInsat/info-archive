//
// Created by terae on 17/09/18.
//

#ifndef ROOT_ELEC_COMMUNICATOR_H
#define ROOT_ELEC_COMMUNICATOR_H


#include "Modules/ModuleManager.h"

#include "Communication/CAN.h"
#include "Communication/NamedPipe.h"
#include "Communication/RS232.h"
#include "Communication/Serie.h"
#include "Communication/TCPIP.h"

#include <condition_variable>
#include <log/Log.h>
#include <sockets/Socket.h>
#include <thread>

namespace Commun {

	enum class ModeConnexion { RS232, LOCAL, PIPES, OTHERS };

	class ElecCommunicator {
	public:
		static std::vector<std::string> makeArgs(const char* typeConnexion, const char* adresse, int port);

		ElecCommunicator(std::shared_ptr<ModuleManager>, uint16_t default_port_TCPIP);
		ElecCommunicator(ModuleManager&, uint16_t default_port_TCPIP);

		~ElecCommunicator();

		// getter pour le CAN -> utile pour réinitialiser le numéro de séquence à la sychronisation avec l'élec
		CAN& getCAN();
		/// Debug du CAN pour les elecs
		void setDebugCAN(bool active);

		bool isSimuConnected();
		Socket& getSocketSimu();

		const ModeConnexion& getModeConnexion() const {
			return _modeConnexion;
		}

		bool connect(std::vector<std::string> const& args);

		void disconnect();

	private:
		std::shared_ptr<ModuleManager> _module_manager;

		uint16_t _default_port_TCPIP;

		/// Bus CAN à utiliser pour communiquer avec l'électronique
		std::unique_ptr<CAN> _busCAN;

		std::unique_ptr<Socket> _socketSimu;

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
		void communicate_with_elecs();
	};
} // namespace Commun

#endif // ROOT_ELEC_COMMUNICATOR_H
