// CarteDebug2011.cpp

#include "CarteDebug2011.h"

CarteDebug2011::CarteDebug2011(unsigned char id, RobotTitan2011* robot) : Carte(id, robot), robotTitan2011(robot) {}

CarteDebug2011::~CarteDebug2011() {}

// Traite les donnees fournies par l'IA
void CarteDebug2011::traiterTrame(Trame const& t) {
	switch(t.getCmd()) {
		case 0x00_b:
			if(t.getNbDonnees() == 1 && t.getDonnee(0) == 0x55_b) {
				// ping => repondre un pong
				Trame aEnvoyer(this->getId(), 0);
				aEnvoyer.addByte(0xAA_b);
				this->envoyer(aEnvoyer);
				cout << "CarteDebug2011::pong ping -> pong" << endl;
			} else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		/*case 0x01_b:
		 if(t.getNbDonnees() == 1)
		 switch(t.getDonnee(0)) {

		 }
		 break;*/
		default:
			break;
	}
}
