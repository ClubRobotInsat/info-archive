#include "SimuPumps.h"

#include <log/Log.h>

SimuPumps::SimuPumps(uint8_t id) : SimuModule(id, "SimuPumps") {}

void SimuPumps::deactivation() {}

JSON SimuPumps::getModuleState() {
	return generate_list_jsons()[0];
}

std::vector<JSON> SimuPumps::generate_list_jsons() const {
	JSON json;
	// json["pump_intensity"] = 65335;
	return {json};
}

void SimuPumps::message_processing(const JSON& json) {
	_state_changed.exchange(true);
}
