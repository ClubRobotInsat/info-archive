//
// Created by terae on 20/05/19.
//

#include "IAOneRobot.h"
#include "IAParsing.h"
#include <petrilab/Cpp/MemberPetriDynamicLib.h>

#ifndef ROOT_IASECONDARY_H
#define ROOT_IASECONDARY_H

namespace Strategy {

	class IASecondary final : public IAOneRobot {
	public:
		explicit IASecondary(const IAArguments& args);

	private:
		std::unique_ptr<Petri::PetriDynamicLib> generate_petrilab_lib() override;

		void funny_action() override;
	};
} // namespace Strategy

#endif // ROOT_IASECONDARY_H
