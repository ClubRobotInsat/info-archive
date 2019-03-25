//
// Created by terae on 16/09/18.
//

#include "IO.h"

namespace PhysicalRobot {
	IO::IO(uint8_t id) : Module(id, "IO"), _tirette(TriggerState::Waiting), _buzzer(BuzzerState::Rest) {}

	TriggerState IO::read_tirette() const {
		std::lock_guard<std::mutex> lk(_mutex_variables);
		return _tirette;
	}

	void IO::set_sound(BuzzerState buzzer) {
		lock_variables();
		_buzzer = buzzer;
		_state_changed.exchange(true);
		unlock_variables();
	}

	std::vector<JSON> IO::generate_list_jsons() const {
		JSON j;
		j["tirette"] = toString(TriggerState::Waiting);
		j["buzzer"] = toString(_buzzer);
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
