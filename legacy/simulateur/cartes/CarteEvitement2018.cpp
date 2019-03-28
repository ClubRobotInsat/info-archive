#include "CarteEvitement2018.h"

CarteEvitement2018::CarteEvitement2018(unsigned char id, RobotLogic& robot) : Carte(id, robot) {}

void CarteEvitement2018::traiterTrame(Trame const& t) {
	switch(t.getCmd()) {
		// On a recu un ping (cmd == 0x00), on répond pong
		case 0:
			if(t.getNbDonnees() == 1 && t.getDonnee(0) == 0x55_b)
				pong();
			break;
		default:
			break;
	}
}

void CarteEvitement2018::pong() {
	Trame aEnvoyer(this->getId(), 0x00);
	aEnvoyer.addByte(0xAA_b);
	this->envoyer(aEnvoyer);
	logInfo("ping -> pong");
}
