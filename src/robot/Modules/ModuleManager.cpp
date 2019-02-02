//
// Created by terae on 8/16/18.
//

#include "ModuleManager.h"
#include <bitset>
#include <log/Log.h>

namespace PhysicalRobot {

	bool ModuleManager::has_module(uint8_t id) const {
		return _modules[id] != nullptr;
	}

	std::vector<GlobalFrame> ModuleManager::write_frame() const {
		for(uint8_t id = 0; id < NB_MODULES_MAX; ++id) {
			if(has_module(id)) {
				if(_modules[id]->needs_to_be_shared()) {
					std::vector<GlobalFrame> v;
					auto frames = _modules[id]->make_frames();
					for(const auto& frame : frames) {
						GlobalFrame f{id};
						f += frame;
						v.push_back(std::move(f));
					}
					_modules[id]->reset_state();
					return v;
				}
			}
		}
		return {};
	}

	void ModuleManager::read_frame(const GlobalFrame& frame) {
		/// Construction d'une trame : <id : u8> <msg : u8*>
		// la taille correspond à celle occupée par le message du module seulement (sans les 2 octets de taille et d'id)
		if(frame.getNbDonnees() < 1) {
			throw std::runtime_error("Frame does not contain the module's id.");
		}
		auto array = frame.getDonnees();

		auto size = static_cast<uint8_t>(frame.getNbDonnees() - 1);
		const uint8_t id = array[0];

		// Vérification que le module existe, sinon on drop la trame
		if(!has_module(id)) {
			logWarn("The module n°" + std::to_string(id) + " isn't initialized: frame dropped.");
			return;
		}

		// Tout est Ok, mise à jour du module
		_modules[id]->update(GlobalFrame(size, array + 1));
	}

	uint8_t ModuleManager::get_nb_modules() const {
		uint8_t count = 0;
		for(uint8_t id = 0; id < NB_MODULES_MAX; ++id) {
			count += has_module(id);
		}
		return count;
	}

	std::vector<uint8_t> ModuleManager::get_list_modules() const {
		std::vector<uint8_t> result;
		for(uint8_t id = 0; id < NB_MODULES_MAX; ++id) {
			if(has_module(id)) {
				result.push_back(id);
			}
		}
		return result;
	}

	Module& ModuleManager::get_module_by_id(uint8_t id) {
		if(id >= NB_MODULES_MAX) {
			throw std::runtime_error("Impossible to get module n°" + std::to_string(id) + " (> " +
			                         std::to_string(NB_MODULES_MAX) + ").");
		} else if(!has_module(id)) {
			throw std::runtime_error("The module n°" + std::to_string(id) + " doesn't exist.");
		}

		return *_modules[id];
	}

	void ModuleManager::deactivation() {
		for(uint8_t id = 0; id < NB_MODULES_MAX; ++id) {
			if(has_module(id)) {
				get_module_by_id(id).deactivation();
			}
		}
	}
} // namespace PhysicalRobot
