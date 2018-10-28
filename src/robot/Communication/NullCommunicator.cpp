//
// Created by paul on 27/04/18.
//

#include "NullCommunicator.h"

namespace Communication {
	void NullCommunicator::read_bytes(uint8_t* bytes, std::size_t bytes_number) {
		while(bytes_number > 0) {
			--bytes_number;
			bytes[bytes_number] = 0;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	void NullCommunicator::write_bytes(uint8_t const*, std::size_t) {}
} // namespace Communication