/*****************************************************************
 * CarteConvertisseurs2007.cpp *
 * Implémente la classe gérant les convertisseurs A/N 2006/2007 *
 *****************************************************************/

#include "CarteConvertisseurs2007.h"

CarteConvertisseurs2007::CarteConvertisseurs2007(Commun::CAN& can, uint8_t id)
        : Carte(can, id), _telemetre(0), _telemetreOk(false), _luminosite(0), _luminositeOk(false) {}

void CarteConvertisseurs2007::traiterMessage(Trame const& message) {
	switch(message.getCmd()) {
		case 0x01_b: // Réponse télémètre
			if(message.getNbDonnees() == 2) {
				bloquerVariables();
				_telemetre = message.getDonnee<uint16_t>(0);
				_telemetreOk = true;
				debloquerVariables();
			}
			break;
		case 0x02_b: // Réponse capteur de luminosité
			if(message.getNbDonnees() == 2) {
				bloquerVariables();
				_luminosite = message.getDonnee<uint16_t>(0);
				_luminositeOk = true;
				debloquerVariables();
			}
			break;
		default:
			this->Carte::traiterMessage(message);
			break;
	}
}

// Demande au télémètre d'envoyer une mesure
void CarteConvertisseurs2007::actualiserTelemetre() {
	_telemetreOk = false;

	envoyerMessage(0x01_b, 0xFF_b);
}

// Vérifie que la mesure du télémètre est à jour
bool CarteConvertisseurs2007::telemetreActualise() {
	return _telemetreOk;
}

// Récupère la mesure du télémètre
uint16_t CarteConvertisseurs2007::lireTelemetre() {
	return _telemetre;
}

// Demande au capteur de luminosité d'envoyer une mesure
void CarteConvertisseurs2007::actualiserLuminosite() {
	_luminositeOk = false;

	envoyerMessage(0x02_b, 0xFF_b);
}

// Vérifie que la mesure du capteur de luminosité est à jour
bool CarteConvertisseurs2007::luminositeActualisee() {
	return _luminositeOk;
}

// Récupère la mesure du capteur de luminosité
unsigned short CarteConvertisseurs2007::lireLuminosite() {
	return _luminosite;
}
