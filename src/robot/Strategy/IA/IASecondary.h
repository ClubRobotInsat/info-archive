//
// Created by terae on 20/05/19.
//

#include "../AbstractStrategy.h"
#include "IAParsing.h"
#include <petrilab/Cpp/MemberPetriDynamicLib.h>

#ifndef ROOT_IASECONDARY_H
#define ROOT_IASECONDARY_H

namespace Strategy {

	class IASecondary final : public AbstractStrategy {
	public:
		explicit IASecondary(const IAArguments& args);

		std::shared_ptr<Interfacer::RobotManager> get_robot() const;

	private:
		void execute() override;

		void funny_action() override;

		// Lance le réseau Petri qui contient l'IA en release ou debug
		ActionResult launch_petrilab(bool debug);
		std::unique_ptr<Petri::PetriDynamicLib> _petrilab;
	};
} // namespace Strategy

#endif // ROOT_IASECONDARY_H
