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

			enum StockingRail {
				Nothing = 0,
				Redium = 60,
				Greenium = 120,
				Blueium = 160,
				Goldenium = 320,
			};

			// Nombre de vannes par pompe sans compter celle de dépressurisation
			static const uint8_t NBR_ATOMS_EXTERNAL_RAIL_FORWARD = 2;
			static const uint8_t NBR_ATOMS_EXTERNAL_RAIL_BACKWARD = 2;
			static const uint8_t NBR_ATOMS_INTERNAL_RAIL = 3;
			static const uint8_t NBR_ATOMS_HAND_FORWARD = 2;
			static const uint8_t NBR_ATOMS_HAND_BACKWARD = 2;

			explicit PumpsInterfacer(interfaced_type& module_pumps);
			explicit PumpsInterfacer(std::shared_ptr<PhysicalRobot::Robot>);
			explicit PumpsInterfacer(PhysicalRobot::Robot&);

			ActionResult catch_forward(StockingRail left, StockingRail right);
			ActionResult catch_backward(StockingRail left, StockingRail right);

			ActionResult release_forward();
			ActionResult release_backward();

		private:
			interfaced_type& _module;

			struct Pump {
				Pump(pump_t pump, valve_t evacuation, valve_t left, valve_t right)
				        : pump(pump), evacuation(evacuation), valve_left(left), valve_right(right) {}
				pump_t pump;
				valve_t evacuation;
				valve_t valve_left;
				valve_t valve_right;
			} _pump_forward, _pump_backward;

			StockingRail _external_rail_forward[NBR_ATOMS_EXTERNAL_RAIL_FORWARD],
			    _external_rail_backward[NBR_ATOMS_EXTERNAL_RAIL_BACKWARD], _internal_rail[NBR_ATOMS_INTERNAL_RAIL],
			    _hand_forward[NBR_ATOMS_HAND_FORWARD], _hand_backward[NBR_ATOMS_HAND_BACKWARD];
		};
	} // namespace Interfacer
} // namespace Strategy

#endif // ROOT_PUMPSINTERFACER_H
