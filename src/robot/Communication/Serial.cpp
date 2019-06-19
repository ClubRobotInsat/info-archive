#include "Serial.h"

namespace Communication {

	void Serial::write_byte(uint8_t byte) {
		this->write_bytes(&byte, 1);
	}

	uint8_t Serial::read_byte() {
		uint8_t ret;
		this->read_bytes(&ret, 1);
		return ret;
	}

} // namespace Communication
