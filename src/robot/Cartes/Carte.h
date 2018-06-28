#ifndef _CARTE_H_
#define _CARTE_H_

#include "../Commun/Communication/CAN.h"
#include <Commun.h>
#include <atomic>
#include <log/Log.h>
#include <mutex>
#include <unordered_map>


// Interface de contrôle générique d'une carte électronique
class Carte {
public:
	// Initialise la carte
	Carte(Commun::CAN& can, uint8_t id);
	virtual ~Carte() = default;

	// Retourne l'ID de la carte dans le robot
	uint8_t getId();

	// Traite les données fournies par le robot
	virtual void traiterMessage(Trame const& message);

	/// Envoie un ping à la carte
	void envoyerPing();

	/// Verifie que la carte a répondu par un pong
	bool verifierPong();

	/// Demande la version de la carte
	void actualiserVersion();

	/// Vérifie que la version de la carte a été actualisée
	bool versionActualisee();

	/// Retourne la version de la carte
	uint16_t getVersion();

	bool pingPong();

protected:
	template <typename... Args>
	Trame make_trame(uint8_t cmd, Args&&... donnees);

	// Transmet les données pour envoi vers l'électronique
	void envoyerMessage(Trame trame, bool rejeu = true);

	void envoyerMessage(uint8_t cmd, uint8_t donnee) {
		this->envoyerMessage(this->make_trame(cmd, donnee), false);
	}

	// Bloque l'accés aux variables membres
	void bloquerVariables();

	// Débloque l'accès aux variables membres
	void debloquerVariables();

	// Mutex permettant de bloquer l'accès aux variables membres
	std::mutex _mutexVariables;

private:
	// Bus CAN à utiliser pour communiquer avec l'électronique
	Commun::CAN& _monCan;

	// ID de la carte dans le robot
	uint8_t _id;

	std::atomic_bool _pongRecu;

	std::atomic<uint16_t> _version;
	std::atomic_bool _versionActualisee;
};

template <typename... Args>
Trame Carte::make_trame(uint8_t cmd, Args&&... donnees) {
	Trame t(this->getId(), cmd);
	t.addDonnees(donnees...);
	return t;
}

#endif
