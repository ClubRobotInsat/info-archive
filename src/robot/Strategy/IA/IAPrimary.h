//
// Created by terae on 13/02/19.
//

#ifndef ROOT_IAPRIMARY_H
#define ROOT_IAPRIMARY_H

#include "IAOneRobot.h"
#include "IAParsing.h"
#include <petrilab/Cpp/MemberPetriDynamicLib.h>

namespace Strategy {

	class IAPrimary final : public IAOneRobot {
	public:
		explicit IAPrimary(const IAArguments& arguments);

	private:
		std::unique_ptr<Petri::PetriDynamicLib> generate_petrilab_lib() override;

		void funny_action() override;
	};

} // namespace Strategy

#endif // ROOT_IAPRIMARY_H
