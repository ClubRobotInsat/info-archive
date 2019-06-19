/************************************************************
 * CartePneumatique2014.cpp *
 * Implémente la classe gérant la carte PNEUMATIQUE *
 ************************************************************/

#include "CartePneumatique2014.h"
#include <cstring>

enum IDMessage { PING = 0, CHANGE_ETAT = 1, SEND_ETAT = 2 };

CartePneumatique2014::CartePneumatique2014(unsigned char id, RobotLogic& robot, IPneumatique& iPneumatique)
        : Carte(id, robot), _iPneumatique(iPneumatique) {}
// Traite les données fournies par l'IA
void CartePneumatique2014::traiterTrame(Trame const& t) {
	switch(t.getCmd()) {
		case PING:
			if(t.getNbDonnees() == 1 && t.getDonnee(0) == 0x55_b) {
				// ping => répondre un pong
				Trame aEnvoyer(this->getId(), 0);
				aEnvoyer.addByte(0xAA_b);
				this->envoyer(aEnvoyer);

				logInfo("[CartePneumatique2014] ping -> pong");
			} else
				throw Trame::ErreurTrameNonTraitee(t);
			return;

		case CHANGE_ETAT:
			if(t.getNbDonnees() == 1) {
				// Activation
				if(t.getDonnee(0) == 1) {
					// On met à jour l'état de la pompe
					_iPneumatique.setActivation(this->getId());
					// On envoie ce nouvel état à l'IA
					Trame aEnvoyer(this->getId(), 2);
					aEnvoyer.addByte((uint8_t)_iPneumatique.isActivated(this->getId()));
					this->envoyer(aEnvoyer);
				} else { // Désactivation
					_iPneumatique.setDesactivation(this->getId());
					Trame aEnvoyer(this->getId(), 2);
					aEnvoyer.addByte((uint8_t)_iPneumatique.isActivated(this->getId()));
					this->envoyer(aEnvoyer);
				}
			} else
				throw Trame::ErreurTrameNonTraitee(t);
			return;

		case SEND_ETAT:
			Trame aEnvoyer(this->getId(), 2);
			aEnvoyer.addByte((uint8_t)_iPneumatique.isActivated(this->getId())); // TODO : C'est bon le cast ici ?
			this->envoyer(aEnvoyer);
			return;
	}

	throw Trame::ErreurTrameNonTraitee(t);
}
