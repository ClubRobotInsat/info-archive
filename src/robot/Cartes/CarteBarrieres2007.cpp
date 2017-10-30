/*****************************************************************
 * CarteBarrieres2007.cpp *
 * Implémente la classe gérant les barrières infrarouge 2006/2007 *
 *****************************************************************/

#include "CarteBarrieres2007.h"
#include <cstring>

CarteBarrieres2007::CarteBarrieres2007(Commun::CAN& can, uint8_t id) : Carte(can, id) {
	std::memset(_barrieres, 0, sizeof(_barrieres));
	_barrieresActualisees = false;
}

void CarteBarrieres2007::traiterMessage(Trame const& message) {
	switch(message.getCmd()) {
		case 0x01_b: // Réponse état capteurs
			if(message.getNbDonnees() == 1 && message.getDonnee(0) != 0xff_b) {
				_barrieresActualisees = true;
			}
			[[fallthrough]];
		// pas de break
		case 0x02_b: // Changement état capteurs
			if(message.getNbDonnees() == 1 && message.getDonnee(0) != 0xff_b)
				for(int i = 0; i < 2; i++) {
					_barrieres[i] = (bool)(message.getDonnee(0) & (0x01 << i));
				}
			break;
		default:
			this->Carte::traiterMessage(message);
			break;
	}
}


// Demande une actualisation des états des barrières
void CarteBarrieres2007::actualiserBarrieres() {
	_barrieresActualisees = false;
	envoyerMessage(0x01_b, 0x01_b);
}

// retourne vrai si l'etat des barrières a ete actualise
bool CarteBarrieres2007::sontBarrieresActualisees() {
	return _barrieresActualisees;
}

// Retourne l'état d'une des barrières
bool CarteBarrieres2007::getBarriere(int barriere) {
	if(barriere >= 2)
		return false;

	return _barrieres[barriere];
}
