// Socket.h - classe representant une socket.
// 2005-2010 par Lionel Fuentes. Si vous utilisez cette classe dans un de vos projets, un mail
// de remerciement a funto66 at gmail.com ferait plaisir :)
// Version 1.2

#ifndef SOCKET_H
#define SOCKET_H

#ifdef WIN32
// ajoute la librairie sous vc++
#ifdef _MSC_VER
#pragma comment (lib, "ws2_32.lib")
#endif
#include <winsock2.h>
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#endif

#include <cstdint>
#include <utility>

// Enumeration des etats ou peut etre une Socket.
enum SockState {
	SOCK_FREE=0,
	SOCK_CONNECTED,
	SOCK_LISTENING,
	SOCK_ACCEPTED
};

// Enumeration des protocoles : pour l'instant on en reste a TCP/IP ^^
enum SockProtocol {
	SOCK_TCP=0
};

class Socket {
public:
	Socket(SockProtocol protocol);
	~Socket();

	// Accesseurs
	int getFD() {return _fd;}
	bool isCreated() {return _fd > 0;}
	SockState getState() {return _state;}
	SockProtocol getProtocol() {return _protocol;}
	
	// Connexion a un serveur :
	// -server_adress : addresse du serveur auquel on se connecte
	// -port : numero du port utilise
	bool connect(const char* server_adress, int port);
	
	// Envoi de donnees (d'un client vers un serveur) :
	// -data : pointeur vers les donnees
	// -nb_bytes : taille des donnees a envoyer, en octets
	int send(const void* data, int nb_bytes);
	
	// Envoi de donnees (d'un serveur vers un client) :
	// -client_socket : pointeur vers la Socket correspondant au client a qui envoyer
	// les donnees (obtenu via Accept())
	// -data : pointeur vers les donnees
	// -nb_bytes : taille des donnees a envoyer, en octets
	int send(Socket &client_socket, const void* data, int nb_bytes);
	
	// Reception de donnees (donnees allant d'un serveur vers un client) :
	// -buffer : pointeur vers l'endroit ou l'on doit stocker les donnees
	// -max_bytes : taille du buffer en octets, nombre maximal de donnees pouvant
	// etre retournees
	int receive(void* buffer, int max_bytes);
	
	// Reception de donnees (donnees allant d'un client vers un serveur) :
	// -client_socket : socket du client qui nous envoie les donnees
	// -buffer : pointeur vers l'endroit ou l'on doit stocker les donnees
	// -max_bytes : taille du buffer en octets, nombre maximal de donnees pouvant
	// etre retournees
	int receive(Socket &client_socket, void* buffer, int max_bytes);
	
	// Envoi d'un paquet (d'un client vers un serveur) :
	// -data : pointeur vers les donnees
	// -nb_bytes : taille des donnees a envoyer, en octets
	// A la difference de Send(), on rajoute un header de 4 octets indiquant la taille
	// du paquet. Un SendMsg() correspond a un ReceiveMsg().
	bool sendMsg(const void* data, int nb_bytes);
	
	// Envoi d'un paquet (d'un serveur vers un client) :
	// -client_socket : pointeur vers la Socket correspondant au client a qui envoyer
	// les donnees (obtenu via Accept())
	// -data : pointeur vers les donnees
	// -nb_bytes : taille des donnees a envoyer, en octets
	// A la difference de Send(), on rajoute un header de 4 octets indiquant la taille
	// du paquet. Un SendMsg() correspond a un ReceiveMsg().
	bool sendMsg(Socket &client_socket, const void* data, int nb_bytes);
	
	// Reception d'un paquet (donnees allant d'un serveur vers un client) :
	// -buffer : pointeur vers l'endroit ou l'on doit stocker les donnees
	// -max_bytes : taille du buffer en octets, nombre maximal de donnees pouvant
	// etre retournees
	// Correspond a un SendMsg()
	int receiveMsg(void* buffer, int max_bytes);
	
	// Reception d'un paquet (donnees allant d'un client vers un serveur) :
	// -client_socket : socket du client qui nous envoie les donnees
	// -buffer : pointeur vers l'endroit ou l'on doit stocker les donnees
	// -max_bytes : taille du buffer en octets, nombre maximal de donnees pouvant
	// etre retournees
	// Correspond a un SendMsg()
	int receiveMsg(Socket &client_socket, void* buffer, int max_bytes);
	
	// Pareil que ReceiveMsg mais ces fonctions allouent la memoire necessaire
	// pour le buffer de reception avec new[].
	int receiveNewMsg(void** buffer);
	int receiveNewMsg(Socket &client_socket, void** buffer);
	
	// Pour un client : renvoie true si le serveur nous a envoye un message, qui
	// est dans la file d'attente, sinon renvoie false.
	bool waitsForAMessage();
	
	// Pour un serveur : renvoie true si le client specifie nous a envoye un message,
	// qui est donc dans la file d'attente, sinon renvoie false.
	// -client_socket : client susceptible de nous avoir envoye un message.
	bool waitsForAMessageFrom(Socket &client_socket);
	
	// Mise sur ecoute (pour un serveur) :
	// -port : numero du port a ecouter
	// -max_queue : nombre maximum de clients pouvant attendre dans une file
	bool listen(int port, int max_queue=10);
	
	// Acceptation d'un nouveau client (pour un serveur)
	// -sock_client : pointeur vers la socket correspondant au nouveau client accepte
	bool accept(Socket &sock_client);
	
	// Ferme la connexion
	void shutDown();

private:
	Socket(const Socket& ref) = delete;
	Socket& operator=(Socket ref) = delete;

#ifdef WIN32
	SOCKET _fd = 0;
#else
	int _fd = 0;
#endif
	SockState _state = SOCK_FREE;
	SockProtocol _protocol;
	sockaddr_in _addr = {};
};

#endif // SOCKET_H
