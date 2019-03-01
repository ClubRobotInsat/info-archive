//
// Created by terae on 01/03/19.
//

#include "Pumps.h"

namespace PhysicalRobot {

	void Pumps::activate_pump(pump_t id) {
		test_is_pump_ok(id);

		lock_variables();
		_pumps_on_off[id] = IOState::On;
		_state_changed.exchange(true);
		unlock_variables();
	}

	void Pumps::deactivate_pump(pump_t id) {
		test_is_pump_ok(id);

		lock_variables();
		_pumps_on_off[id] = IOState::Off;
		_state_changed.exchange(true);
		unlock_variables();
	}

	uint16_t Pumps::get_pump_intensity(pump_t id) const {
		test_is_pump_ok(id);

		std::lock_guard<std::mutex> lk(_mutex_variables);
		return _pump_intensity[id];
	}

	void Pumps::activate_valve(valve_t id) {
		test_is_valve_ok(id);

		lock_variables();
		_valves_on_off[id] = IOState::On;
		_state_changed.exchange(true);
		unlock_variables();
	}

	void Pumps::deactivate_valve(valve_t id) {
		test_is_valve_ok(id);

		lock_variables();
		_valves_on_off[id] = IOState::Off;
		_state_changed.exchange(true);
		unlock_variables();
	}

	std::vector<JSON> Pumps::generate_list_jsons() const {
		// FIXME : le nombre de pompes en méca n'est pas fix, du coup on prévoit des structures pour en contrôler N mais en élec ils n'en ont qu'une pour le moment
		assert(ID_MAX_PUMP == 1);

		std::vector<JSON> result;

		JSON pumps;
		pumps["pumps"] = _pumps_on_off[0].load();
		pumps["pump_intensity"] = _pump_intensity[0].load();
		for(valve_t id = 0; id < ID_MAX_VALVE - 1; ++id) {
			pumps["valves"][id] = toString(_valves_on_off[id]);
		}

		return std::vector({pumps});
	}

	void Pumps::message_processing(const JSON& j) {
		assert(ID_MAX_PUMP == 1);
		_pump_intensity[0].exchange(j["pump_intensity"]);
	}

	void Pumps::deactivation() {
		assert(ID_MAX_PUMP == 1);

		lock_variables();

		_pumps_on_off[0] = IOState::Off;
		for(valve_t id = 0; id < ID_MAX_VALVE; ++id) {
			_valves_on_off[id] = IOState::Off;
		}

		_state_changed.exchange(true);
		unlock_variables();
	}

	void Pumps::test_is_pump_ok(pump_t id) const {
		if(id >= ID_MAX_PUMP) {
			throw std::runtime_error("Invalid number for the pump: "s + std::to_string(id));
		}
	}

	void Pumps::test_is_valve_ok(PhysicalRobot::Pumps::valve_t id) const {
		if(id >= ID_MAX_VALVE) {
			throw std::runtime_error("Invalid number for the valve: "s + std::to_string(id));
		}
	}
} // namespace PhysicalRobot
