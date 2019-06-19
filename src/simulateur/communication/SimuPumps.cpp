#include "SimuPumps.h"

#include <log/Log.h>

SimuPumps::SimuPumps(uint8_t id) : SimuModule(id, "SimuPumps") {}

void SimuPumps::add_field(const std::string& key, const std::string& value) {
	_frame[key] = value;
}

void SimuPumps::deactivation() {}

JSON SimuPumps::getModuleState() {
	std::lock_guard<std::mutex> _lock(_mutex_variables);
	return _state;
}

std::vector<JSON> SimuPumps::generate_list_jsons() const {
	return {_frame};
}

void SimuPumps::message_processing(const JSON& json) {
	_state_changed.exchange(true);
	_state = json;
}
