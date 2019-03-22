#include "Carte.h"

/// Initialise la carte
Carte::Carte(Commun::CAN& can, uint8_t id)
        : _monCan(can), _id(id), _pongRecu(false), _version(0), _versionActualisee(false) {}

/// Transmet les données pour envoi vers l'électronique
void Carte::envoyerMessage(Trame trame, bool rejeu) {
	_monCan.envoyerTrame(std::move(trame), rejeu);
}

/// Retourne l'ID de la carte dans le robot
uint8_t Carte::getId() {
	return _id;
}

/// Bloque l'accès aux variables membres
void Carte::bloquerVariables() {
	_mutexVariables.lock();
}

/// Débloque l'accès aux variables membres
void Carte::debloquerVariables() {
	_mutexVariables.unlock();
}

/// Traite les messages communs a toutes les cartes
void Carte::traiterMessage(Trame const& message) {
	if(message.getCmd() == 0x00_b) {
		switch(message.getDonnee(0)) {
			case 0xaa_b: // Pong
				_pongRecu = true;
				break;
			case 0x01_b: // Reponse à demande de version
				_version = message.getDonnee<uint16_t>(1);
				_versionActualisee = true;
				break;
			case 0x02_b: // Signal de boot
				break;
		}
	} else {
		logWarn("Carte 0x", Byte(this->getId()), ": message ", Byte(message.getCmd()), " inconnu\n");
	}
}

/// Envoie un ping a la carte
void Carte::envoyerPing() {
	this->envoyerMessage(0x00_b, 0x55_b);
}

/// Verifie que la carte a repondu par un pong
bool Carte::verifierPong() {
	// Si _pongRecu, _pongRecu = false. On retourne l'acienne valeur dans tous les cas
	return _pongRecu.exchange(false);
}

/// Demande la version de la carte
void Carte::actualiserVersion() {
	this->envoyerMessage(0x00_b, 0x01_b);
	_versionActualisee = false;
}

/// Verifie que la version de la carte a ete actualisee
bool Carte::versionActualisee() {
	return _versionActualisee;
}

/// Retourne la version de la carte
uint16_t Carte::getVersion() {
	return _version;
}

bool Carte::pingPong() {
	this->envoyerPing();
	int retries = 0;
	while(retries <= 150) {
		if(this->verifierPong()) {
			return true;
		}
		retries += 1;
		sleep(10_ms);
	}
	return false;
}