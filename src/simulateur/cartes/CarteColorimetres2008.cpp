#include "CarteColorimetres2008.h"

const unsigned char CarteColorimetres2008::NBR_CAPTEURS = 2;

CarteColorimetres2008::CarteColorimetres2008(unsigned char id, Robot& robot, IColorimetres& iColorimetres)
        : Carte(id, robot), _iColorimetres(iColorimetres) {}

CarteColorimetres2008::~CarteColorimetres2008() {}

// Traite les données fournies par l'IA
void CarteColorimetres2008::traiterTrame(Trame const& t) {
	switch(t.getCmd()) {
		case 0x00_b:
			if(t.getNbDonnees() == 1 && t.getDonnee(0) == 0x55_b) {
				// ping => repondre un pong
				Trame aEnvoyer(this->getId(), 0);
				aEnvoyer.addByte(0xAA_b);
				this->envoyer(aEnvoyer);
				std::cout << "CarteColorimetres2008::pong ping -> pong" << std::endl;
			} else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		case 0x01_b:
		case 0x02_b:
		case 0x03_b:
			if(t.getNbDonnees() == 1)
				envoyerEtatColorimetres(t.getCmd() - 1);
			else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		default:
			throw Trame::ErreurTrameNonTraitee(t);
	}
}

// Envoi l'etat des contacteurs indexe a partir de 0
void CarteColorimetres2008::envoyerEtatColorimetres(uint8_t num) {
	// verifier si le numero du capteur n'et pas trop grand
	if(num >= NBR_CAPTEURS)
		throw ErreurNumCapteurs(num);

	// aller chercher la couleur du capteur dans le robot
	IColorimetres::Couleur* couleurs = _iColorimetres.getColorimetre(num);

	// construire la trame
	Trame t(this->getId(), num + 1);
	t.addDonnees(num);
	for(int coul = 0; coul < IColorimetres::NBR_COULEURS; coul++)
		t.addDonnees(couleurs->couleur[coul]);

	// envoyer cette trame
	this->envoyer(t);
}
