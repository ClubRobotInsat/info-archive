//
// Created by paul on 27/04/18.
//

#include "NullCommunicator.h"

namespace Communication {
	size_t NullCommunicator::read_bytes(uint8_t* bytes, std::size_t bytes_number) {
		while(bytes_number > 0) {
			--bytes_number;
			bytes[bytes_number] = 0;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		return bytes_number;
	}

	void NullCommunicator::write_bytes(const uint8_t*, std::size_t) {}
} // namespace Communication
