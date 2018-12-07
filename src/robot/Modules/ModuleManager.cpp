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

	std::optional<std::vector<GlobalFrame>> ModuleManager::write_frame() const {
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
			// logWarn("The module n°" + std::to_string(id) + " isn't initialized: frame dropped.");
			return;
		}

		// Tout est Ok, mise à jour du module
		_modules[id]->update(GlobalFrame(size, array + 1));
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
