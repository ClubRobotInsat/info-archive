/*******************************************************************
 * CarteElectrovannes2016.h *
 * Spécifie la classe gérant les électrovannes 2016 *
 *******************************************************************/

#include "CarteElectrovannes2016.h"

#define CMD_ACTIV 0x1
#define CMD_DESACTIV 0x2
#define VANNES_ALL 0x6

CarteElectrovannes2016::CarteElectrovannes2016(Commun::CAN& can, uint8_t id) : Carte(can, id) {
	bloquerVariables();
	for(int i = 0; i < VANNES_COUNT; i++) {
		this->_states[i] = false;
	}
	debloquerVariables();
}

void CarteElectrovannes2016::traiterMessage(Trame const& message) {
	int vanneId = message.getDonnee(0);
	switch(message.getCmd()) {
		case CMD_ACTIV: // Acquittement activation
			if(vanneId == VANNES_ALL) {
				for(int i = 0; i < VANNES_ALL; i++) {
					_states[i] = true;
				}
			} else if(vanneId >= 0 && vanneId < VANNES_COUNT) {
				_states[vanneId] = true;
			} else {
				logError("[CarteElectrovannes] : Wrong ack ! vanneId = ", vanneId);
			}
			_ack = true;
			break;
		case CMD_DESACTIV: // Acquittement désactivation
			if(vanneId == VANNES_ALL) {
				for(int i = 0; i < VANNES_ALL; i++) {
					_states[i] = false;
				}
			} else if(vanneId >= 0 && vanneId < VANNES_COUNT) {
				_states[vanneId] = false;
			} else {
				logError("[CarteElectrovannes] : Wrong ack ! vanneId = ", vanneId);
			}
			_ack = true;
			break;
		default:
			this->Carte::traiterMessage(message);
			break;
	}
}


void CarteElectrovannes2016::ouvrirVanne(uint8_t vanneId) {
	_ack = false;
	if(vanneId < 0 || vanneId > VANNES_ALL)
		logError("ID de vanne '", vanneId, "' out of range.");
	logDebug8("Message elec ouverture : ", this->make_trame(CMD_ACTIV, vanneId).toStringLong());
	envoyerMessage(this->make_trame(CMD_ACTIV, vanneId));
}

void CarteElectrovannes2016::fermerVanne(uint8_t vanneId) {
	_ack = false;
	if(vanneId < 0 || vanneId > VANNES_ALL)
		logError("ID de vanne '", vanneId, "' out of range.");
	logDebug8("Message elec ouverture : ", this->make_trame(CMD_ACTIV, vanneId).toStringLong());
	envoyerMessage(this->make_trame(CMD_DESACTIV, vanneId));
}


// Obtient l'etat de la vanne.
bool CarteElectrovannes2016::etatVanne(uint8_t vanneId) {
	if(vanneId < 0 || vanneId > VANNES_ALL)
		logError("ID de vanne '", vanneId, "' out of range.");

	// Vérification de toutes les vannes
	if(vanneId == VANNES_ALL) {
		for(int i = 0; i < VANNES_ALL; i++) {
			if(!_states[vanneId])
				return false;
		}
		return true;
	}

	return _states[vanneId];
}
// Vérifie qu'un ack ait été reçu
bool CarteElectrovannes2016::ackRecu() {
	return _ack;
}
