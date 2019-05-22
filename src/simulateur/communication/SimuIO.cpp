#include "SimuIO.h"

#include <log/Log.h>

using PhysicalRobot::TriggerState;

SimuIO::SimuIO(uint8_t id) : SimuModule(id, "SimuIO"), _tirette(TriggerState::Waiting) {}

void SimuIO::deactivation() {}

void SimuIO::setTriggerState(TriggerState state) {
	lock_variables();
	_tirette = state;
	unlock_variables();
}

JSON SimuIO::getModuleState() {
	return generate_list_jsons()[0];
}

std::vector<JSON> SimuIO::generate_list_jsons() const {
	JSON json;
	json["tirette"] = toString(_tirette);
	return {json};
}

void SimuIO::message_processing(const JSON& json) {
	_state_changed.exchange(true);
}
