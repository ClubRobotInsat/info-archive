#include "CarteIO2018.h"
#include "../../core/Simulateur.h"
#include <cstdint>
#include <cstring>

CarteIO2018::CarteIO2018(unsigned char id, RobotLogic& robot, IContacteursV2& iContacteurs)
        : Carte(id, robot), _iContacteurs(iContacteurs) {
	std::memset(_capteurs, 1, sizeof(_capteurs));
	for(int i = 0; i < NBCAPTEURS; i++)
		_capteurs_actualises[i] = false;
}

// Traite les données fournies par l'IA
void CarteIO2018::traiterTrame(Trame const& t) {
	switch(t.getCmd()) {
		// ping
		case 0x00_b: {
			if(t.getNbDonnees() == 1 && t.getDonnee(0) == 0x55_b) {
				// ping => repondre un pong
				envoyerPong();
			} else
				throw Trame::ErreurTrameNonTraitee(t);
			break;
		}
		// réception de l'état d'un capteur
		case 0x01_b: {
			if(t.getNbDonnees() == 2) {
				uint8_t noCapteur = t.getDonnee(0);
				_capteurs[noCapteur] = static_cast<bool>(t.getDonnee(1));
				_capteurs_actualises[noCapteur] = true;
			} else
				throw Trame::ErreurTrameNonTraitee(t);
			break;
		}
		default:
			throw Trame::ErreurTrameNonTraitee(t);
	}
}

// envoyer la reponce a un ping
void CarteIO2018::envoyerPong() {
	Trame aEnvoyer(this->getId(), 0);
	aEnvoyer.addByte(0xAA_b);
	this->envoyer(aEnvoyer);
	std::cout << "CarteIO2018::envoyerPong ping -> pong" << std::endl;
}

bool CarteIO2018::actualiserPuisLireUnCapteur(uint8_t noCapteur) {
	int demandesActualisation = 0;
	actualiserUnCapteur(noCapteur);
	while(!capteurEstActualise(noCapteur)) {
		sleep(10_ms);
		demandesActualisation++;
		// sécurité : toutes les 5 vérifications, on renvoie le message
		if((demandesActualisation % 5) == 0) {
			this->actualiserUnCapteur(noCapteur);
		}
	}

	bool valeurCapteur = lireUnCapteur(noCapteur);

	return valeurCapteur;
}


// Envoie la demande de lecture des capteurs
void CarteIO2018::actualiserUnCapteur(uint8_t noCapteur) {
	if(noCapteur > NBCAPTEURS) {
		throw ErreurNumContact(noCapteur);
	}
	_capteurs_actualises[noCapteur] = false;
	Trame aEnvoyer(this->getId(), 1);
	aEnvoyer.addBytes({noCapteur});
	this->envoyer(aEnvoyer);
}

bool CarteIO2018::lireUnCapteur(uint8_t noCapteur) {
	if(noCapteur > NBCAPTEURS || noCapteur <= 0) {
		throw ErreurNumContact(noCapteur);
	}
	return _capteurs[noCapteur];
}

bool CarteIO2018::capteurEstActualise(uint8_t noCapteur) {
	if(noCapteur > NBCAPTEURS || noCapteur <= 0) {
		throw ErreurNumContact(noCapteur);
	}
	return _capteurs_actualises[noCapteur];
}