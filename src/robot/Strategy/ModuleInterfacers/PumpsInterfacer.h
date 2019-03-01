//
// Created by terae on 01/03/19.
//

#ifndef ROOT_PUMPSINTERFACER_H
#define ROOT_PUMPSINTERFACER_H

#include "Interfacer.hpp"

namespace Strategy {
	namespace Interfacer {

		class PumpsInterfacer final : public AbstractInterfacer {
		public:
			using interfaced_type = PhysicalRobot::Pumps;

			using pump_t = interfaced_type::pump_t;
			using valve_t = interfaced_type::valve_t;

			explicit PumpsInterfacer(interfaced_type& module_pumps);
			explicit PumpsInterfacer(std::shared_ptr<PhysicalRobot::Robot>);
			explicit PumpsInterfacer(PhysicalRobot::Robot&);

			interfaced_type* operator->();

		private:
			interfaced_type& _module;
		};
	} // namespace Interfacer
} // namespace Strategy

#endif // ROOT_PUMPSINTERFACER_H
