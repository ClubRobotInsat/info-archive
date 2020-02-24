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

    IOState IO::limit_switches(std::string limit_switch_name) const {
	    if(limit_switch_name == "left_high"){
	        return _limit_left_high;
	    }
	    else if(limit_switch_name == "left_middle")
        {
	        return _limit_left_middle;
        }
	    else if(limit_switch_name == "left_down"){
	        return _limit_left_down;
	    }
	    else if(limit_switch_name == "right_high"){
            return _limit_right_high;
	    }
	    else if(limit_switch_name == "right_middle"){
	        return _limit_right_middle;
	    }
	    else if(limit_switch_name == "right_down"){
	        return _limit_right_down;
	    }

	    return IOState::Exception;

    }

	std::vector<JSON> IO::generate_list_jsons() const {
		JSON j;
		j["tirette"] = toString(TriggerState::Waiting);
		j["buzzer"] = toString(_buzzer);
		return {j};
	}

	void IO::message_processing(const JSON& j) {
		if(json_has_fields(j, {"tirette", "limit_left_down", "limit_left_middle", "limit_left_high", "limit_right_down", "limit_right_middle", "limit_right_high"})) {
			const std::string tirette_state = j["tirette"];
			for(auto ee : getEnumValues<TriggerState>()) {
				if(toString(ee) == tirette_state) {
					_tirette.exchange(ee);
					break;
				}
			}

			if(j["limit_left_down"] == toString(IOState::On)) {
				_limit_left_down.exchange(IOState::On);
			} else if(j["limit_left_down"] == toString(IOState::Off)) {
				_limit_left_down.exchange(IOState::Off);
			}

			if(j["limit_left_middle"] == toString(IOState::On)) {
				_limit_left_middle.exchange(IOState::On);
			} else if(j["limit_left_middle"] == toString(IOState::Off)) {
				_limit_left_middle.exchange(IOState::Off);
			}

			if(j["limit_left_high"] == toString(IOState::On)) {
				_limit_left_high.exchange(IOState::On);
			} else if(j["limit_left_high"] == toString(IOState::Off)) {
				_limit_left_high.exchange(IOState::Off);
			}

			if(j["limit_right_down"] == toString(IOState::On)) {
				_limit_right_down.exchange(IOState::On);
			} else if(j["limit_right_down"] == toString(IOState::Off)) {
				_limit_right_down.exchange(IOState::Off);
			}

			if(j["limit_right_middle"] == toString(IOState::On)) {
				_limit_right_middle.exchange(IOState::On);
			} else if(j["limit_right_middle"] == toString(IOState::Off)) {
				_limit_right_middle.exchange(IOState::Off);
			}

			if(j["limit_right_high"] == toString(IOState::On)) {
				_limit_right_high.exchange(IOState::On);
			} else if(j["limit_right_high"] == toString(IOState::Off)) {
				_limit_right_high.exchange(IOState::Off);
			}
		}
	}

} // namespace PhysicalRobot
