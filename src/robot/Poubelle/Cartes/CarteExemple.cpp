#include "CarteExemple.h"

CarteExemple::CarteExemple(Commun::CAN& can, uint8_t id) : Carte(can, id) {}

/// Traite les messages de la carte
void CarteExemple::traiterMessage(Trame const& message) {
	switch(message.getCmd()) {
		// Ici le traitement des messages specifiques a la carte
		default:
			this->Carte::traiterMessage(message);
			break;
	}
}
