//
// Created by terae on 28/05/19.
//

#ifndef ROOT_PUMPSINTERFACERSECONDARY_H
#define ROOT_PUMPSINTERFACERSECONDARY_H

#include "AtomType.h"
#include "Interfacer.hpp"

namespace Strategy::Interfacer {

	class PumpsInterfacerSecondary final : public AbstractInterfacer {
	public:
		using interfaced_type = PhysicalRobot::Pumps;

		using pump_t = interfaced_type::pump_t;
		using valve_t = interfaced_type::valve_t;

		explicit PumpsInterfacerSecondary(interfaced_type& module_pumps);

		explicit PumpsInterfacerSecondary(const std::shared_ptr<PhysicalRobot::Robot>&);

		explicit PumpsInterfacerSecondary(PhysicalRobot::Robot&);

		ActionResult catch_atom(AtomType);

		ActionResult release_atom();

	private:
		interfaced_type& _module;

		pump_t _pump;
		valve_t _evacuation;

		AtomType _hand;
		std::vector<AtomType> _left_container;
		std::vector<AtomType> _right_container;
	};

} // namespace Strategy::Interfacer

#endif // ROOT_PUMPSINTERFACERSECONDARY_H
