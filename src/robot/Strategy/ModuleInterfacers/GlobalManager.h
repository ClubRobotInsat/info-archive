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
			GlobalManager(std::unique_ptr<PhysicalRobot::Robot> robot);

			PhysicalRobot::Robot& get_robot() const;

			template <typename Interfacer, typename... T>
			Interfacer& add_interfacer(T... params);

			template <typename Interfacer>
			bool has_interfacer() const;

			template <typename Interfacer>
			Interfacer& get_interfacer();

		private:
			std::unique_ptr<PhysicalRobot::Robot> _robot;
			std::vector<std::shared_ptr<AbstractInterfacer>> _interfacers;
		};

		template <typename Interfacer, typename... T>
		Interfacer& GlobalManager::add_interfacer(T... params) {
			static_assert(std::is_base_of<AbstractInterfacer, Interfacer>::value,
			              "The specified module must inherit AbstractInterfacer as public.");
			if(has_interfacer<Interfacer>()) {
				throw std::runtime_error("Double assignment of the interfacer: "s + typeid(Interfacer).name());
			}

			std::shared_ptr<AbstractInterfacer> result = std::make_unique<Interfacer>(params...);
			_interfacers.push_back(result);
			return dynamic_cast<Interfacer&>(*result);
		}

		template <typename Interfacer>
		bool GlobalManager::has_interfacer() const {
			for(auto interfacer : _interfacers) {
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
			for(auto interfacer : _interfacers) {
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
