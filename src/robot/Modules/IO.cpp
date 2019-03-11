//
// Created by terae on 16/09/18.
//

#include "IO.h"

namespace PhysicalRobot {
	IO::IO(uint8_t id) : Module(id, "IO"), _tirette(TriggerState::Waiting) {}

	TriggerState IO::read_tirette() const {
		std::lock_guard<std::mutex> lk(_mutex_variables);
		return _tirette;
	}

	std::vector<JSON> IO::generate_list_jsons() const {
		JSON j;
		j["tirette"] = toString(TriggerState::Waiting);
		return {j};
	}

	void IO::message_processing(const JSON& j) {
		for(auto ee : getEnumValues<TriggerState>()) {
			if(toString(ee) == j["tirette"]) {
				_tirette.exchange(ee);
				break;
			}
		}
	}
} // namespace PhysicalRobot
