#include "CarteServos2007.h"

const unsigned char CarteServos2007::SERVOS_NBR = 3;

CarteServos2007::CarteServos2007(unsigned char id, Robot& robot, IServos& iServos)
        : Carte(id, robot), _iServos(iServos) {}

CarteServos2007::~CarteServos2007() {}

// Traite les données fournies par l'IA
void CarteServos2007::traiterTrame(Trame const& t) {
	switch(t.getCmd()) {
		case 0x00_b:
			// ping => repondre un pong
			if(t.getNbDonnees() == 1 && t.getDonnee(0) == 0x55_b)
				envoyerPong();
			else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		case 0x01_b:
			if(t.getNbDonnees() == 2) {
				// verifier les donnees
				unsigned char numServos = t.getDonnee(0);
				unsigned char angle = t.getDonnee(1);
				if(numServos >= SERVOS_NBR)
					throw ErreurNumServos(numServos);
				// modifier l'angle d'un servo
				_iServos.setAngleServo(getId(), numServos, angle);
				// envoyer l'acquittement
				envoyerAcquittementAngle(numServos);
			} else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		default:
			throw Trame::ErreurTrameNonTraitee(t);
	}
}

// Envoi reponce a un ping
void CarteServos2007::envoyerPong() {
	Trame aEnvoyer(this->getId(), 0);
	aEnvoyer.addByte(0xAA_b);
	this->envoyer(aEnvoyer);
	std::cout << "CarteServos2007::envoyerPong ping -> pong" << std::endl;
}

// Envoi acquittement
void CarteServos2007::envoyerAcquittementAngle(uint8_t numServo) {
	Trame aEnvoyer(this->getId(), 1);
	aEnvoyer.addDonnees(numServo);
	this->envoyer(aEnvoyer);
}
