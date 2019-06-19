//
// Created by terae on 23/05/19.
//

#ifndef ROOT_IAONEROBOT_H
#define ROOT_IAONEROBOT_H

#include "../AbstractStrategy.h"
#include "IAParsing.h"
#include <petrilab/Cpp/MemberPetriDynamicLib.h>

namespace Strategy {

	class IAOneRobot : public AbstractStrategy {
		std::string get_robot_name() const;

	protected:
		IAOneRobot(const IAArguments& arguments, const std::string& robot_name);

		const std::string name;

		void funny_action() override;

		void init_strategy();

		virtual std::unique_ptr<Petri::PetriDynamicLib> generate_petrilab_lib() = 0;

		std::shared_ptr<Interfacer::RobotManager> get_robot() const;

	private:
		using AbstractStrategy::add_manager;
		using AbstractStrategy::add_robot;
		using AbstractStrategy::get_robot;
		using AbstractStrategy::get_robot_names;

		void execute() override;

		// Lance le réseau Petri qui contient l'IA en release ou debug
		Outcome launch_petrilab(bool debug);
		std::unique_ptr<Petri::PetriDynamicLib> _petrilab;

		std::thread _points_printer;
	};

} // namespace Strategy

#endif // ROOT_IAONEROBOT_H
