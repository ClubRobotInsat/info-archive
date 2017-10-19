/************************************************************
 * CartePneumatique2014.cpp *
 * Implémente la classe gérant la carte PNEUMATIQUE *
 ************************************************************/

#include "CartePneumatique2014.h"
#include <cstring>

CartePneumatique2014::CartePneumatique2014(Commun::CAN& can, uint8_t id) : Carte(can, id) {
	bloquerVariables();
	this->_pompeActivee = false;
	debloquerVariables();
}

void CartePneumatique2014::traiterMessage(Trame const& message) {
	switch(message.getCmd()) {
		case 0x02_b:
			_pompeActivee = message.getDonnee(0);
			break;

		default:
			this->Carte::traiterMessage(message);
			break;
	}
}

void CartePneumatique2014::activerPompe() {
	envoyerMessage(0x01_b, 0x01_b);
}

void CartePneumatique2014::desactiverPompe() {
	envoyerMessage(0x01_b, 0x00_b);
}

void CartePneumatique2014::actualiserEtatPompe() {
	envoyerMessage(0x02_b, 0x00_b);
}

bool CartePneumatique2014::getPompeActivee() {
	return _pompeActivee;
}
