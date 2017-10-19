#include "Serie.h"

namespace Commun {
	void Serie::ecrireOctet(uint8_t octet) {
		this->ecrireOctets(&octet, 1);
	}

	uint8_t Serie::lireOctet() {
		uint8_t ret;
		this->lireOctets(&ret, 1);
		return ret;
	}
}