/************************************************************
 * CarteColorimetre2009.cpp *
 * Implémente la classe gérant les colorimètres 2009 *
 ************************************************************/

#include "CarteColorimetre2009.h"

CarteColorimetre2009::CarteColorimetre2009(Commun::CAN& can, uint8_t id) : Carte(can, id) {
	std::memset(_couleurs, 0, sizeof(_couleurs));
}

void CarteColorimetre2009::traiterMessage(Trame const& message) {
	switch(message.getCmd()) {
		case 0x01_b: // Reponse etat capteurs
			if(message.getNbDonnees() == 2 * NOMBRE_DE_COULEURS) {
				bloquerVariables();
				for(int couleur = 0; couleur < NOMBRE_DE_COULEURS; couleur++) {
					_couleurs[couleur] = message.getDonnee<uint16_t>(2 * couleur);
				}
				_ack = true;
				debloquerVariables();
			} else
				printf("[ATTENTION] Carte 0x%x : message 0x%x : longueur de trame incorrecte\n", getId(), message.getCmd());
			break;

		case 3_b:
			if(message.getNbDonnees() == 1) {
				bloquerVariables();
				_objetDetecte = message.getDonnee(0);
				_objetDetecteOk = true;
				debloquerVariables();
			} else
				printf("[ATTENTION] Carte 0x%x : message 0x%x : longueur de trame incorrecte\n", getId(), message.getCmd());
			break;

		default:
			this->Carte::traiterMessage(message);
			break;
	}
}

// Demande une actualisation des etats des colorimetres
void CarteColorimetre2009::actualiserCouleurs() {
	_ack = false;

	envoyerMessage(0x00_b, 0xFF_b);
}

// Demande si le capteur a bien ete actualise
bool CarteColorimetre2009::couleursActualise() {
	return _ack;
}

// Retourne l'etat du colorimetre
void CarteColorimetre2009::getCouleurs(uint16_t* rgb) {
	bloquerVariables();
	for(int couleur = 0; couleur < NOMBRE_DE_COULEURS; couleur++)
		rgb[couleur] = _couleurs[couleur];
	debloquerVariables();
}

// accesseur sur une des _couleurs du colorimetre
uint16_t CarteColorimetre2009::getCouleurs(uint8_t couleur) {
	if(couleur >= NOMBRE_DE_COULEURS)
		// TODO lever erreur
		return 0;

	return _couleurs[couleur];
}

// accesseur sur une des _couleurs du colorimetre
Vector3us CarteColorimetre2009::getCouleurs() {
	Vector3us retour;

	bloquerVariables();
	for(int couleur = 0; couleur < NOMBRE_DE_COULEURS; couleur++)
		retour[couleur] = _couleurs[couleur];
	debloquerVariables();

	return retour;
}

void CarteColorimetre2009::activerEnvoiAuto() {
	envoyerMessage(0x00_b, 0x01_b);
}

void CarteColorimetre2009::desactiverEnvoiAuto() {
	envoyerMessage(0x00_b, 0x00_b);
}

// Demande une actualisation des etats des colorimetres
void CarteColorimetre2009::actualiserObjetDetecte() {
	_objetDetecteOk = false;

	envoyerMessage(0x00_b, 0xFF_b);
}

// Demande si le capteur a bien ete actualise
bool CarteColorimetre2009::objetDetecteActualise() {
	return _objetDetecteOk;
}

bool CarteColorimetre2009::verifierObjetDetecte() {
	return _objetDetecte;
}
