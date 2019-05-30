//
// Created by terae on 28/05/19.
//

#ifndef ROOT_PUMPSINTERFACERSECONDARY_H
#define ROOT_PUMPSINTERFACERSECONDARY_H

#include "AtomType.h"
#include "CaptorsInterfacerSecondary.h"
#include "Interfacer.hpp"

namespace Strategy::Interfacer {

	class PumpsInterfacerSecondary final : public AbstractInterfacer {
	public:
		using interfaced_type = PhysicalRobot::Pumps;

		using pump_t = interfaced_type::pump_t;
		using valve_t = interfaced_type::valve_t;

		PumpsInterfacerSecondary(interfaced_type& module_pumps, CaptorsInterfacerSecondary&);

		PumpsInterfacerSecondary(const std::shared_ptr<PhysicalRobot::Robot>&, CaptorsInterfacerSecondary&);

		PumpsInterfacerSecondary(PhysicalRobot::Robot&, CaptorsInterfacerSecondary&);

		Outcome catch_atom();

		Outcome release_atom();

		AtomType get_color_hand() const;

		AtomType get_color_top_left() const;

		AtomType get_color_top_right() const;

	private:
		interfaced_type& _module;
		CaptorsInterfacerSecondary& _captors;

		pump_t _pump;
		valve_t _evacuation;

		AtomType _hand;
		std::vector<AtomType> _left_container;
		std::vector<AtomType> _right_container;

		AtomType _color_to_atom(PhysicalRobot::CaptorColor) const;
	};

} // namespace Strategy::Interfacer

#endif // ROOT_PUMPSINTERFACERSECONDARY_H
