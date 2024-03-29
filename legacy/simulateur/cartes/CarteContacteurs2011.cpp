// CarteContacteurs2011.cpp

#include "CarteContacteurs2011.h"

CarteContacteurs2011::CarteContacteurs2011(unsigned char id, Robot& robot, IContacteursV2& iContacteurs)
        : Carte(id, robot), _iContacteurs(iContacteurs) {
	for(int i = 0; i < NOMBRE_CONTACTEURS; i++)
		_contacteurs[i] = false;
}

CarteContacteurs2011::~CarteContacteurs2011() {}

// Traite les données fournies par l'IA
void CarteContacteurs2011::traiterTrame(Trame const& t) {
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
			if(t.getNbDonnees() == 1)
				envoyerEtatContacteurs();
			else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		default:
			throw Trame::ErreurTrameNonTraitee(t);
	}
}

void CarteContacteurs2011::envoyerEtatContacteurs() {
	uint8_t etat = 0;

	if(NOMBRE_CONTACTEURS > 8)
		throw ErreurNbrContact();

	Trame aEnvoyer(this->getId(), 1);
	for(int i = 0; i < NOMBRE_CONTACTEURS; i++)
		etat += !(unsigned char)(_contacteurs[i]) << i;

	aEnvoyer.addDonnees(etat);

	this->envoyer(aEnvoyer);
}

// Vérifie si les contacteurs ont été modifiés et envoie une trame si besoin
void CarteContacteurs2011::update(Duration elapsed) {
	Carte::update(elapsed);
	bool modification = false;
	bool etat;

	for(unsigned char contact = 0; contact < NOMBRE_CONTACTEURS; contact++) {
		etat = _iContacteurs.getEtatContact(contact, this->getId());
		if(_contacteurs[contact] != etat) {
			modification = true;
			_contacteurs[contact] = etat;
		}
	}
	if(modification)
		envoyerEtatContacteurs();
}
