/*
 * CarteIO2014.cpp
 *
 *  Created on: 20 mars 2014
 *      Author: seydoux
 */
#include "CarteIO2018.h"
//#include <stdlib.h>
#include "../../commun/log/Log.h"
#include <cstring>

CarteIO2018::CarteIO2018(Commun::CAN& can, uint8_t id) : Carte(can, id) {
	std::memset(_capteurs, 1, sizeof(_capteurs));
	for(int i = 0; i < NBCAPTEURS; i++)
		_capteurs_actualises[i] = false;
}

void CarteIO2018::traiterMessage(Trame const& message) {
	switch(message.getCmd()) {
		case 0x01_b: {
			// Reception de l'etat d'un capteur
			// Le numéro du capteur est le premier octet de données
			if(message.getNbDonnees() == 2) {
				uint8_t noCapteur = message.getDonnee(0);
				// On a reçu un booléen
				_capteurs[noCapteur] = static_cast<bool>(message.getDonnee(1));
				_capteurs_actualises[noCapteur] = true;
			}
			break;
		}
		default:
			this->Carte::traiterMessage(message);
			break;
	}
}


// Envoie la demande de lecture des capteurs
void CarteIO2018::actualiserUnCapteur(uint8_t noCapteur) {
	if(noCapteur > NBCAPTEURS) {
		logError("Numéro de capteur ", (int)noCapteur, " hors limite pour la carte IO");
	} else {
		_capteurs_actualises[noCapteur] = false;
		debloquerVariables();
		envoyerMessage(0x01_b, noCapteur);
	}
}

bool CarteIO2018::lireUnCapteur(uint8_t noCapteur) {
	if(noCapteur > NBCAPTEURS || noCapteur <= 0) {
		logError("Numéro de capteur hors limite pour la carte IO");
		return false;
	} else {
		// logDebug("|||||| LIRE CAPTEUR N°", (int)noCapteur, " : ", _capteurs[noCapteur-1]);
		return _capteurs[noCapteur];
	}
}

bool CarteIO2018::capteurEstActualise(uint8_t noCapteur) {
	if(noCapteur > NBCAPTEURS || noCapteur <= 0) {
		logError("Numéro de capteur hors limite pour la carte IO");
		return false;
	} else {
		return _capteurs_actualises[noCapteur];
	}
}

bool CarteIO2018::actualiserPuisLireUnCapteur(uint8_t noCapteur) {
	int demandesActualisation = 0;
	actualiserUnCapteur(noCapteur);
	while(!capteurEstActualise(noCapteur)) {
		sleep(10_ms);
		demandesActualisation++;
		// sécurité : toutes les 5 vérifications, on renvoie le message
		if((demandesActualisation % 5) == 0) {
			actualiserUnCapteur(noCapteur);
		}
	}

	bool valeurCapteur = lireUnCapteur(noCapteur);
	// logDebug("%%% VALEUR CAPTEUR : ", valeurCapteur);

	return valeurCapteur;
}
