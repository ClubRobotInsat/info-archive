//
// Created by paul on 24/04/18.
//

#ifndef ROOT_INTERFACE_H
#define ROOT_INTERFACE_H

#include "../../robot/Commun/Serie.h"
#include <cstdint>

namespace Commun {

	uint8_t Serie::lireOctet() {
		return 0;
	};

	void Serie::ecrireOctet(uint8_t octet){};
}


#endif // ROOT_INTERFACE_H
