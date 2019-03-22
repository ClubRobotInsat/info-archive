// CarteDebugDessin.cpp

#include "CarteDebugDessin.h"
#include "../simulateur/GLHeaders.h"

CarteDebugDessin::CarteDebugDessin(unsigned char id, Robot& robot, IDebugDessin& iDebugDessin)
        : Carte(id, robot), _iDebugDessin(iDebugDessin), _grilleTailleX(0), _grilleTailleY(0), _activationDessin(false) {}

CarteDebugDessin::~CarteDebugDessin() {}

// Traite les donnees fournies par l'IA
void CarteDebugDessin::traiterTrame(Trame const& t) {
	switch(t.getCmd()) {
		case 0x00_b:
			if(t.getNbDonnees() == 1 && t.getDonnee(0) == 0x55_b) {
				// ping => repondre un pong
				Trame aEnvoyer(this->getId(), 0);
				aEnvoyer.addByte(0xAA_b);
				this->envoyer(aEnvoyer);
				std::cout << "CarteDebugDessin::pong ping -> pong" << std::endl;
			} else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		case 0x01_b:
			if(t.getNbDonnees() == 4) {
				logDebug3("Réception d'un redimensionnement de la grille");
				// fixer la resolution de la grille
				_grilleTailleX = (t.getDonnee(1) << 8) | t.getDonnee(0);
				_grilleTailleY = (t.getDonnee(3) << 8) | t.getDonnee(2);
				logDebug3(_grilleTailleX, "x", _grilleTailleY);
				_iDebugDessin.setTailleGrille(_grilleTailleX, _grilleTailleY);
			} else if(t.getNbDonnees() == 3) {
				// fixer la hauteur de la grille de l'A*
				_iDebugDessin.setHauteur(Distance::makeFromMm(t.getDonnee<short>(0)) + 10_mm);
			} else if(t.getNbDonnees() == 8) {
				// fixer l'extremite haut gauche et bas droite de la grille de l'A*
				_iDebugDessin.setCoordExtremite(Vector2m(Distance::makeFromMm(t.getDonnee<short>(0)),
				                                         Distance::makeFromMm(t.getDonnee<short>(2))),
				                                Vector2m(Distance::makeFromMm(t.getDonnee<short>(4)),
				                                         Distance::makeFromMm(t.getDonnee<short>(6))));
			} else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		case 0x02_b:
			// donne l'etat des t.getNbDonnees() prochaines case de la grille
			for(int i = 0; i < t.getNbDonnees(); i++) {
				Vector4ub couleur(((t.getDonnee(i) & 0xC0) >> 6) * 85,
				                  ((t.getDonnee(i) & 0x30) >> 4) * 85,
				                  ((t.getDonnee(i) & 0x0C) >> 2) * 85,
				                  (t.getDonnee(i) & 0x03) * 85);
				_iDebugDessin.setCouleurCase(couleur);
			}
			break;

			// TODO reception des formes

		default:
			throw Trame::ErreurTrameNonTraitee(t);
	}
}

// mise a jour de la carte
void CarteDebugDessin::update(Duration elapsed) {
	Carte::update(elapsed);

	// verifier si changement etat activation affichage debug dessin
	if(_activationDessin != _iDebugDessin.getEtatActivation()) {
		_activationDessin = _iDebugDessin.getEtatActivation();
		envoyerActivationAffichage(_activationDessin);
	}
}

// envoyer activation/desactivation de l'affichage des dessin
void CarteDebugDessin::envoyerActivationAffichage(bool activation) {
	Trame aEnvoyer(this->getId(), 1);
	aEnvoyer.addByte(activation);
	this->envoyer(aEnvoyer);
}
