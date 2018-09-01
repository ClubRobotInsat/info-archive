//
// Created by terae on 8/16/18.
//

#include "ModuleManager.h"
#include <log/Log.h>

namespace Commun {
	ModuleManager::ModuleManager(Commun::CAN& can) : _busCAN(can) {}

	/// Verifie que la carte a répondu par un pong
	bool ModuleManager::verifyPong() {
		// Si _pongRecu, _pongRecu = false. On retourne l'acienne valeur dans tous les cas
		return _pongRecu.exchange(false);
	}

	bool ModuleManager::pingPong() {
		this->sendPing();
		int retries = 0;
		while(retries <= 150) {
			if(this->verifyPong()) {
				return true;
			}
			retries += 1;
			sleep(10_ms);
		}
		return false;
	}

	void ModuleManager::sendState() {
		this->sendMessage(make_state_frame());
	}

	/// Transmet les données pour envoi vers l'électronique
	void ModuleManager::sendMessage(GlobalFrame frame, bool replay) {
		// TODO : modifs sur le CAN pour accepter n'importe quelle trame
		//_busCAN.envoyerTrame(std::move(frame), replay);
	}

	GlobalFrame ModuleManager::make_state_frame() const {
		GlobalFrame f;
		uint16_t list_modules = 0;
		for(int i = 0; i < GlobalFrame::NB_MODULES_MAX; ++i) {
			list_modules |= (bool)_modules[i];
			list_modules <<= 1;
		}

		uint8_t max8 = std::numeric_limits<uint8_t>::max();
		f.addByte(static_cast<uint8_t>((max8 - ((0xff00 & list_modules) >> 8)) ^ max8));
		f.addByte(static_cast<uint8_t>((max8 - list_modules) ^ max8));

		for(int i = 0; i < GlobalFrame::NB_MODULES_MAX; ++i) {
			if(_modules[i]) {
				f.addByte(_modules[i]->get_frame_size());
			}
		}

		for(int i = 0; i < GlobalFrame::NB_MODULES_MAX; ++i) {
			if(_modules[i]) {
				f += _modules[i]->make_frame();
			}
		}

		return f;
	}

	void ModuleManager::addModule(std::unique_ptr<Commun::BaseModule> module) {
		if(module->get_id() >= GlobalFrame::NB_MODULES_MAX) {
			logError("Demande du module ", module->get_id(), ", ID trop grand !\n");
		} else if(_modules[module->get_id()]) {
			logError("Double assignation de la carte ", module->get_id(), " !\n");
		} else {
			_modules[module->get_id()] = std::move(module);
		}
	}
}
