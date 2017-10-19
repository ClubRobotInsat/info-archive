/************************************************************
 * CarteAlimentation2010.cpp *
 * Spécifie la classe gérant les Alimentation 2009/2010 *
 ************************************************************/

#include "CarteAlimentation2010.h"

#include <cstring>
#include <ctype.h>

CarteAlimentation2010::CarteAlimentation2010(Commun::CAN& can, uint8_t id) : Carte(can, id), _tensionActualisee(false) {
	_tension = 0;
}

void CarteAlimentation2010::traiterMessage(Trame const& message) {
	switch(message.getCmd()) {
		case 0x01_b: // Reception d'un octet
			if(message.getNbDonnees() == 2) {
				bloquerVariables();
				_tension = message.getDonnee<std::int16_t>(0);
				_tensionActualisee = true;
				debloquerVariables();
			}
			break;
		default:
			this->Carte::traiterMessage(message);
			break;
	}
}

// Envoie l'ordre d'actualiser la _tension a la carte
void CarteAlimentation2010::actualiserTension() {
	_tensionActualisee = false;

	envoyerMessage(0x01_b, uint8_t(0x00_b));
}

// Verifie si la _tension a ete actualisee
bool CarteAlimentation2010::verifierTensionActualisee() {
	return _tensionActualisee.exchange(false);
}

// Retourne la _tension
int CarteAlimentation2010::getTension() {
	return _tension;
}
