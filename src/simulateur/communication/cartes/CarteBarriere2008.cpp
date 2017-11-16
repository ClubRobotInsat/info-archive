// CarteBarriere2008.cpp

#include "CarteBarriere2008.h"

CarteBarriere2008::CarteBarriere2008(unsigned char id, Robot& robot, IBarrieres& iBarrieres)
        : Carte(id, robot), _iBarrieres(iBarrieres) {
	for(int i = 0; i < NOMBRE_BARRIERE; i++)
		_barrieres[i] = false;
}

CarteBarriere2008::~CarteBarriere2008() {}

// Traite les données fournies par l'IA
void CarteBarriere2008::traiterTrame(Trame const& t) {
	switch(t.getCmd()) {
		case 0x00_b:
			if(t.getNbDonnees() == 1 && t.getDonnee(0) == 0x55_b) {
				// ping => repondre un pong
				Trame aEnvoyer(this->getId(), 0);
				aEnvoyer.addByte(0xAA_b);
				this->envoyer(aEnvoyer);
				std::cout << "CarteBarriere2008::pong ping -> pong" << std::endl;
			} else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		case 0x01_b:
			if(t.getNbDonnees() == 1)
				envoyerEtatBarrieres();
			else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		default:
			throw Trame::ErreurTrameNonTraitee(t);
	}
}

void CarteBarriere2008::envoyerEtatBarrieres() {
	static_assert(NOMBRE_BARRIERE <= 8, "L'état des barrières ne rentre pas sur un uint8_t !");

	uint8_t etat = 0;

	Trame aEnvoyer(this->getId(), 1);
	for(int i = 0; i < NOMBRE_BARRIERE; i++)
		etat += uint8_t(_barrieres[i]) << i;

	aEnvoyer.addDonnees(etat);

	this->envoyer(aEnvoyer);
}

// verifie si les barrieres on ete modifier et envoie une trame si besoin
void CarteBarriere2008::update(Duration elapsed) {
	Carte::update(elapsed);
	bool modification = false;
	bool etat;

	for(int barriere = 0; barriere < NOMBRE_BARRIERE; barriere++) {
		etat = _iBarrieres.getEtatBarrieres(this->getId(), barriere);
		if(_barrieres[barriere] != etat) {
			modification = true;
			_barrieres[barriere] = etat;
		}
	}
	if(modification)
		envoyerEtatBarrieres();
}
