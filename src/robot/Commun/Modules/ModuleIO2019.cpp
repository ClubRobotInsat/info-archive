//
// Created by terae on 16/09/18.
//

#include "ModuleIO2019.h"

namespace Commun {
	ModuleIO2019::ModuleIO2019(uint8_t id) : Module(id, io_read_frame, io_write_frame) {}

	bool ModuleIO2019::read_tirette() const {
		std::lock_guard<std::mutex> lk(_mutex_variables);
		return _tirette;
	}

	uint8_t ModuleIO2019::get_frame_size() const {
		return 1;
	}

	SharedIO2019 ModuleIO2019::generate_shared() const {
		SharedIO2019 s = {(uint8_t)_tirette, 0};

		return s;
	}

	void ModuleIO2019::message_processing(const SharedIO2019& s) {
		if(s.parsing_failed == 0) {
			_tirette.exchange((bool)s.tirette);
		}
	}
} // namespace Commun