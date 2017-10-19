//
// Created by paul on 14/04/16.
//

#include "CarteElectrovannes2016.h"

enum IDMessage { PING = 0, ACTIV = 1, DESACTIV = 2 };

CarteElectrovannes2016::CarteElectrovannes2016(unsigned char id, RobotLogic& robot, IVanne& iVanne)
        : Carte(id, robot), _iVanne(iVanne) {}

void CarteElectrovannes2016::traiterTrame(Trame const& t) {
	switch(t.getCmd()) {
		// Ping de la carte
		case PING:
			if(t.getNbDonnees() == 1 && t.getDonnee(0) == 0x55_b) {
				Trame aEnvoyer(this->getId(), PING);
				aEnvoyer.addByte(0xAA_b);
				this->envoyer(aEnvoyer);
				logInfo("[CarteElectrovannes2016] ping -> pong");
			} else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		// activation des électrovannes
		case ACTIV:
			// DATA : numéro de l'électrovanne à activer
			if(t.getNbDonnees() == 1) {
				uint8_t num = t.getDonnee(0);
				// activation d'une unique électrovanne
				if(num >= 1 && num <= 6) {
					// mise à jour de l'état de la vanne
					_iVanne.setValveOn(num);
					// on envoie à l'IA
					Trame aEnvoyer(this->getId(), ACTIV);
					aEnvoyer.addByte(num);
					this->envoyer(aEnvoyer);

					logInfo("[CarteElectrovannes2016] activation of valve ", (short)num);
				} else if(num == 7) {
					_iVanne.activateAllValves();
					Trame aEnvoyer(this->getId(), ACTIV);
					aEnvoyer.addByte(num);
					this->envoyer(aEnvoyer);

					logInfo("[CarteElectrovannes2016] activation of all valves");
				} else {
					throw Trame::ErreurTrameNonTraitee(t); // num out of range
				}
			} else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		// désactivation des électrovannes
		case DESACTIV:
			if(t.getNbDonnees() == 1) {
				uint8_t num = t.getDonnee(0);
				if(num >= 1 && num <= 6) {
					_iVanne.setValveOff(num);
					Trame aEnvoyer(this->getId(), DESACTIV);
					aEnvoyer.addByte(num);
					this->envoyer(aEnvoyer);

					logInfo("[CarteElectrovannes2016] desactivation of valve ", (short)num);
				} else if(num == 7) {
					_iVanne.desactivateAllValves();
					Trame aEnvoyer(this->getId(), DESACTIV);
					aEnvoyer.addByte(num);
					this->envoyer(aEnvoyer);

					logInfo("[CarteElectrovannes2016] desactivation of all valves");
				} else {
					throw Trame::ErreurTrameNonTraitee(t); // num out of range
				}
			} else
				throw Trame::ErreurTrameNonTraitee(t);
			break;


		default:
			throw Trame::ErreurTrameNonTraitee(t);
	}
}
