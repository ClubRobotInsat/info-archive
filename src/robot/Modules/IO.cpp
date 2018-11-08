//
// Created by terae on 16/09/18.
//

#include "IO.h"

namespace PhysicalRobot {
	IO::IO(uint8_t id) : Module(id, io_read_frame, io_write_frame) {}

	bool IO::read_tirette() const {
		std::lock_guard<std::mutex> lk(_mutex_variables);
		return _tirette;
	}

	uint8_t IO::get_frame_size() const {
		return get_size_io_frame();
	}

	SharedIO2019 IO::generate_shared() const {
		SharedIO2019 s = {static_cast<uint8_t>(_tirette), 0};

		return s;
	}

	void IO::message_processing(const SharedIO2019& s) {
		if(s.parsing_failed == 0) {
			_tirette = static_cast<bool>(s.tirette);
		}
	}
} // namespace PhysicalRobot