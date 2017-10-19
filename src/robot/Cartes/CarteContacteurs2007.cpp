/************************************************************
 * CarteContacteurs2007.cpp *
 * Implémente la classe gérant les contacteurs 2006/2007 *
 ************************************************************/

#include "CarteContacteurs2007.h"
#include <cstring>

CarteContacteurs2007::CarteContacteurs2007(Commun::CAN& can, uint8_t id) : Carte(can, id) {
	std::memset(_contacteurs, 1, sizeof(_contacteurs));
	_contacteursActualises = false;
}

void CarteContacteurs2007::traiterMessage(Trame const& message) {
	switch(message.getCmd()) {
		case 0x01_b: // Reception de l'etat des _contacteurs
			if(message.getNbDonnees() == 1) {
				bloquerVariables();
				_contacteursActualises = true;

				for(int i = 0; i < 6; i++) {
					_contacteurs[i] = message.getDonnee(0) & (0x01 << i);
				}
				debloquerVariables();
			}
			break;
		default:
			this->Carte::traiterMessage(message);
			break;
	}
}

// Demande une actualisation des états des _contacteurs
void CarteContacteurs2007::actualiserContacteurs() {
	_contacteursActualises = false;

	envoyerMessage(0x01_b, 0xFF_b);
}

// retourne vrai si l'etat des contacts a ete actualise
bool CarteContacteurs2007::sontContacteursActualises() {
	return _contacteursActualises;
}

// Retourne l'état d'un des _contacteurs
bool CarteContacteurs2007::getContacteur(int contacteur) {
	if(contacteur >= 6)
		return false;

	return !_contacteurs[contacteur];
}
