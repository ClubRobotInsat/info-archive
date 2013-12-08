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

// Enumeration des etats ou peut etre une Socket.
enum SockState
{
	SOCK_FREE=0,
	SOCK_CONNECTED,
	SOCK_LISTENING,
	SOCK_ACCEPTED
};

// Enumeration des protocoles : pour l'instant on en reste a TCP/IP ^^
enum SockProtocol
{
	SOCK_TCP=0
};

class Socket
{
private:
#ifdef WIN32
	SOCKET m_fd;
#else
	int m_fd;
#endif
	SockState m_state;
	SockProtocol m_protocol;
	sockaddr_in m_addr;

public:
	Socket();
	Socket(const Socket& ref);
	~Socket();

	// Operateur d'affectation
	Socket& operator=(const Socket& ref);

	// Fonctions correspondant aux constructeur/destructeur : toutes les autres methodes
	// doivent etre placees entre les appels a ces 2 methodes.
	bool Create(SockProtocol protocol=SOCK_TCP);
	void Destroy();

	// Methodes statiques pour la compatibilite avec les WinSockets : a appeler au
	// debut et a la fin du programme.
	static bool Init();
	static void CleanUp();

	// Accesseurs
	int GetFD() {return m_fd;}
	bool IsCreated() {return m_fd > 0;}
	SockState GetState() {return m_state;}
	SockProtocol GetProtocol() {return m_protocol;}

	// Connexion a un serveur :
	// -server_adress : addresse du serveur auquel on se connecte
	// -port : numero du port utilise
	bool Connect(const char* server_adress, int port);

	// Envoi de donnees (d'un client vers un serveur) :
	// -data : pointeur vers les donnees
	// -nb_bytes : taille des donnees a envoyer, en octets
	int Send(const void* data, int nb_bytes);

	// Envoi de donnees (d'un serveur vers un client) :
	// -client_socket : pointeur vers la Socket correspondant au client a qui envoyer
	// les donnees (obtenu via Accept())
	// -data : pointeur vers les donnees
	// -nb_bytes : taille des donnees a envoyer, en octets
	int Send(Socket* client_socket, const void* data, int nb_bytes);

	// Reception de donnees (donnees allant d'un serveur vers un client) :
	// -buffer : pointeur vers l'endroit ou l'on doit stocker les donnees
	// -max_bytes : taille du buffer en octets, nombre maximal de donnees pouvant
	// etre retournees
	int Receive(void* buffer, int max_bytes);

	// Reception de donnees (donnees allant d'un client vers un serveur) :
	// -client_socket : socket du client qui nous envoie les donnees
	// -buffer : pointeur vers l'endroit ou l'on doit stocker les donnees
	// -max_bytes : taille du buffer en octets, nombre maximal de donnees pouvant
	// etre retournees
	int Receive(Socket* client_socket, void* buffer, int max_bytes);

	// Envoi d'un paquet (d'un client vers un serveur) :
	// -data : pointeur vers les donnees
	// -nb_bytes : taille des donnees a envoyer, en octets
	// A la difference de Send(), on rajoute un header de 4 octets indiquant la taille
	// du paquet. Un SendMsg() correspond a un ReceiveMsg().
	bool SendMsg(const void* data, int nb_bytes);

	// Envoi d'un paquet (d'un serveur vers un client) :
	// -client_socket : pointeur vers la Socket correspondant au client a qui envoyer
	// les donnees (obtenu via Accept())
	// -data : pointeur vers les donnees
	// -nb_bytes : taille des donnees a envoyer, en octets
	// A la difference de Send(), on rajoute un header de 4 octets indiquant la taille
	// du paquet. Un SendMsg() correspond a un ReceiveMsg().
	bool SendMsg(Socket* client_socket, const void* data, int nb_bytes);

	// Reception d'un paquet (donnees allant d'un serveur vers un client) :
	// -buffer : pointeur vers l'endroit ou l'on doit stocker les donnees
	// -max_bytes : taille du buffer en octets, nombre maximal de donnees pouvant
	// etre retournees
	// Correspond a un SendMsg()
	int ReceiveMsg(void* buffer, int max_bytes);

	// Reception d'un paquet (donnees allant d'un client vers un serveur) :
	// -client_socket : socket du client qui nous envoie les donnees
	// -buffer : pointeur vers l'endroit ou l'on doit stocker les donnees
	// -max_bytes : taille du buffer en octets, nombre maximal de donnees pouvant
	// etre retournees
	// Correspond a un SendMsg()
	int ReceiveMsg(Socket* client_socket, void* buffer, int max_bytes);

	// Pareil que ReceiveMsg mais ces fonctions allouent la memoire necessaire
	// pour le buffer de reception avec new[].
	int ReceiveNewMsg(void** buffer);
	int ReceiveNewMsg(Socket* client_socket, void** buffer);

	// Pour un client : renvoie true si le serveur nous a envoye un message, qui
	// est dans la file d'attente, sinon renvoie false.
	bool WaitsForAMessage();

	// Pour un serveur : renvoie true si le client specifie nous a envoye un message,
	// qui est donc dans la file d'attente, sinon renvoie false.
	// -client_socket : client susceptible de nous avoir envoye un message.
	bool WaitsForAMessageFrom(Socket* client_socket);

	// Mise sur ecoute (pour un serveur) :
	// -port : numero du port a ecouter
	// -max_queue : nombre maximum de clients pouvant attendre dans une file
	bool Listen(int port, int max_queue=10);

	// Acceptation d'un nouveau client (pour un serveur)
	// -sock_client : pointeur vers la socket correspondant au nouveau client accepte
	bool Accept(Socket* sock_client);

	// Ferme la connexion
	void ShutDown();
};

#endif // SOCKET_H
