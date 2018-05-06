#include "CarteMoteurs2018.h"

const unsigned char CarteMoteurs2018::NBR_MAX_MOTEUR = 8;

CarteMoteurs2018::CarteMoteurs2018(unsigned char id, RobotLogic& robot, IMoteurs& iMoteurs)
        : Carte(id, robot), _iMoteurs(iMoteurs) {}

// Traite les données fournies par l'IA
void CarteMoteurs2018::traiterTrame(Trame const& t) {
	switch(t.getCmd()) {
		case 0x00_b:
			if(t.getNbDonnees() == 1 && t.getDonnee(0) == 0x55_b) {
				// ping => repondre un pong
				envoyerPong();
			} else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		case 0x06_b:
			if(t.getNbDonnees() == 2) {
				// position terminée pour les moteurs asservis
			} else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		default:
			throw Trame::ErreurTrameNonTraitee(t);
	}
}

// envoyer la reponce a un ping
void CarteMoteurs2018::envoyerPong() {
	Trame aEnvoyer(this->getId(), 0);
	aEnvoyer.addByte(0xAA_b);
	this->envoyer(aEnvoyer);
	std::cout << "CarteMoteurs2018::envoyerPong ping -> pong" << std::endl;
}
