//
// Created by terae on 01/03/19.
//

#ifndef ROOT_PUMPS_H
#define ROOT_PUMPS_H

#include "IO.h"
#include "Module.hpp"

namespace PhysicalRobot {

	class Pumps final : public Module {
	public:
		using pump_t = uint8_t;
		using valve_t = uint8_t;

		static const pump_t NBR_MAX_PUMP = 1;
		static const valve_t NBR_MAX_VALVE = 4;

		explicit Pumps(uint8_t id);

		void activate_pump(pump_t);
		void deactivate_pump(pump_t);
		IOState is_pump_activated(pump_t);

		void activate_valve(valve_t);
		void deactivate_valve(valve_t);
		IOState is_valve_activated(valve_t);

	private:
		std::vector<JSON> generate_list_jsons() const override;
		void message_processing(const JSON&) override;

		void deactivation() override;

		std::array<std::atomic<IOState>, NBR_MAX_PUMP> _pumps_on_off;
		std::array<std::atomic<IOState>, NBR_MAX_VALVE> _valves_on_off;

		void test_is_pump_ok(pump_t) const;
		void test_is_valve_ok(valve_t) const;
	};

} // namespace PhysicalRobot

#endif // ROOT_PUMPS_H
