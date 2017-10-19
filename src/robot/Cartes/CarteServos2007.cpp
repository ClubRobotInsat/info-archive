/************************************************************
 * CarteServos2007.cpp *
 * Implémente la classe gérant les servos 2006/2007 *
 ************************************************************/

#include "CarteServos2007.h"

CarteServos2007::CarteServos2007(Commun::CAN& can, uint8_t id) : Carte(can, id), _ack(false) {}

void CarteServos2007::traiterMessage(Trame const& message) {
	switch(message.getCmd()) {
		case 0x01_b: // Acquittement position
			_ack = true;
			break;
		default:
			this->Carte::traiterMessage(message);
			break;
	}
}

// Règle la position d'un servo
void CarteServos2007::reglerPosition(uint8_t servo, uint8_t angle) {
	_ack = false;

	envoyerMessage(this->make_trame(0x01_b, servo, angle));
}

// Vérifie qu'un _ack ait été reçu
bool CarteServos2007::ackRecu() {
	return _ack;
}
