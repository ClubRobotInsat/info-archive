// CarteDebug2010.cpp

#include "CarteDebug2010.h"

CarteDebug2010::CarteDebug2010(unsigned char id, Robot2010& robot) : Carte(id, robot) {}

CarteDebug2010::~CarteDebug2010() {}

// Traite les donnees fournies par l'IA
void CarteDebug2010::traiterTrame(Trame const& t) {
	switch(t.getCmd()) {
		case 0x00_b:
			if(t.getNbDonnees() == 1 && t.getDonnee(0) == 0x55_b) {
				// ping => repondre un pong
				Trame aEnvoyer(this->getId(), 0);
				aEnvoyer.addByte(0xAA_b);
				this->envoyer(aEnvoyer);
				std::cout << "CarteDebug2010::pong ping -> pong" << std::endl;
			} else
				throw Trame::ErreurTrameNonTraitee(t);
			break;
		case 0x01_b:
			if(t.getNbDonnees() == 1) {
				switch(t.getDonnee(0)) {}
			}
			break;
	}
}
