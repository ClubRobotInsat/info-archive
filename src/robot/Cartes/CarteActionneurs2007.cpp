/*******************************************************************
 * CarteActionneurs2007.cpp *
 * Implémente la classe gérant les moteurs et servos 2006/2007 *
 *******************************************************************/

#include "CarteActionneurs2007.h"

void CarteActionneurs2007::traiterMessage(Trame const& message) {

	switch(message.getCmd()) {
		case 0x02_b: // Acquittement position
		case 0x03_b: // Acquittement vitesse
			_ack = true;
			break;
		default:
			this->Carte::traiterMessage(message);
			break;
	}
}

// Règle la position d'un servo
void CarteActionneurs2007::reglerPosition(uint8_t servo, uint8_t angle) {
	_ack = false;

	envoyerMessage(this->make_trame(0x02_b, servo, angle));
}

// Règle la vitesse d'un moteur
void CarteActionneurs2007::reglerVitesse(uint8_t moteur, uint8_t vitesse) {
	_ack = false;

	envoyerMessage(this->make_trame(0x03_b, moteur, vitesse));
}

// Vérifie qu'un ack ait été reçu
bool CarteActionneurs2007::ackRecu() {
	return _ack;
}
