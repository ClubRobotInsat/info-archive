//
// Created by terae on 8/16/18.
//

#ifndef ROOT_MODULEMANAGER_H
#define ROOT_MODULEMANAGER_H

#include "Module.hpp"
#include <memory>

// TODO: receive data + read data for each module

namespace Commun {
	class ModuleManager final {
	public:
		ModuleManager(CAN& can);

		/// Envoie un ping à la carte
		void sendPing();

		/// Verifie que la carte a répondu par un pong
		bool verifyPong();

		bool pingPong();

		void sendState();

	protected:
		// Transmet les données pour envoi vers l'électronique
		void sendMessage(GlobalFrame frame, bool replay = true);

		// TODO : voir comment envoyer un PING
		/*void sendMessage(uint8_t donnee) {
		    this->sendMessage(this->make_trame(cmd, donnee), false);
		}*/

		GlobalFrame make_state_frame() const;

		void addModule(std::unique_ptr<BaseModule>);

	private:
		std::unique_ptr<BaseModule> _modules[GlobalFrame::NB_MODULES_MAX];

		/// Bus CAN à utiliser pour communiquer avec l'électronique
		CAN& _busCAN;

		std::atomic_bool _pongRecu;
	};
}

#endif // ROOT_MODULEMANAGER_H
