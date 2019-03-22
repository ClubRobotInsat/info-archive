/************************************************************
 * CarteLed2008.cpp *
 * Implémente la classe gérant les LEDs Tunning 2007/2008 *
 ************************************************************/

#include "CarteLed2008.h"
#include <cstring>

CarteLed2008::CarteLed2008(Commun::CAN& can, uint8_t id) : Carte(can, id) {}

void CarteLed2008::traiterMessage(Trame const& message) {
	switch(message.getCmd()) {
		default:
			this->Carte::traiterMessage(message);
			break;
	}
}

void CarteLed2008::reglerSequence(uint8_t numero_sequence) {
	envoyerMessage(0x01_b, numero_sequence);
}

void CarteLed2008::afficherSequence(uint16_t sequence) {
	envoyerMessage(this->make_trame(0x02_b, uint8_t(1), sequence));
}

void CarteLed2008::rendreLaMain(void) {
	envoyerMessage(0x01_b, 0x00_b);
}
