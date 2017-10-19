// CarteCodesBarres2011.cpp

#include "CarteCodesBarres2011.h"

CarteCodesBarres2011::CarteCodesBarres2011(unsigned char id, Robot& robot, ICodesBarres& iCodesBarres)
        : Carte(id, robot), _iCodesBarres(iCodesBarres) {
	for(int i = 0; i < NOMBRE_CODE_BARRE; i++)
		_codesBarres[i] = ICodesBarres::RIEN;
}

CarteCodesBarres2011::~CarteCodesBarres2011() {}

// Traite les données fournies par l'IA
void CarteCodesBarres2011::traiterTrame(Trame const& t) {

	switch(t.getCmd()) {
		case 0x00_b:
			if(t.getNbDonnees() == 1 && t.getDonnee(0) == 0x55_b) {
				// ping => répondre un pong
				Trame aEnvoyer(this->getId(), 0);
				aEnvoyer.addByte(0xAA_b);
				this->envoyer(aEnvoyer);
				std::cout << "CarteCodebarre2011::pong ping -> pong" << std::endl;
			} else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		case 0x01_b:
			if(t.getNbDonnees() == 1) {
				envoyerEtatCodeBarre(0);
				envoyerEtatCodeBarre(1);
			} else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		default:
			throw Trame::ErreurTrameNonTraitee(t);
	}
}

void CarteCodesBarres2011::envoyerEtatCodeBarre(uint8_t numCodeBarre) {
	uint8_t etat = 0;

	if(NOMBRE_CODE_BARRE > 8)
		throw ErreurNbrContact();

	Trame aEnvoyer(this->getId(), 1);

	switch(_codesBarres[numCodeBarre]) {
		case ICodesBarres::REINE:
			logDebug9("c'est une reine");
			etat = (unsigned char)(0);
			break;
		case ICodesBarres::ROI:
			logDebug9("c'est un roi");
			etat = (unsigned char)(1);
			break;
		case ICodesBarres::RIEN:
			logDebug9("c'est rien");
			etat = (unsigned char)(2);
			break;
	}

	// on met le num du code barre
	aEnvoyer.addDonnees(etat, numCodeBarre);
	// on met l'état du code barre
	// etat += 1; //!(unsigned char)(codesBarres[numCodeBarre]);
	logDebug4(int(etat));

	this->envoyer(aEnvoyer);
}

// Vérifie si les codes barres ont été modifiés et envoie une trame si besoin
void CarteCodesBarres2011::update(Duration elapsed) {
	Carte::update(elapsed);
	bool modification = false;
	ICodesBarres::MessageLu etat;

	for(unsigned char numCodeBarre = 0; numCodeBarre < NOMBRE_CODE_BARRE; numCodeBarre++) {
		etat = _iCodesBarres.getEtatCodeBarre(numCodeBarre);

		if(_codesBarres[numCodeBarre] != etat) {
			modification = true;
			_codesBarres[numCodeBarre] = etat;
			envoyerEtatCodeBarre(numCodeBarre);
		}
	}
}
