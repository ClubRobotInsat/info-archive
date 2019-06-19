//
// Created by terae on 01/03/19.
//

#ifndef ROOT_PUMPSINTERFACERPRIMARY_H
#define ROOT_PUMPSINTERFACERPRIMARY_H

#include "AtomType.h"
#include "Interfacer.hpp"

namespace Strategy::Interfacer {

	class PumpsInterfacerPrimary final : public AbstractInterfacer {
	public:
		using interfaced_type = PhysicalRobot::Pumps;

		using pump_t = interfaced_type::pump_t;
		using valve_t = interfaced_type::valve_t;

		// Nombre de vannes par pompe sans compter celle de dépressurisation
		static const uint8_t NBR_ATOMS_FRONT_EXTERNAL_STORAGE = 2;
		static const uint8_t NBR_ATOMS_BACK_EXTERNAL_STORAGE = 2;
		static const uint8_t NBR_ATOMS_INTERNAL_STORAGE = 3;
		static const uint8_t NBR_ATOMS_FRONT_HAND = 2;
		static const uint8_t NBR_ATOMS_BACK_HAND = 2;

		explicit PumpsInterfacerPrimary(interfaced_type& module_pumps);
		explicit PumpsInterfacerPrimary(const std::shared_ptr<PhysicalRobot::Robot>&);
		explicit PumpsInterfacerPrimary(PhysicalRobot::Robot&);

		Outcome catch_front(AtomType left, AtomType right);
		Outcome catch_back(AtomType left, AtomType right);

		Outcome release_all();

	private:
		interfaced_type& _module;

		pump_t _pump;
		valve_t _front_left_valve, _front_right_valve, _back_left_valve, _back_right_valve;

		std::array<AtomType, NBR_ATOMS_FRONT_EXTERNAL_STORAGE> _front_external_storage;
		std::array<AtomType, NBR_ATOMS_BACK_EXTERNAL_STORAGE> _back_external_storage;
		std::array<AtomType, NBR_ATOMS_INTERNAL_STORAGE> _internal_storage;
		std::array<AtomType, NBR_ATOMS_FRONT_HAND> _front_hand;
		std::array<AtomType, NBR_ATOMS_BACK_HAND> _back_hand;
	};

} // namespace Strategy::Interfacer

#endif // ROOT_PUMPSINTERFACERPRIMARY_H
