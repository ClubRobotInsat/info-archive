// CartePneumatique2013.cpp

#include "CartePneumatique2013.h"

CartePneumatique2013::CartePneumatique2013(unsigned char id, Robot& robot, IPneumatique& iPneumatique)
        : Carte(id, robot) {}

CartePneumatique2013::~CartePneumatique2013() {}

// Traite les données fournies par l'IA
void CartePneumatique2013::traiterTrame(Trame const& t) {
	switch(t.getCmd()) {
		case 0x00_b:
			if(t.getNbDonnees() == 1 && t.getDonnee(0) == 0x55_b) {
				// ping => répondre un pong
				Trame aEnvoyer(this->getId(), 0);
				aEnvoyer.addByte(0xAA_b);
				this->envoyer(aEnvoyer);
				logInfo("ping -> pong");
			} else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		case 0x01_b:
			if(t.getNbDonnees() == 1) {
				// demarrage timer => répondre par ok
				Trame aEnvoyer(this->getId(), 1);
				aEnvoyer.addByte(0x11_b);
				this->envoyer(aEnvoyer);
			} else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		default:
			throw Trame::ErreurTrameNonTraitee(t);
	}
}

// Envoie une trame si besoin
void CartePneumatique2013::update(Duration elapsed) {
	Carte::update(elapsed);
}
