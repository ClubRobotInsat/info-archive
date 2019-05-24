//
// Created by terae on 01/03/19.
//

#ifndef ROOT_PUMPSINTERFACER_H
#define ROOT_PUMPSINTERFACER_H

#include "../PetriLab/ServosPrimary.h"
#include "Interfacer.hpp"

namespace Strategy {
	namespace Interfacer {

		class PumpsInterfacer final : public AbstractInterfacer {
		public:
			using interfaced_type = PhysicalRobot::Pumps;

			using pump_t = interfaced_type::pump_t;
			using valve_t = interfaced_type::valve_t;

			enum AtomType {
				Nothing = 0,
				Redium = 60,
				Greenium = 120,
				Blueium = 160,
				Goldenium = 320,
			};

			// Nombre de vannes par pompe sans compter celle de dépressurisation
			static const uint8_t NBR_ATOMS_FRONT_EXTERNAL_STORAGE = 2;
			static const uint8_t NBR_ATOMS_BACK_EXTERNAL_STORAGE = 2;
			static const uint8_t NBR_ATOMS_INTERNAL_STORAGE = 3;
			static const uint8_t NBR_ATOMS_FRONT_HAND = 2;
			static const uint8_t NBR_ATOMS_BACK_HAND = 2;

			explicit PumpsInterfacer(interfaced_type& module_pumps);
			explicit PumpsInterfacer(std::shared_ptr<PhysicalRobot::Robot>);
			explicit PumpsInterfacer(PhysicalRobot::Robot&);

			ActionResult catch_front(AtomType left, AtomType right);
			ActionResult catch_back(AtomType left, AtomType right);

			ActionResult release_front();
			ActionResult release_back();

		private:
			interfaced_type& _module;

			struct Pump {
				Pump(pump_t pump, valve_t evacuation, valve_t left, valve_t right)
				        : pump(pump), evacuation(evacuation), valve_left(left), valve_right(right) {}
				pump_t pump;
				valve_t evacuation;
				valve_t valve_left;
				valve_t valve_right;
			} _front_pump, _back_pump;

			std::array<AtomType, NBR_ATOMS_FRONT_EXTERNAL_STORAGE> _front_external_storage;
			std::array<AtomType, NBR_ATOMS_BACK_EXTERNAL_STORAGE> _back_external_storage;
			std::array<AtomType, NBR_ATOMS_INTERNAL_STORAGE> _internal_storage;
			std::array<AtomType, NBR_ATOMS_FRONT_HAND> _front_hand;
			std::array<AtomType, NBR_ATOMS_BACK_HAND> _back_hand;
		};
	} // namespace Interfacer
} // namespace Strategy

#endif // ROOT_PUMPSINTERFACER_H
