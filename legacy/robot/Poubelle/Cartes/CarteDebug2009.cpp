/*******************************************************************
 * CarteDebug2009.h *
 * Spécifie la classe gérant les actions de *
 * très haut niveau sur le robot 2009 (ramasser, déposer, etc...) *
 *******************************************************************/

#include "CarteDebug2009.h"

void CarteDebug2009::traiterMessage(Trame const& message) {
	switch(message.getCmd()) {
		case 0x01_b:
			_paletARamasser = message.getDonnee(0);
			break;
		default:
			this->Carte::traiterMessage(message);
			break;
	}
}

// Demande si un objet est à ramasser ou pas
void CarteDebug2009::demanderPaletARamasser() {
	envoyerMessage(0x01_b, 0xFF_b);
}

// Vérifie que la carte a répondu par un pong
bool CarteDebug2009::verifierPaletARamasser() {
	return _paletARamasser;
}

// Ramasser un palet dans la colonne no x (1 ou 2)
void CarteDebug2009::ramasserPalet(uint8_t no_colonne) {
	envoyerMessage(0x02_b, no_colonne);
}

// Poser la colonne no x (1 ou 2) a une certaine hauteur (en nombre de palets)
void CarteDebug2009::poserColonne(uint8_t no_colonne, uint8_t hauteur) {
	envoyerMessage(this->make_trame(0x03_b, no_colonne, hauteur));
}

// Poser lle linteau a une certaine hauteur (en nombre de palets)
void CarteDebug2009::poserLinteau(uint8_t hauteur) {
	envoyerMessage(0x04_b, hauteur);
}

// ramasser un linteau
void CarteDebug2009::ramasserLinteau() {
	envoyerMessage(0x05_b, 0x00_b);
}
