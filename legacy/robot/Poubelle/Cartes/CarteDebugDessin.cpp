/*******************************************************************
 * CarteDebugDessin.cpp *
 * Spécifie la classe gérant l'affichage de l'a star *
 * et l'affichage de forme geometrique comme une ligne, un disque *
 *******************************************************************/

#include "CarteDebugDessin.h"

CarteDebugDessin::CarteDebugDessin(Commun::CAN& can, uint8_t id)
        : Carte(can, id), _grilleTaille({0, 0}), _couleurAEnvoyerCourant(0), _numCaseAEnvoyer(0), _activationDessin(false) {}

void CarteDebugDessin::traiterMessage(Trame const& message) {
	switch(message.getCmd()) {
		case 0x01_b:
			if(message.getNbDonnees() == 1) {
				_activationDessin = message.getDonnee(0);
			}
			break;
		default:
			this->Carte::traiterMessage(message);
			break;
	}
}

// modifie le nombre de case de la grille
void CarteDebugDessin::setTailleGrille(Vector2u16 taille) {
	// logDebug3("Initialisation de la taille de la grille de l'A* : ", taille.x, "x", taille.y);
	_grilleTaille = taille;
	Trame t = this->make_trame(0x01_b, taille.x, taille.y);
	// logDebug3("Taille de la trame ", (int)t.getNbDonnees());
	envoyerMessage(t);
}

// modifie la position d'affichage de la grille
void CarteDebugDessin::setCoordExtremite(Vector2m coordHautGauche, Vector2m coordBasDroit) {
	envoyerMessage(this->make_trame(0x01_b,
	                                (coordHautGauche.x.toMm<std::int16_t>()),
	                                (coordHautGauche.y.toMm<std::int16_t>()),
	                                (coordBasDroit.x.toMm<std::int16_t>()),
	                                (coordBasDroit.y.toMm<std::int16_t>())));
}

// modifie la hauteur d'affichage de la grille
void CarteDebugDessin::setHauteur(Distance hauteur) {
	envoyerMessage(this->make_trame(0x01_b, (hauteur.toMm<std::int16_t>()), 0x00_b));
}

// modifie la couleur d'une case
void CarteDebugDessin::setCouleurCase(Couleur couleur) {
	_couleurAEnvoyer[_couleurAEnvoyerCourant] = static_cast<uint8_t>(couleur);
	_couleurAEnvoyerCourant++;
	_numCaseAEnvoyer++;
	if(_couleurAEnvoyerCourant == 8 || _numCaseAEnvoyer == _grilleTaille.x * _grilleTaille.y) {
		Trame t = this->make_trame(0x02_b);
		t.addBytes(_couleurAEnvoyerCourant, _couleurAEnvoyer);
		envoyerMessage(t);
		_couleurAEnvoyerCourant = 0;
		if(_numCaseAEnvoyer == _grilleTaille.x * _grilleTaille.y) {
			_numCaseAEnvoyer = 0;
		}
	}
}

// acceder a l'etat dactivation de l'affichage des dessin
bool CarteDebugDessin::getEtatActivation() {
	return _activationDessin;
}
