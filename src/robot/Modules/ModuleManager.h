/**
 * Cette classe regroupe tous les modules du robot (ils doivent hériter de `Module`)
 * Elle a un double rôle :
 *     - conteneur : on peut rajouter et accéder facilement à n'importe quel module
 *     - interface avec la communication : l'état global du robot est traité ici
 * Pour voir comment l'utiliser, tu peux lire les tests unitaires dessus
 * dans `/test/unit-modules.cpp`, au TEST_CASE("ModuleManager")
 *
 * Voici un petit exemple :
 * ```cpp
 * ModuleManager manager;
 * manager.add_module<Servos2019>(2)
 *         .add_servo(4, 50_deg, Servos2019::BlockingMode::HOLD_ON_BLOCKING);
 * auto& motors = manager.add_module<Motors2019>(3);
 * motors.add_controlled_motor(1, Motors2019::RotatingDirection::TRIGONOMETRIC);
 * motors.add_brushless(2);
 * motors.add_brushless(6);
 *
 * assert(&motors == &manager.get_module_by_id(3));
 * assert(&motors == &manager.get_module<Motors2019>());
 * auto frame = manager.write_frame();
 * manager.read_frame(GlobalFrame{1, 2, 3, 4, ...});
 * ```
 */

#ifndef ROOT_MODULEMANAGER_H
#define ROOT_MODULEMANAGER_H

#include "Module.hpp"

#include "Avoidance2019.h"
#include "IO2019.h"
#include "LED2019.h"
#include "Motors2019.h"
//#include "Moving2019.h"
#include "Servos2019.h"

#include <memory>   // unique_ptr, make_unique
#include <optional> // optional

namespace PhysicalRobot {
	class ModuleManager final {
	public:
		static const uint8_t NB_MODULES_MAX = 16;

		ModuleManager() = default;

		/// Ajoute un module dont l'ID et le type sont uniques
		/// @throw  si les paramètres sont mauvais ou si un module ayant le même id / type existe déjà
		template <typename Module, typename... T>
		Module& add_module(uint8_t id, T... params);

		/// Retourne vrai si un module de type 'Module' existe
		/// @nothrow
		template <typename Module>
		bool has_module() const;

		/// Retourne vrai si un module possède déjà l'ID 'id'
		/// @nothrow
		bool has_module(uint8_t id) const;

		/// Retourne le nombre de modules instantiés
		/// @nothrow
		uint8_t get_nb_modules() const;

		/// Retourne le module selon l'id
		/// Il faut appliquer `dynamic_cast<T&>(module);` pour convertir 'module' vers le type souhaité
		/// @throw si l'id est trop grand ou si le module associé n'existe pas
		BaseModule& get_module_by_id(uint8_t id);

		/// Retourne le module selon le type
		/// @throw si aucun module ayant le bon type n'existe
		template <typename Module>
		Module& get_module();

		/// Écrit une trame pour un module qui nécessite d'envoyer son état à l'électronique
		/// Si aucun module ne doit partager son état dans l'immédiat, retourne std::nullopt
		/// Cette fonction implémente l'API de `ParsingClassChecker`
		// FIXME Vérifier que les modules de grands `id`s peuvent communiquer
		// FIXME sinon utiliser un compteur pour répartir la parole de manière cyclique
		std::optional<GlobalFrame> write_frame() const;
		/// Lecture d'une trame ; implémente l'API de `ParsingClassChecker`
		void read_frame(const GlobalFrame&);

		/*/// Construit la trame globale du robot depuis chaque module instantié
		/// @maythrow si on module n'arrive pas à générer sa trame (très peu probable)
		GlobalFrame write_frame() const;

		/// Met à jour tous les modules depuis une trame qui contient l'état général du robot
		/// @throw si la trame a un mauvais formatage
		void read_frame(const GlobalFrame&);*/

		/// Désactive tous les comportements mécaniques du robot (anciennement 'couperMeca')
		void deactivation();

	private:
		std::unique_ptr<BaseModule> _modules[NB_MODULES_MAX];
	};

	/////////////////////////////////////////
	/// Définition des fonctions template ///
	/////////////////////////////////////////

	template <typename Module>
	bool ModuleManager::has_module() const {
		for(uint8_t i = 0; i < NB_MODULES_MAX; ++i) {
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
		if(id >= NB_MODULES_MAX) {
			throw std::runtime_error("Impossible to add module n°" + std::to_string(id) + " (> " +
			                         std::to_string(NB_MODULES_MAX) + ").");
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
		for(uint8_t id = 0; id < NB_MODULES_MAX; ++id) {
			if(_modules[id] != nullptr) {
				try {
					return dynamic_cast<Module&>(*_modules[id]);
				} catch(std::bad_cast&) {
				}
			}
		}

		throw std::runtime_error("The module doesn't exist.");
	}
} // namespace PhysicalRobot

#endif // ROOT_MODULEMANAGER_H
