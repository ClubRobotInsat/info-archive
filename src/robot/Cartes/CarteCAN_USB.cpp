/*
 * CarteCAN_USB.cpp
 *
 *  Created on: 8 mai 2014
 *      Author: seydoux
 */
#include "CarteCAN_USB.h"

CarteCAN_USB::CarteCAN_USB(Commun::CAN& can, uint8_t id) : Carte(can, id) {
	_init = false;
}

void CarteCAN_USB::reinitialiserNumSequence() {
	_init = false;
	envoyerMessage(0x01_b, 0xFF_b);
}

void CarteCAN_USB::traiterMessage(Trame const& message) {
	switch(message.getCmd()) {
		// Réponse initialisation
		case 0x01_b:
			_init = true;
			break;
		default:
			this->Carte::traiterMessage(message);
			break;
	}
}

bool CarteCAN_USB::isInitialiseNumSequence() {
	return _init;
}
