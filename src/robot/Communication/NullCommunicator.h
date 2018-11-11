//
//  NullCommunicator.h
//  Club Robot
//
//  Created by Rémi on 21/03/2016.
//

#ifndef NullCommunicator_h
#define NullCommunicator_h

#include "Serial.h"
#include <thread>

namespace Communication {
	class NullCommunicator : public Serial {
	public:
		NullCommunicator() = default;
		~NullCommunicator() override = default;

		void write_bytes(const uint8_t* bytes, std::size_t bytes_number) override;

		size_t read_bytes(uint8_t* bytes, std::size_t bytes_number) override;

		bool is_connected() {
			return true;
		}
	};
} // namespace Communication


#endif /* NullCommunicator_h */
