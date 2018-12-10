#include "SimuLed.h"

#include <log/Log.h>

SimuLed::SimuLed(uint8_t id) : Module(id), _on_off{false} {}

void SimuLed::deactivation() {
	_on_off = false;
}

std::vector<JSON> SimuLed::generate_list_jsons() const {
	JSON json;
	json["on_off"] = _on_off;
	return {json};
}

void SimuLed::message_processing(const JSON& json) {
	bool new_on_off = json["on_off"].get<bool>();

	if(new_on_off != _on_off) {
		logDebug("LED state changed to ", new_on_off);
		_on_off = new_on_off;
	}
	_state_changed.exchange(true);
}
