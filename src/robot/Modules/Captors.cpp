//
// Created by terae on 29/05/19.
//

#include "Captors.h"

namespace PhysicalRobot {

	Captors::Captors(uint8_t id) : Module(id, "Captors"), _color(CaptorColor::Green) {
		for(switch_t id = 0; id < NBR_MAX_SWITCHES; ++id) {
			_switches_on_off[id].exchange(TriggerState::Waiting);
		}
	}

	CaptorColor Captors::read_color() const {
		std::lock_guard<std::mutex> lk(_mutex_variables);
		return _color;
	}

	TriggerState Captors::read_switch(switch_t id) const {
		if(id >= NBR_MAX_SWITCHES) {
			throw std::runtime_error("ID du switch trop grand ("s + std::to_string(id) +
			                         " >= " + std::to_string(NBR_MAX_SWITCHES) + ") !");
		}

		std::lock_guard<std::mutex> lk(_mutex_variables);
		return _switches_on_off[id].load();
	}

	std::vector<JSON> Captors::generate_list_jsons() const {
		JSON captors;
		captors["color"] = toString(_color.load());
		for(switch_t id = 0; id < NBR_MAX_SWITCHES; ++id) {
			captors["switches"][id] = toString(_switches_on_off[id].load());
		}

		return std::vector({captors});
	}

	void Captors::message_processing(const JSON& j) {
		if(json_has_fields(j, {"color", "switches"})) {
			for(auto ee : getEnumValues<CaptorColor>()) {
				if(toString(ee) == j["color"]) {
					_color.exchange(ee);
					break;
				}
			}

			for(switch_t id = 0; id < NBR_MAX_SWITCHES; ++id) {
				const std::string state = j["switches"][id];
				for(auto ee : getEnumValues<TriggerState>()) {
					if(toString(ee) == state) {
						_switches_on_off[id].exchange(ee);
						break;
					}
				}
			}
		}
	}

	void Captors::deactivation() {}

} // namespace PhysicalRobot
