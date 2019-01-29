//
// Created by terae on 23/01/19.
//

#ifndef ROOT_INTERFACERMANAGER_H
#define ROOT_INTERFACERMANAGER_H

#include "../../Robot.h"
#include "Avoidance.h"
#include "Elevator.h"
#include "Interfacer.hpp"
#include "ServosManager.h"
#include <memory>

namespace Strategy {
	namespace Interfacer {

		class GlobalManager {
		public:
			GlobalManager(std::shared_ptr<PhysicalRobot::Robot> robot);

			std::shared_ptr<PhysicalRobot::Robot> get_robot() const;

			size_t size() const {
				return _interfacers.size();
			}

			/**
			 * Insert a new interfacer and builds it from a `PhysicalRobot::Robot` and specific arguments
			 * @tparam Interfacer to build
			 * @tparam T Optional additional arguments for the `Interfacer`
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
			std::vector<std::unique_ptr<AbstractInterfacer>> _interfacers;
		};

		template <typename Interfacer, typename... Args>
		Interfacer& GlobalManager::add_interfacer(Args&&... params) {
			static_assert(std::is_base_of<AbstractInterfacer, Interfacer>::value,
			              "The specified module must inherit AbstractInterfacer as public.");
			if(has_interfacer<Interfacer>()) {
				throw std::runtime_error("Double assignment of the interfacer: "s + typeid(Interfacer).name());
			}

			auto interfacer = std::make_unique<Interfacer>(_robot, params...);
			Interfacer& result = *interfacer;
			_interfacers.push_back(std::move(interfacer));
			return result;
		}

		template <typename Interfacer>
		bool GlobalManager::has_interfacer() const {
			for(const auto& interfacer : _interfacers) {
				if(typeid(Interfacer) == typeid(*interfacer)) {
					return true;
				}
			}
			return false;
		}

		template <typename Interfacer>
		Interfacer& GlobalManager::get_interfacer() {
			static_assert(std::is_base_of<AbstractInterfacer, Interfacer>::value,
			              "The specified module must inherit AbstractInterfacer as public.");
			for(const auto& interfacer : _interfacers) {
				try {
					return dynamic_cast<Interfacer&>(*interfacer);
				} catch(std::bad_cast&) {
				}
			}

			throw std::runtime_error("The interfacer doesn't exist.");
		}
	} // namespace Interfacer
} // namespace Strategy


#endif // ROOT_INTERFACERMANAGER_H
