/*****************************************************************
 * CarteAlimentation2009.cpp *
 * Implémente la classe gérant la carte alimentation 2009 *
 *****************************************************************/

#include "CarteAlimentation2009.h"

CarteAlimentation2009::CarteAlimentation2009(Commun::CAN& can, uint8_t id) : Carte(can, id), _infosOk(false) {
	_tension = 0;
	_adaptateur = false;
}

void CarteAlimentation2009::traiterMessage(Trame const& message) {
	switch(message.getCmd()) {
		case 0x01_b: // Récupération d'une distance
			if(message.getNbDonnees() == 5) {
				bloquerVariables();
				_tension = message.getDonnee<int32_t>(0);
				_adaptateur = message.getDonnee(4);
				_infosOk = true;
				debloquerVariables();
			}
			break;
		default:
			this->Carte::traiterMessage(message);
			break;
	}
}

// Demande à la carte d'envoyer une mesure
void CarteAlimentation2009::actualiserInfos() {
	_infosOk = false;

	envoyerMessage(0x01_b, 0x00_b);
}

// Vérifie que les mesures sont à jour
bool CarteAlimentation2009::infosActualisees() {
	return _infosOk;
}

// Vérifie que l'adapteur est en position
bool CarteAlimentation2009::adaptateurPresent() {
	return _adaptateur;
}

// Récupère la mesure de la _tension
float CarteAlimentation2009::lireTension() {
	return _tension;
}

// Active l'envoi de trames toutes les secondes
void CarteAlimentation2009::activerEnvoiAuto() {
	envoyerMessage(0x03_b, 0x01_b);
}

// Désactive l'envoi de trames toutes les secondes
void CarteAlimentation2009::desactiverEnvoiAuto() {
	envoyerMessage(0x03_b, 0x00_b);
}
