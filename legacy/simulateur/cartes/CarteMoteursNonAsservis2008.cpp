#include "CarteMoteursNonAsservis2008.h"

const unsigned char CarteMoteursNonAsservis2008::MOTEUR_NON_ASSERVIS_NBR_MAX_MOTEUR = 3;
const unsigned char CarteMoteursNonAsservis2008::MOTEUR_NON_ASSERVIS_PUISSANCE_MAX = 100;


CarteMoteursNonAsservis2008::CarteMoteursNonAsservis2008(unsigned char id, Robot& robot, IMoteursNonAsservis& iMoteursNonAsservis)
        : Carte(id, robot), _iMoteursNonAsservis(iMoteursNonAsservis) {}

CarteMoteursNonAsservis2008::~CarteMoteursNonAsservis2008() {}

// Traite les données fournies par l'IA
void CarteMoteursNonAsservis2008::traiterTrame(Trame const& t) {
	switch(t.getCmd()) {
		case 0x00_b:
			if(t.getNbDonnees() == 1 && t.getDonnee(0) == 0x55_b) {
				// ping => repondre un pong
				envoyerPong();
			} else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		case 0x03_b:
			if(t.getNbDonnees() == 2) {
				// modifier la puissance d'un moteur
				unsigned char numMoteur = t.getDonnee(0);
				char puissance = t.getDonnee(1);
				if(numMoteur >= MOTEUR_NON_ASSERVIS_NBR_MAX_MOTEUR)
					throw ErreurNumMoteur(numMoteur);
				if(puissance < -MOTEUR_NON_ASSERVIS_PUISSANCE_MAX || MOTEUR_NON_ASSERVIS_PUISSANCE_MAX < puissance)
					throw ErreurPuissanceMoteur(puissance);
				_iMoteursNonAsservis.setPuissance(numMoteur, puissance);
				envoyerAcquittementPuissance(numMoteur);
			} else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		default:
			throw Trame::ErreurTrameNonTraitee(t);
	}
}

// envoyer la reponce a un ping
void CarteMoteursNonAsservis2008::envoyerPong() {
	Trame aEnvoyer(this->getId(), 0);
	aEnvoyer.addByte(0xAA_b);
	this->envoyer(aEnvoyer);
	std::cout << "CarteMoteursNonAsservis2008::envoyerPong ping -> pong" << std::endl;
}

// Envoi l'acquittement de la puissance moteur atteinte
void CarteMoteursNonAsservis2008::envoyerAcquittementPuissance(uint8_t numMoteur) {
	Trame aEnvoyer(this->getId(), 3);
	aEnvoyer.addDonnees(numMoteur);
	this->envoyer(aEnvoyer);
}
