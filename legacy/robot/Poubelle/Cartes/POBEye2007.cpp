/************************************************************
 * POBEye2007.cpp *
 * Implemente la classe de la POBEye 2006/2007 *
 ************************************************************/

#include "POBEye2007.h"

POBEye2007::POBEye2007(Commun::CAN& can, uint8_t id)
        : Carte(can, id), _reception(false), _nombreCanettes(0), _nombreBouteilles(0) {
	std::memset(_canettes, 0, sizeof(_canettes));
	std::memset(_bouteilles, 0, sizeof(_bouteilles));
}

void POBEye2007::traiterMessage(Trame const& message) {
	switch(message.getCmd()) {
		case 0x01_b: // Liste de dechets
			if(message.getNbDonnees() == 3) {
				bloquerVariables();
				if(message.getDonnee(0) == 0) {
					_canettes[_nombreCanettes].x = message.getDonnee(1);
					_canettes[_nombreCanettes].y = message.getDonnee(2);
					_nombreCanettes++;
				} else if(message.getDonnee(0) == 1) {
					_bouteilles[_nombreBouteilles].x = message.getDonnee(1);
					_bouteilles[_nombreBouteilles].y = message.getDonnee(2);
					_nombreBouteilles++;
				}
				debloquerVariables();
			} else
				printf("[ATTENTION] POB-Eye : recu un dechet avec taille != 3\n");
			break;
		case 0x05_b: // Fin de liste de dechets
			if(message.getNbDonnees() == 1) {
				_reception = false;
			} else
				printf("[ATTENTION] POB-Eye : recu un message \"fin de liste de dechets\" avec une taille != 1\n");
			break;
		default:
			printf("[ATTENTION] POB-Eye : message 0x%x inconnu\n", message.getCmd());
			break;
	}
}

/// Envoie un message a la POB-Eye pour lui demander ou sont les barycentres des _canettes et _bouteilles
void POBEye2007::actualiserBarycentres() {
	envoyerMessage(0x01_b, 0x00_b);

	bloquerVariables();

	// On va commencer a recevoir des barycentres, d'ou :
	_reception = true;
	_nombreBouteilles = 0;
	_nombreCanettes = 0;

	debloquerVariables();
}

/// Vérifie que la liste des barycentres est à jour
bool POBEye2007::barycentresActualises() {
	return !_reception;
}

/// Retourne le nombre de _canettes vues
int POBEye2007::nombreCanettesVues() {
	return _nombreCanettes;
}

/// Retourne la position du barycentre d'une canette
Pixel POBEye2007::barycentreCanette(int canette) {
	Pixel resultat;

	bloquerVariables();
	resultat = _canettes[canette];
	debloquerVariables();

	return resultat;
}

/// Retourne le nombre de _bouteilles vues
int POBEye2007::nombreBouteillesVues() {
	return _nombreBouteilles;
}

/// Retourne la position du barycentre d'une bouteille
Pixel POBEye2007::barycentreBouteille(int bouteille) {
	Pixel resultat;

	bloquerVariables();
	resultat = _bouteilles[bouteille];
	debloquerVariables();

	return resultat;
}
