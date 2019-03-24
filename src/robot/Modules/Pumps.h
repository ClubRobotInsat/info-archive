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
		using vacst_t = uint8_t;
		using valve_t = uint8_t;

		static const pump_t ID_MAX_PUMP = 2;
		static const valve_t ID_MAX_VALVE = 6;
		static const vacst_t ID_MAX_VACST = 0;

		explicit Pumps(uint8_t id);

		void activate_pump(pump_t);
		void deactivate_pump(pump_t);
		IOState is_pump_activated(pump_t);

		uint16_t get_pump_intensity(pump_t) const;

		void activate_valve(valve_t);
		void deactivate_valve(valve_t);
		IOState is_valve_activated(valve_t);

		void activate_vacst(vacst_t);
		void deactivate_vacst(vacst_t);
		IOState is_vacst_activated(vacst_t);

	private:
		std::vector<JSON> generate_list_jsons() const override;
		void message_processing(const JSON&) override;

		void deactivation() override;

		std::array<std::atomic<IOState>, ID_MAX_PUMP> _pumps_on_off;
		std::array<std::atomic<IOState>, ID_MAX_VALVE> _valves_on_off;
		std::array<std::atomic<IOState>, ID_MAX_VACST> _vacst_on_off;
		std::array<std::atomic_uint16_t, ID_MAX_PUMP> _pump_intensity;

		void test_is_pump_ok(pump_t) const;
		void test_is_valve_ok(valve_t) const;
		void test_is_vacst_ok(vacst_t) const;
	};
} // namespace PhysicalRobot

#endif
