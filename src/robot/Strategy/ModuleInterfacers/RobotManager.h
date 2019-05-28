//
// Created by terae on 23/01/19.
//

#ifndef ROOT_INTERFACERMANAGER_H
#define ROOT_INTERFACERMANAGER_H

#include "AvoidanceInterfacer.h"
#include "ElevatorInterfacer.h"
#include "IOInterfacer.h"
#include "NavigationInterfacer.h"
#include "PumpsInterfacerPrimary.h"
#include "PumpsInterfacerSecondary.h"
#include "ServosInterfacer.h"

#include <typeindex>

namespace Strategy::Interfacer {

	/** @brief Le RobotManager permet de gérer les fonctions de haut niveau d'un robot.
	 *
	 * Là où le `PhysicalRobot::Robot` ne contient que les informations et l'API
	 * nécessaire pour interagir directement avec le robot physique, le RobotManager
	 * contient des `Interfacers` capables d'effectuer des operations plus complexes
	 * (par exemple, aller à un certain point sur la table, effectuer des séquences
	 * complètes d'opérations, etc). */
	class RobotManager {
	public:
		RobotManager(std::shared_ptr<PhysicalRobot::Robot>);

		std::shared_ptr<PhysicalRobot::Robot> get_robot() const;

		size_t size() const {
			return _interfacers.size();
		}

		/**
		 * Insert a new interfacer and builds it from a `PhysicalRobot::Robot` and specific arguments
		 * @tparam Interfacer to build
		 * @tparam T Optional additional arguments for the `Interfacer`. The PhysicalRobot is automatically
		 * added by the RobotManager, you may not include it in the additional arguments.
		 * @return A reference to the built interfacer
		 * @throws If the same `Interfacer` already exists
		 * @todo Check if in some cases we should want several identical interfacers
		 */
		template <typename Interfacer, typename... Args>
		Interfacer& add_interfacer(Args&&... params);

		/**
		 * Helper to know if a specific `Interfacer` is already declared inside the manager
		 * @return `true` if the `Interfacer` exists
		 */
		template <typename Interfacer>
		bool has_interfacer() const;

		/**
		 * Getter on a specific `Interfacer`
		 * @return An `Interfacer` reference if it exists
		 * @throws If the interfacer doesn't exist
		 */
		template <typename Interfacer>
		Interfacer& get_interfacer();

	private:
		std::shared_ptr<PhysicalRobot::Robot> _robot;
		std::unordered_map<std::type_index, std::unique_ptr<AbstractInterfacer>> _interfacers;
	};

	template <typename Interfacer, typename... Args>
	Interfacer& RobotManager::add_interfacer(Args&&... params) {
		static_assert(std::is_base_of<AbstractInterfacer, Interfacer>::value,
		              "The specified module must inherit AbstractInterfacer as public.");
		if(has_interfacer<Interfacer>()) {
			throw std::runtime_error("Double assignment of the interfacer: "s + typeid(Interfacer).name());
		}

		auto interfacer = std::make_unique<Interfacer>(_robot, params...);
		Interfacer& result = *interfacer;
		_interfacers[typeid(Interfacer)] = std::move(interfacer);
		return result;
	}

	template <typename Interfacer>
	bool RobotManager::has_interfacer() const {
		return _interfacers.find(typeid(Interfacer)) != _interfacers.cend();
	}

	template <typename Interfacer>
	Interfacer& RobotManager::get_interfacer() {
		static_assert(std::is_base_of<AbstractInterfacer, Interfacer>::value,
		              "The specified module must inherit AbstractInterfacer as public.");
		auto it = _interfacers.find(typeid(Interfacer));
		if(it != _interfacers.cend()) {
			return dynamic_cast<Interfacer&>(*(it->second));
		}

		throw std::runtime_error("The interfacer doesn't exist.");
	}

} // namespace Strategy::Interfacer

#endif // ROOT_INTERFACERMANAGER_H
