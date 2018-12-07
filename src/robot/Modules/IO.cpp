//
// Created by terae on 16/09/18.
//

#include "IO.h"

namespace PhysicalRobot {
	IO::IO(uint8_t id) : Module(id) {}

	bool IO::read_tirette() const {
		std::lock_guard<std::mutex> lk(_mutex_variables);
		return _tirette;
	}

	std::vector<JSON> IO::generate_list_jsons() const {
		JSON j;
		j["tirette"] = 0;
		return {j};
	}

	void IO::message_processing(const JSON& j) {
		_tirette = j["tirette"];
	}
} // namespace PhysicalRobot