//
// Created by terae on 29/05/19.
//

#ifndef ROOT_INTERFACER_CAPTORS_SECONDARY_H
#define ROOT_INTERFACER_CAPTORS_SECONDARY_H

#include "Interfacer.hpp"

namespace Strategy::Interfacer {

	class CaptorsInterfacerSecondary final : public AbstractInterfacer {
	public:
		/*using interfaced_type = PhysicalRobot::Captors;

		using switch_t = interfaced_type::switch_t;

		explicit CaptorsInterfacerSecondary(interfaced_type& module_captors);
		explicit CaptorsInterfacerSecondary(std::shared_ptr<PhysicalRobot::Robot>);
		explicit CaptorsInterfacerSecondary(PhysicalRobot::Robot&);

		interfaced_type* operator->();
*/
	private:
		//interfaced_type& _module;
	};

} // namespace Strategy::Interfacer

#endif // ROOT_INTERFACER_CAPTORS_SECONDARY_H
