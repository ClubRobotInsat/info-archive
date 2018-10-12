// Communicateur.h

#ifndef COMMUNICATEUR_H
#define COMMUNICATEUR_H

#include <Commun.h>
#include <log/Log.h>

#include <atomic>
#include <mutex>
#include <string>
#include <thread>

class ITrameListener {
public:
	virtual ~ITrameListener() = default;

	virtual bool onRecu(Trame const& trame, bool isAck) = 0;
};

class Communicateur {
public:
	// constructeur
	Communicateur(const std::string& port, ITrameListener& robot);
	// lève ErreurOuvertureCommunication si imposible d'ouvrir la connexion

	// destructeur
	virtual ~Communicateur();

	// attendre que le client se connecte pour les communicateurs avec connexion
	virtual void attendreClient() {}

	// force une déconnexion, et attend la fin du thread de réception.
	// Surchargé dans les classes filles.
	// On doit obligatoirement appeler cette fonction avant de détruire un Communicateur
	// (il est impossible de faire ce qu'elle fait dans le destructeur, car on a besoin que
	// l'objet soit encore construit).
	virtual void cleanUp();

	// vrai si un client est connecte pour les communicateurs avec connexion
	bool estConnecte() {
		return _connecte;
	}

	// lors de l'envoi d'une trame l'envoyer octet par octet dans le canal de communication
	virtual void onEnvoye(Trame const& t, bool isAck = false);
	// cette trame sera detruite apres traitement

	bool lireOctets(uint8_t& id_cmd, uint8_t& idFort, uint8_t& nbDonnees, uint8_t* donnees, uint8_t& num_paquet);

	// thread de reception des trames
	void threadReception();

	// erreur d'ouverture du canal de communication
	class ErreurOuvertureCommunication : public std::runtime_error {
	public:
		ErreurOuvertureCommunication(std::string msg) : std::runtime_error(msg) {}
	};

	// erreur de lecture dans le canal de communication
	class ErreurReceptionCommunication : public std::runtime_error {
	public:
		ErreurReceptionCommunication(std::string msg) : std::runtime_error(msg) {}
	};

	// erreur d'ecriture dans le canal de communication
	class ErreurEmissionCommunication : public std::runtime_error {
	public:
		ErreurEmissionCommunication(std::string msg) : std::runtime_error(msg) {}
	};

private:
	// Robot a qui appartient ce canal de communication
	ITrameListener& _robot;

	// Thread de réception des trames
	std::thread _reception;

	// vrai si le client est connecte
	std::atomic_bool _connecte;
	std::mutex _mutexTrame;

protected:
	// vrai si l'on a appelé cleanup() (i.e. lorsque l'on quitte le simu)
	bool _cleaningUp;

	// nom du port de communication
	std::string _port;

	// Ecrit un octet sur le port
	virtual void envoyer(uint8_t octet) = 0;
	// lève ErreurEmissionCommunication si erreur lors de l'envoi d'un octet

	// Attend un octet sur le port et retourne le premier arrivé - BLOQUANT
	virtual unsigned char lire() = 0;
	// ErreurReceptionTCPIP si erreur lors de la reception d'un octet
	// si le client c'est deconnecte retourne 0 et met a false "connecte"

	// signaler une connexion
	void connexion();

	// signaler une deconnexion
	void deconnexion();

	// lance le thread de réception (appelé par les classes filles)
	void commencerReception();

	// attend le thread de réception
	void attendreFinReception();
};

#endif // COMMUNICATEUR_H
