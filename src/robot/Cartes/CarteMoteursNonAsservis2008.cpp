/*******************************************************************
 * CarteMoteursNonAsservis2008.cpp *
 * Implémente la classe gérant les moteurs 2007/2008 *
 *******************************************************************/

#include "CarteMoteursNonAsservis2008.h"

void CarteMoteursNonAsservis2008::traiterMessage(Trame const& message) {
	switch(message.getCmd()) {
		case 0x03_b: // Acquittement puissance
			_ack = true;
			break;
		default:
			this->Carte::traiterMessage(message);
			break;
	}
}

// Règle la puissance d'un moteur
void CarteMoteursNonAsservis2008::reglerPuissance(uint8_t moteur, int8_t puissance) {
	if(moteur >= MOTEUR_NON_ASSERVIS_NBR_MAX_MOTEUR) {
		printf("[ATTENTION] Carte 0x%x : numero du moteur %d est supperieur au max %d\n", getId(), moteur, MOTEUR_NON_ASSERVIS_NBR_MAX_MOTEUR);
		return;
	}

	if(puissance < -MOTEUR_NON_ASSERVIS_PUISSANCE_MAX || MOTEUR_NON_ASSERVIS_PUISSANCE_MAX < puissance) {
		printf("[ATTENTION] Carte 0x%x : puissance %d est supperieur au max %d\n", getId(), puissance, MOTEUR_NON_ASSERVIS_PUISSANCE_MAX);
		return;
	}

	_ack = false;

	envoyerMessage(this->make_trame(0x01_b, moteur, puissance));
}

// Vérifie qu'un _ack ait été reçu
bool CarteMoteursNonAsservis2008::ackRecu() {
	return _ack;
}
