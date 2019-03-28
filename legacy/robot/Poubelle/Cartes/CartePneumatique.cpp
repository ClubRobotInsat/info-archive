/************************************************************
 * CartePneumatique.cpp *
 * Implémente la classe gérant la carte PNEUMATIQUE *
 ************************************************************/

#include "CartePneumatique.h"
#include <cstring>

CartePneumatique::CartePneumatique(Commun::CAN& can, uint8_t id) : Carte(can, id) {
	this->_timerDemarre = false;
}

void CartePneumatique::traiterMessage(Trame const& message) {
	switch(message.getCmd()) {
		case 0x01_b:
			_timerDemarre = true;
			break;

		default:
			this->Carte::traiterMessage(message);
			break;
	}
}

bool CartePneumatique::estTimerDemarre() {
	return _timerDemarre;
}

// Demande une actualisation des états des contacteurs
void CartePneumatique::demarrerTimer() {
	envoyerMessage(0x01_b, 0xFF_b);
}
