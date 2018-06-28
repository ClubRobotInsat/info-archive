// TCPIP.h

#ifndef _TCPIP_H_
#define _TCPIP_H_

#include "Communicateur.h"

#ifdef WIN32
#include <windows.h>
#include <winsock2.h>
#else
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

#include <string>

/// Interface de communication série via socket TCP/IP
class TCPIP : public Communicateur {
public:
	// Creer un serveur
	TCPIP(unsigned short port, ITrameListener& robot);
	// lève ErreurOuvertureCommunicationTCPIP si imposible d'ouvrir la connexion

	~TCPIP() override;

	// attendre que le client se connecte pour les communicateurs avec connexion
	void attendreClient() override;

	// Ecrit un octet sur le port
	void envoyer(unsigned char octet) override;
	// lève ErreurEmissionTCPIP si erreur lors de l'envoie d'un octet

	// Attend un octet sur le port et retourne le premier arrivé - BLOQUANT
	unsigned char lire() override;
	// si le client c'est deconnecte retourne 0 et met a false clientConnecte

	// force une déconnexion et attend la fin du thread de réception
	void cleanUp() override;

	// erreur si la connexion n'est pas ouverte
	class ErreurOuvertureTCPIP : public Communicateur::ErreurOuvertureCommunication {
	public:
		explicit ErreurOuvertureTCPIP(std::string msg) : ErreurOuvertureCommunication(msg) {}
	};

	// erreur de lecture dans le canal de communication
	class ErreurReceptionTCPIP : public Communicateur::ErreurReceptionCommunication {
	public:
		explicit ErreurReceptionTCPIP(std::string msg) : ErreurReceptionCommunication(msg) {}
	};

	// erreur d'ecriture dans le canal de communication
	class ErreurEmissionTCPIP : public Communicateur::ErreurEmissionCommunication {
	public:
		explicit ErreurEmissionTCPIP(std::string msg) : ErreurEmissionCommunication(msg) {}
	};

private:
// Descripteur du socket TCP/IP
#ifdef WIN32
	SOCKET _sockfd;
	SOCKET _sockBis;
#else
	int _sockfd;
	int _sockBis;
#endif
};

#endif
