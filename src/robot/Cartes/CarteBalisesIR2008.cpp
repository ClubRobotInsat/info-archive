/*********************************************************
 * CarteBalisesIR2008.cpp *
 * Implémente la classe gérant les balises IR 2008 *
 *********************************************************/

#include "CarteBalisesIR2008.h"
#include <cstring>

CarteBalisesIR2008::CarteBalisesIR2008(Commun::CAN& can, uint8_t id) : Carte(can, id) {
	_recepteurs = 0;
	_recepteursOk = false;
}

void CarteBalisesIR2008::traiterMessage(Trame const& message) {
	switch(message.getCmd()) {
		case 0x01_b: // Etat récepteurs
			if(message.getNbDonnees() == 2) {
				bloquerVariables();
				_recepteurs = message.getDonnee<uint16_t>(0);
				_recepteursOk = true;
				debloquerVariables();
			}
			break;
		default:
			this->Carte::traiterMessage(message);
			break;
	}
}

// Demande une mise à jour des récepteurs
void CarteBalisesIR2008::actualiserRecepteurs() {
	bloquerVariables();
	_recepteurs = 0;
	_recepteursOk = false;
	debloquerVariables();

	envoyerMessage(0x01_b, 0xFF_b);
}

// Vérifie que les récepteurs sont à jour
bool CarteBalisesIR2008::recepteursActualises() {
	return _recepteursOk;
}

// Liste les récepteurs captant une balise donnée
uint16_t CarteBalisesIR2008::lireRecepteurs() {
	return _recepteurs;
}

// Active l'envoi automatique des mesures
void CarteBalisesIR2008::activerEnvoiAuto() {
	envoyerMessage(0x02_b, 0x01_b);
}

// Désactive l'envoi automatique des mesures
void CarteBalisesIR2008::desactiverEnvoiAuto() {
	envoyerMessage(0x02_b, 0x00_b);
}
