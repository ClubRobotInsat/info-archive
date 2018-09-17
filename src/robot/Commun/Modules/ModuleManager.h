//
// Created by terae on 8/16/18.
//

#ifndef ROOT_MODULEMANAGER_H
#define ROOT_MODULEMANAGER_H

#include "Module.hpp"

#include "IO2019.h"
#include "Motors2019.h"
#include "Moving2019.h"
#include "Servos2019.h"

#include <memory>
#include <tuple>
#include <typeinfo>

// TODO: receive data + read data for each module

namespace Commun {
	class ModuleManager final : public std::enable_shared_from_this<ModuleManager> {
	public:
		ModuleManager() = default;

		template <typename Module, typename... T>
		Module& add_module(uint8_t id, T... params);

		template <typename Module>
		bool has_module();

		bool has_module(uint8_t id);

		uint8_t get_nb_modules() const;

		BaseModule& get_module_by_id(uint8_t id);

		template <typename Module>
		Module& get_module();

		GlobalFrame make_state_frame() const;

		void update_all(const GlobalFrame&);

	private:
		std::unique_ptr<BaseModule> _modules[GlobalFrame::NB_MODULES_MAX];
	};

	/////////////////////////////////////////
	/// Définition des fonctions template ///
	/////////////////////////////////////////

	template <typename Module>
	bool ModuleManager::has_module() {
		for(uint8_t i = 0; i < GlobalFrame::NB_MODULES_MAX; ++i) {
			if(_modules[i] != nullptr) {
				if(typeid(Module) == typeid(*_modules[i])) {
					return true;
				}
			}
		}
		return false;
	}

	template <typename Module, typename... T>
	Module& ModuleManager::add_module(uint8_t id, T... params) {
		if(id >= GlobalFrame::NB_MODULES_MAX) {
			throw std::runtime_error("Impossible to add module n°" + std::to_string(id) + " (> " +
			                         std::to_string(GlobalFrame::NB_MODULES_MAX) + ").");
		} else if(_modules[id] != nullptr) {
			throw std::runtime_error("Double assignment of the module n°" + std::to_string(id) + ".");
		} else {
			// Un seul module de chaque type est accepté
			if(has_module<Module>()) {
				throw std::runtime_error("Double assignment of the module type: "s + typeid(Module).name());
			}

			_modules[id] = std::make_unique<Module>(id, params...);
			return dynamic_cast<Module&>(*_modules[id]);
		}
	}

	template <typename Module>
	Module& ModuleManager::get_module() {
		static_assert(std::is_base_of<BaseModule, Module>::value, "The specified module must inherit Module as public.");
		for(uint8_t id = 0; id < GlobalFrame::NB_MODULES_MAX; ++id) {
			if(_modules[id] != nullptr) {
				try {
					return dynamic_cast<Module&>(*_modules[id]);
				} catch(std::bad_cast&) {
				}
			}
		}

		throw std::runtime_error("The module doesn't exist.");
	}
} // namespace Commun

#endif // ROOT_MODULEMANAGER_H
