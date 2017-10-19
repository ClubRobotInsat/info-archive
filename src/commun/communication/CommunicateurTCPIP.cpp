// TCPIP.cpp

#include "CommunicateurTCPIP.h"
#include <cstring>

#ifdef WIN32
typedef int socklen_t;
#else
#define INVALID_SOCKET -1
#define closesocket(param) close(param)
#endif

// Creer un serveur
TCPIP::TCPIP(unsigned short port, ITrameListener& robot)
        : Communicateur("TCPIP" + to_string(port), robot), _sockfd(INVALID_SOCKET), _sockBis(INVALID_SOCKET) {
#ifdef WIN32
	WSAData data;
	if(WSAStartup(MAKEWORD(2, 2), &data) != 0)
		throw ErreurOuvertureTCPIP("Impossible d'initialiser l'API Winsock");
#endif

	// creation du socket
	_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // Domaine Internet, par flux, protocole TCP
	if(_sockfd == INVALID_SOCKET)
		throw ErreurOuvertureTCPIP("Impossible de creer le socket TCPIP");

	struct sockaddr_in adrLocal;

	// affectation de l'adresse local
	memset((char*)&adrLocal, 0, sizeof(adrLocal));
	adrLocal.sin_family = AF_INET;
	adrLocal.sin_port = htons(port);
	adrLocal.sin_addr.s_addr = INADDR_ANY;

	// association @ socket <=> representation interne
	if(::bind(_sockfd, (struct sockaddr*)&adrLocal, sizeof(adrLocal)) == -1)
		throw ErreurOuvertureTCPIP("Impossible d'associer le socket TCPIP");

	commencerReception();
}

TCPIP::~TCPIP() {
#ifdef WIN32
	WSACleanup();
#endif
}

// attendre que le client se connecte pour les communicateurs avec connexion
void TCPIP::attendreClient() {
	logInfo("attente d'un client...");
	sockaddr_in adrDest;

	// attente de la demande de connexion
	if(listen(_sockfd, 5) == -1)
		throw ErreurOuvertureTCPIP("Échec de la demande de connexion TCPIP");

	// accepte la demande de connexion
	int lgAdrDest = sizeof(adrDest);
	_sockBis = accept(_sockfd, (sockaddr*)&adrDest, (socklen_t*)&lgAdrDest);
	if(_sockBis == INVALID_SOCKET)
		throw ErreurOuvertureTCPIP("Échec de l'acceptation de la demande de connexion TCPIP");

	connexion();

	logInfo("client connecte");
}

// Ecrit un octet sur le socket
void TCPIP::envoyer(uint8_t octet) {
// lève ErreurEmissionTCPIP si erreur lors de l'envoie d'un octet
#ifdef WIN32
	int flags = 0;
#elif __APPLE__
	int flags = SO_NOSIGPIPE;
#else
	int flags = MSG_NOSIGNAL;
#endif
	int longueur = send(_sockBis, &octet, 1, flags);

	// Envoi de données (d'un client vers un serveur) :
	if(longueur <= 0) {
		deconnexion();
		throw ErreurEmissionTCPIP("Echec d'envoi d'octet sur le port TCPIP");
	}
}

// Attend un octet sur le socket et retourne le premier arrivé - BLOQUANT
uint8_t TCPIP::lire() {
	// si le client s'est deconnecte retourne 0 et met a false "connecte"
	uint8_t octet;
	int longueur = recv(_sockBis, &octet, 1, 0);
	if(longueur <= 0) {
		deconnexion();
		return 0;
	}
	return octet;
}

// force une déconnexion, et attend la fin du thread de réception.
void TCPIP::cleanUp() {
	_cleaningUp = true;

	if(_sockBis != INVALID_SOCKET) {
		shutdown(_sockBis, 2);
		closesocket(_sockBis);
		_sockBis = INVALID_SOCKET;
	}

	if(_sockfd != INVALID_SOCKET) {
		shutdown(_sockfd, 2);
		closesocket(_sockfd);
		_sockfd = INVALID_SOCKET;
	}

	attendreFinReception();

	deconnexion();
}
