//
//  CarteCAN_USB.cpp
//  Club Robot
//
//  Created by Rémi on 17/10/2014.
//

#include "CarteCAN_USB.h"

#include <cstdint>
#include <cstring>


CarteCAN_USB::CarteCAN_USB(unsigned char id, RobotLogic& robot) : Carte(id, robot) {}

// Traite les données fournies par l'IA
void CarteCAN_USB::traiterTrame(Trame const& t) {
	switch(t.getCmd()) {
		case 0x00_b:
			if(t.getNbDonnees() == 1 && t.getDonnee(0) == 0x55_b) {
				// ping => répondre un pong
				Trame aEnvoyer(this->getId(), 0);
				aEnvoyer.addByte(0xAA_b);
				this->envoyer(aEnvoyer);
			} else
				throw Trame::ErreurTrameNonTraitee(t);
			break;
		default:
			throw Trame::ErreurTrameNonTraitee(t);
	}
}
