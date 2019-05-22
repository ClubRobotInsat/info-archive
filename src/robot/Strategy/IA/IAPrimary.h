//
// Created by terae on 13/02/19.
//

#ifndef ROOT_IAPRIMARY_H
#define ROOT_IAPRIMARY_H

#include "../AbstractStrategy.h"
#include "IAParsing.h"
#include <petrilab/Cpp/MemberPetriDynamicLib.h>

namespace Strategy {

	class IAPrimary final : public AbstractStrategy {
	public:
		explicit IAPrimary(const IAArguments& arguments);

		std::shared_ptr<Interfacer::RobotManager> get_robot() const;

	private:
		using AbstractStrategy::add_manager;
		using AbstractStrategy::add_robot;
		using AbstractStrategy::get_robot;
		using AbstractStrategy::get_robot_names;

		void execute() override;

		void funny_action() override;

		// Lance le réseau Petri qui contient l'IA en release ou debug
		ActionResult launch_petrilab(bool debug);
		std::unique_ptr<Petri::PetriDynamicLib> _petrilab;

		std::thread _points_printer;
	};
} // namespace Strategy

#endif // ROOT_IAPRIMARY_H
