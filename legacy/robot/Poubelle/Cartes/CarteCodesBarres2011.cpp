/************************************************************
 * CarteCodesBarres2011.cpp *
 * Implémente la classe gérant les _codes barres 2011 *
 ************************************************************/

#include "CarteCodesBarres2011.h"
#include <cstring>

CarteCodesBarres2011::CarteCodesBarres2011(Commun::CAN& can, uint8_t id) : Carte(can, id) {
	for(int i = 0; i < 2; i++) {
		_codesBarresActualises[i] = false;
		_codesBarres[i] = (unsigned char)(2);
	}
}

void CarteCodesBarres2011::traiterMessage(Trame const& message) {
	switch(message.getCmd()) {
		case 0x01_b: // Reception de l'etat des _codes barres
			if(message.getNbDonnees() == 2) {
				bloquerVariables();
				if(message.getDonnee(1) == 0) {
					_codesBarresActualises[0] = true;
					_codesBarres[0] = message.getDonnee(0);
				} else if(message.getDonnee(1) == 1) {
					_codesBarresActualises[1] = true;
					_codesBarres[1] = message.getDonnee(0);
				}
				debloquerVariables();
			}
			break;
		default:
			this->Carte::traiterMessage(message);
			break;
	}
}

// retourne vrai si l'etat des contacts a ete actualise
bool CarteCodesBarres2011::estCodeBarreDetecte(int numeroCodeBarre) {
	bool resultat;

	if(numeroCodeBarre >= 2)
		return false;

	bloquerVariables();
	resultat = _codesBarresActualises[numeroCodeBarre];
	debloquerVariables();

	return resultat;
}

// Retourne l'état d'un des _codes barres
unsigned char CarteCodesBarres2011::getCodeLu(int numeroCodeBarre) {
	unsigned char resultat;

	if(numeroCodeBarre >= 2)
		return (unsigned char)(2);

	bloquerVariables();
	resultat = _codesBarres[numeroCodeBarre];
	_codesBarres[numeroCodeBarre] = 2; // Le code a ete recupere par l'info : on revient en mode "rien de scanne"
	_codesBarresActualises[numeroCodeBarre] = false;
	debloquerVariables();

	return resultat;
}
