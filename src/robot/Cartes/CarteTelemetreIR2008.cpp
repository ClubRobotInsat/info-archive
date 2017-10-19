/*****************************************************************
 * CarteTelemetreIR2008.cpp *
 * Implémente la classe gérant les _telemetre IR 2008 *
 *****************************************************************/

#include "CarteTelemetreIR2008.h"

CarteTelemetreIR2008::CarteTelemetreIR2008(Commun::CAN& can, uint8_t id) : Carte(can, id) {
	for(int i = 0; i < NOMBRE_TELEMETRE; i++) {
		_telemetre[i] = 0;
		_telemetreOk[i] = 0;
	}
}

void CarteTelemetreIR2008::traiterMessage(Trame const& message) {
	switch(message.getCmd()) {
		case 0x01_b: // Réponse télémètre
			if(message.getNbDonnees() == 3 && message.getDonnee(0) < NOMBRE_TELEMETRE) {
				bloquerVariables();
				_telemetre[message.getDonnee(0)] = message.getDonnee<uint16_t>(1);
				_telemetreOk[message.getDonnee(0)] = true;
				debloquerVariables();
			}
			break;
		case 0x02_b:
			if(message.getNbDonnees() == 6) {
				bloquerVariables();
				_telemetre[0] = message.getDonnee<uint16_t>(0);
				_telemetre[1] = message.getDonnee<uint16_t>(2);
				_telemetre[2] = message.getDonnee<uint16_t>(4);
				_telemetreOk[0] = true;
				_telemetreOk[1] = true;
				_telemetreOk[2] = true;
				debloquerVariables();
			}
			break;
		default:
			this->Carte::traiterMessage(message);
			break;
	}
}

// Demande au télémètre d'envoyer une mesure
void CarteTelemetreIR2008::actualiserTelemetre(uint8_t numero_telemetre) {
	if(numero_telemetre >= NOMBRE_TELEMETRE)
		// TODO lever erreur
		return;

	_telemetreOk[numero_telemetre] = false;

	envoyerMessage(0x01_b, numero_telemetre);
}

// Vérifie que la mesure du télémètre est à jour
bool CarteTelemetreIR2008::telemetreActualise(uint8_t numero_telemetre) {
	if(numero_telemetre >= NOMBRE_TELEMETRE)
		// TODO lever erreur
		return false;

	return _telemetreOk[numero_telemetre];
}

// Récupère la mesure du télémètre
uint16_t CarteTelemetreIR2008::lireTelemetre(uint8_t numero_telemetre) {
	if(numero_telemetre >= NOMBRE_TELEMETRE)
		// TODO lever erreur
		return 0;

	return _telemetre[numero_telemetre];
}

void CarteTelemetreIR2008::activerEnvoiAuto() {
	envoyerMessage(0x03_b, 0x01_b);
}
void CarteTelemetreIR2008::desactiverEnvoiAuto() {
	envoyerMessage(0x03_b, 0x00_b);
}
