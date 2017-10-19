/*
 * CarteColorimetre2014.cpp
 *
 *  Created on: 5 mai 2014
 *      Author: Romain Taprest
 */
#include "CarteColorimetre2014.h"
#include <cstring>

CarteColorimetre2014::CarteColorimetre2014(Commun::CAN& can, uint8_t id) : Carte(can, id) {
	_couleur = COLORIMETRE_INCONNUE;
}

void CarteColorimetre2014::traiterMessage(Trame const& message) {
	switch(message.getCmd()) {
		case 0x01_b:
			_couleurActualisee = true;
			switch(message.getDonnee(0)) {
				case 'r':
					_couleur = COLORIMETRE_ROUGE;
					break;
				case 'v':
					_couleur = COLORIMETRE_VERT;
					break;
				case 'b':
					_couleur = COLORIMETRE_BLEU;
					break;
				case 'c':
					_couleur = COLORIMETRE_CYAN;
					break;
				case 'm':
					_couleur = COLORIMETRE_MAGENTA;
					break;
				case 'j':
					_couleur = COLORIMETRE_JAUNE;
					break;
				default:
					_couleur = COLORIMETRE_INCONNUE;
			}
			break;
		default:
			this->Carte::traiterMessage(message);
			break;
	}
}

// Fonction qui renvoie la couleur détectée par le colorimètre
CarteColorimetre2014::CouleurColorimetre CarteColorimetre2014::lireCouleurColorimetre() {
	this->actualiserCouleurColorimetre();
	while(!this->couleurEstActualisee()) {
		sleep(10_ms);
	}

	return _couleur;
}

// Actualiser la valeur interne de la couleur detectée
void CarteColorimetre2014::actualiserCouleurColorimetre() {
	// Envoie de la requête de donnée
	this->envoyerMessage(0x01_b, 0x00_b);
	_couleurActualisee = false;
}

// Vérifie que la variable interne de la couleur est bien actualisée
bool CarteColorimetre2014::couleurEstActualisee() {
	return _couleurActualisee;
}
