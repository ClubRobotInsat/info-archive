//
// Created by paul on 27/04/18.
//

#include "NullCommunicator.h"

namespace Communication {
	void NullCommunicator::lireOctets(uint8_t* octets, std::size_t nombre) {
		while(nombre > 0) {
			--nombre;
			octets[nombre] = 0;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	void NullCommunicator::ecrireOctets(uint8_t const*, std::size_t) {}

	NullCommunicator::NullCommunicator() = default;

	NullCommunicator::~NullCommunicator() = default;
} // namespace Communication