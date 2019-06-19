/************************************************************
 * CarteIHM.cpp *
 * Implémente la classe gérant la carte IHM *
 ************************************************************/

#include "CarteIHM.h"

#include <cstring>

CarteIHM::CarteIHM(Commun::CAN& can, uint8_t id) : Carte(can, id) {

	std::memset(_contacteurs, 1, sizeof(_contacteurs));
	_contacteursActualises = false;
}

void CarteIHM::traiterMessage(Trame const& message) {
	switch(message.getCmd()) {
		case 0x01_b:
			break;

		case 0x02_b:
			// Reception de l'etat des _contacteurs
			// _contacteurs[0] est le choix de la couleur
			// _contacteurs[1] est la tirette
			// _contacteurs[2] est la présence d'un petit robot adversaire
			// _contacteurs[3] est l'utilisation de catadioptres sur nos robots
			// _contacteurs[4] est l'utilisation de catadioptres à l'extérieur
			if(message.getNbDonnees() == 1) {
				bloquerVariables();
				_contacteursActualises = true;

				for(int i = 0; i < 2; i++) {
					_contacteurs[i] = message.getDonnee(0) & (0x1 << i);
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
void CarteIHM::actualiserContacteurs() {
	_contacteursActualises = false;

	envoyerMessage(0x02_b, 0xFF_b);
}

// retourne vrai si l'etat des contacts a ete actualise
bool CarteIHM::sontContacteursActualises() {
	return _contacteursActualises;
}

// Retourne l'état d'un des _contacteurs
bool CarteIHM::getContacteur(int contacteur) {
	if(contacteur >= 2)
		return false;

	return !_contacteurs[contacteur];
}
