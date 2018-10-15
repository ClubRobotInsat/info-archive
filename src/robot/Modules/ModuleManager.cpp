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

	GlobalFrame ModuleManager::write_frame(uint8_t id) const {
		if(id >= NB_MODULES_MAX) {
			throw std::runtime_error("Impossible to get module n°" + std::to_string(id) + " (> " +
			                         std::to_string(NB_MODULES_MAX) + ").");
		} else if(!has_module(id)) {
			throw std::runtime_error("The module n°" + std::to_string(id) + " doesn't exist.");
		}

		GlobalFrame f{};
		f.addByte(_modules[id]->get_frame_size());
		f.addByte(id);
		f += _modules[id]->make_frame();
		return f;
	}

	void ModuleManager::read_frame(const GlobalFrame& frame) {
		/// Construction d'une trame : <size : u8> <id : u8> <msg : u8*>
		// la taille correspond à celle occupée par le message du module seulement (sans les 2 octets de taille et d'id)
		uint8_t wanted_frame_size = 2;
		if(frame.getNbDonnees() < wanted_frame_size) {
			throw std::runtime_error("Frame does not contain the module's size and id.");
		}
		auto array = frame.getDonnees();

		const uint8_t size = array[0];
		wanted_frame_size += size;
		const uint8_t id = array[1];

		// Vérification que le module existe
		if(!has_module(id)) {
			throw std::runtime_error("The module n°" + std::to_string(id) + " isn't initialized.");
		}
		// Vérification que la taille théorique du module corresponde avec la taille annoncée dans la trame
		if(size != _modules[id]->get_frame_size()) {
			throw std::runtime_error("The size of the module n°" + std::to_string(id) + " does not correspond to the theory (" +
			                         std::to_string(size) + " != " + std::to_string(_modules[id]->get_frame_size()) + ").");
		}

		// Vérification que la trame contienne assez de place pour toutes les données
		if(frame.getNbDonnees() != wanted_frame_size) {
			throw std::runtime_error("Global frame's size does not correspond to its theoretical size.");
		}

		// Tout est Ok, mise à jour du module
		_modules[id]->update(GlobalFrame(size, array + wanted_frame_size - size)); // array + 2
	}

	/*GlobalFrame ModuleManager::write_frame() const {
	    GlobalFrame f;

	    uint16_t list_modules = 0;
	    for(uint8_t i = 0; i < NB_MODULES_MAX; ++i) {
	        list_modules |= has_module(i);
	        list_modules <<= 1;
	    }

	    uint8_t max8 = std::numeric_limits<uint8_t>::max();
	    f.addByte(static_cast<uint8_t>((max8 - ((0xff00 & list_modules) >> 8)) ^ max8));
	    f.addByte(static_cast<uint8_t>((max8 - list_modules) ^ max8));

	    for(uint8_t i = 0; i < NB_MODULES_MAX; ++i) {
	        if(has_module(i)) {
	            f.addByte(_modules[i]->get_frame_size());
	        }
	    }

	    for(uint8_t i = 0; i < NB_MODULES_MAX; ++i) {
	        if(has_module(i)) {
	            f += _modules[i]->make_frame();
	        }
	    }

	    return f;
	}

	void ModuleManager::read_frame(const GlobalFrame& frame) {
	    // 2 octets pour spécifier quels modules sont présents
	    uint8_t wanted_frame_size = 2;
	    if(frame.getNbDonnees() < wanted_frame_size) {
	        throw std::runtime_error("Frame does not contain the modules listing.");
	    }

	    auto array = frame.getDonnees();
	    uint8_t count = 0;

	    std::bitset<16> list_modules = array[count++];
	    list_modules <<= 8;
	    list_modules |= array[count++];

	    // un octet pour chaque module présent pour spécifier la taille de la trame asociée
	    wanted_frame_size += list_modules.count();

	    if(frame.getNbDonnees() < 2 + list_modules.count()) {
	        throw std::runtime_error("Frame does not contain the modules' size.");
	    }

	    for(uint8_t id = 0; id < NB_MODULES_MAX; ++id) {
	        // pour chaque module annoncé dans la trame, on ajoute la taille de leur trame associée
	        if(list_modules[id]) {
	            if(!has_module(id)) {
	                throw std::runtime_error("The module n°" + std::to_string(id) + " isn't initialized.");
	            }
	            uint8_t size = array[count++];
	            if(size != _modules[id]->get_frame_size()) {
	                throw std::runtime_error("The size of the module n°" + std::to_string(id) +
	                                         " does not correspond to the theory (" + std::to_string(size) +
	                                         " != " + std::to_string(_modules[id]->get_frame_size()) + ").");
	            }
	            wanted_frame_size += size;
	        } else if(_modules[id] != nullptr) {
	            throw std::runtime_error("None information on the frame about the module n°" + std::to_string(id));
	        }
	    }

	    if(frame.getNbDonnees() != wanted_frame_size) {
	        throw std::runtime_error("Global frame size does not correspond to its theoretical size.");
	    }

	    // c'est bon, la trame a le bon format ! on peut mettre à jour chaque module
	    for(uint8_t id = 0; id < NB_MODULES_MAX; ++id) {
	        if(has_module(id)) {
	            const uint8_t SIZE = _modules[id]->get_frame_size();
	            _modules[id]->update(GlobalFrame(SIZE, array + SIZE));
	            count += SIZE;
	        }
	    }
	}*/

	uint8_t ModuleManager::get_nb_modules() const {
		uint8_t count = 0;
		for(uint8_t id = 0; id < NB_MODULES_MAX; ++id) {
			count += has_module(id);
		}
		return count;
	}

	BaseModule& ModuleManager::get_module_by_id(uint8_t id) {
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
