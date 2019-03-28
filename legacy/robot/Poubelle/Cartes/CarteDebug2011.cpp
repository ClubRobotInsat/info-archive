/******************************************************************
 * CarteDebug2011.h *
 * Spécifie la classe gérant les actions de *
 * très haut niveau sur le robot 2011 (ramasser, déposer, etc...) *
 *******************************************************************/

#include "CarteDebug2011.h"

CarteDebug2011::CarteDebug2011(Commun::CAN& can, uint8_t id) : Carte(can, id) {
	_aPionSousBras[0] = false;
	_aPionSousBras[1] = false;
	_contacteursBrasActualises = false;

	_aPionEntrePinces = false;
	_contacteurPincesActualise = false;
}

void CarteDebug2011::traiterMessage(Trame const& message) {
	switch(message.getCmd()) {
		break;
		case 0x01_b:
			if(message.getNbDonnees() == 2) {
				bloquerVariables();
				_contacteursBrasActualises = true;
				_aPionSousBras[0] = message.getDonnee(0);
				_aPionSousBras[1] = message.getDonnee(1);
				debloquerVariables();
			}
			break;
		case 0x02_b:
			if(message.getNbDonnees() == 1) {
				bloquerVariables();
				_contacteurPincesActualise = true;
				_aPionEntrePinces = message.getDonnee(0);
				debloquerVariables();
			}
			break;
		default:
			this->Carte::traiterMessage(message);
			break;
	}
}

// Lecture de l'état des contacteurs des bras
void CarteDebug2011::actualiserContacteursBras() {
	_contacteursBrasActualises = false;

	envoyerMessage(0x01_b, 0xFF_b);
}

bool CarteDebug2011::sontContacteursBrasActualises() {
	return _contacteurPincesActualise;
}

// Lecture de l'état du contacteur des pinces
void CarteDebug2011::actualiserContacteurPinces() {
	_contacteurPincesActualise = false;

	envoyerMessage(0x02_b, 0xFF_b);
}

bool CarteDebug2011::estContacteurPincesActualise() {
	return _contacteurPincesActualise;
}

bool CarteDebug2011::aPionEntresPinces() {
	return _aPionEntrePinces;
}

// Ramassage d'un pion avec les pinces
void CarteDebug2011::ramasserPionPinces() {
	envoyerMessage(0x03_b, 0x55_b);
}

// Dépose d'un pion avec les pinces
void CarteDebug2011::deposerPionPinces() {
	envoyerMessage(0x04_b, 0x55_b);
}
