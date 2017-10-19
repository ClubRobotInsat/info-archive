/*****************************************************************
 * CarteTelemetres2009.cpp *
 * Implémente la classe gérant les _telemetre IR 2009 *
 *****************************************************************/

#include "CarteTelemetres2009.h"

CarteTelemetres2009::CarteTelemetres2009(Commun::CAN& can, uint8_t id, int nbTelemetres)
        : Carte(can, id), _nbTelemetres(nbTelemetres) {
	for(int i = 0; i < NB_MAX_TELEMETRES; i++) {
		_distances[i] = 0;
		_telemetresOk[i] = false;
	}
}

void CarteTelemetres2009::traiterMessage(Trame const& message) {
	switch(message.getCmd()) {
		case 0x01_b: // Récupération d'une distance
			if(message.getNbDonnees() == 3 && message.getDonnee(0) < _nbTelemetres) {
				bloquerVariables();
				_distances[message.getDonnee(0)] = message.getDonnee<uint16_t>(1);
				_telemetresOk[message.getDonnee(0)] = true;
				debloquerVariables();
			}
			break;
		case 0x02_b: // Récupération de toutes les _distances gérées par la carte
			if(message.getNbDonnees() == 6) {
				bloquerVariables();
				_distances[0] = message.getDonnee<uint16_t>(0);
				_distances[1] = message.getDonnee<uint16_t>(2);
				_distances[2] = message.getDonnee<uint16_t>(4);
				_telemetresOk[0] = true;
				_telemetresOk[1] = true;
				_telemetresOk[2] = true;
				debloquerVariables();
			}
			break;
		default:
			this->Carte::traiterMessage(message);
			break;
	}
}

// Demande au télémètre d'envoyer une mesure
void CarteTelemetres2009::actualiserTelemetre(uint8_t numeroTelemetre) {
	if(numeroTelemetre >= _nbTelemetres) {
		logError("CarteTelemetres2009 : demande d'actualisation du _telemetre ",
		         (int)numeroTelemetre,
		         "alors que seulement ",
		         (int)_nbTelemetres,
		         " _telemetres sont gérés\n");
		return;
	}

	_telemetresOk[numeroTelemetre] = false;

	envoyerMessage(0x01_b, numeroTelemetre);
}

// Vérifie que la mesure du télémètre est à jour
bool CarteTelemetres2009::telemetreActualise(uint8_t numeroTelemetre) {
	if(numeroTelemetre >= _nbTelemetres) {
		logError("CarteTelemetres2009 : vérification de l'actualisation du _telemetre ",
		         (int)numeroTelemetre,
		         "alors que seulement ",
		         (int)_nbTelemetres,
		         " _telemetres sont gérés\n");
		return false;
	}

	return _telemetresOk[numeroTelemetre];
}

// Récupère la mesure du télémètre
uint16_t CarteTelemetres2009::lireDistanceTelemetre(uint8_t numeroTelemetre) {
	if(numeroTelemetre >= _nbTelemetres) {
		logError("CarteTelemetres2009 : lecture du _telemetre ", (int)numeroTelemetre, "alors que seulement ", (int)_nbTelemetres, " _telemetres sont gérés\n");
		return 0;
	}

	return _distances[numeroTelemetre];
}

// Active l'envoi de trames toutes les 50 ms
void CarteTelemetres2009::activerEnvoiAuto() {
	envoyerMessage(0x03_b, 0x01_b);
}

// Désactive l'envoi de trames toutes les 50 ms
void CarteTelemetres2009::desactiverEnvoiAuto() {
	envoyerMessage(0x03_b, 0x00_b);
}
