//
// Created by terae on 16/09/18.
//

#include "IO2019.h"

namespace PhysicalRobot {
	IO2019::IO2019(uint8_t id) : Module(id, io_read_frame, io_write_frame) {}

	bool IO2019::read_tirette() const {
		std::lock_guard<std::mutex> lk(_mutex_variables);
		return _tirette;
	}

	uint8_t IO2019::get_frame_size() const {
		return 1;
	}

	SharedIO2019 IO2019::generate_shared() const {
		SharedIO2019 s = {static_cast<uint8_t>(_tirette), 0};

		return s;
	}

	void IO2019::message_processing(const SharedIO2019& s) {
		if(s.parsing_failed == 0) {
			_tirette.exchange(static_cast<bool>(s.tirette));
		}
	}
} // namespace PhysicalRobot