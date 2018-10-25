//
// Created by terae on 23/10/18.
//

#include "LED2019.h"

namespace PhysicalRobot {
	LED2019::LED2019(uint8_t id) : Module(id, led_read_frame, led_write_frame) {}

	void LED2019::set_state(bool on_off) {
		lock_variables();
		_on_off = on_off;
		_state_changed = true;
		unlock_variables();
	}

	uint8_t LED2019::get_frame_size() const {
		return get_size_io_frame();
	}

	SharedLED2019 LED2019::generate_shared() const {
		SharedLED2019 s = {static_cast<uint8_t>(_on_off), 0};

		return s;
	}

	void LED2019::message_processing(const SharedLED2019& s) {
		;
	}
} // namespace PhysicalRobot
