// Socket.cpp - classe representant une socket.
// 2005-2010 par Lionel Fuentes. Si vous utilisez cette classe dans un de vos projets, un mail
// de remerciement a funto66 at gmail.com ferait plaisir :)
// Version 1.2

#include "Socket.h"
#include <cstring>
#include <cstdio>
#include <cassert>
#include <stdexcept>
#include <memory>

Socket::Socket(SockProtocol protocol) : _protocol(protocol) {
	if(protocol != SOCK_TCP) {
		fprintf(stderr, "Erreur : protocole non implemente\n");
		throw std::logic_error("Erreur : protocole non implemente\n");
	}

	_fd = socket(AF_INET, SOCK_STREAM, 0); // AF_INET : internet; SOCK_STREAM : par flux; 0 : protocol (TCP)

	if(
#ifdef WIN32
	   _fd == INVALID_SOCKET
#else
	   _fd < 0
#endif
	   ) {
		throw std::runtime_error("Impossible de créer le socket !");
	}
}

Socket::~Socket() {
#ifdef WIN32
	if(_fd == INVALID_SOCKET || _fd == 0) return;
#else
	if(_fd <= 0) return;
#endif

	shutDown();
#ifdef WIN32
	closesocket(_fd);
#else
	close(_fd);
#endif
}

// Connexion au serveur
bool Socket::connect(const char* server_name, std::uint16_t port) {
#ifdef WIN32
	if(_fd == INVALID_SOCKET || _fd == 0) return false;
#else
	if(_fd <= 0) return false;
#endif

	hostent* hostinfo;
	// Recupere l'adresse ip correspond a l'adresse du serveur
	hostinfo = gethostbyname(server_name);
	// Impossible de recuperer l'addresse IP <=> impossible de se connecter, on sort
	if(hostinfo == nullptr)
		return false;


	// On remplit la structure _addr
	_addr.sin_family = AF_INET; // Adresse de type internet : on doit toujours mettre ca
	_addr.sin_port = htons(port); // Port
	_addr.sin_addr = *(struct in_addr*) hostinfo->h_addr; // Adresse IP du serveur
	memset(&(_addr.sin_zero), 0, 8); // On met le reste (8 octets) a 0

	if(::connect(_fd, (struct sockaddr*)&_addr, sizeof(struct sockaddr_in)) < 0)
		_state = SOCK_FREE;
	else
		_state = SOCK_CONNECTED;

	return _state == SOCK_CONNECTED;
}

// Envoi de donnees (d'un client vers un serveur) :
ssize_t Socket::send(const void* data, std::size_t nb_bytes) {
	if(_state != SOCK_CONNECTED) return -1;

#ifdef WIN32
	return ::send(_fd, (const char*)data, nb_bytes, 0);
#elif __APPLE__
	return ::send(_fd, data, nb_bytes, SO_NOSIGPIPE);
#else
	// NB : MSG_NOSIGNAL ne vaut que pour Linux, pas meme pour MacOS X !
	return ::send(_fd, data, nb_bytes, MSG_NOSIGNAL);
#endif
}

// Envoi de donnees (d'un serveur vers un client) :
ssize_t Socket::send(Socket &client_socket, const void* data, size_t nb_bytes) {
	if(_state != SOCK_LISTENING) return -1;

#ifdef WIN32
	return ::send(client_socket._fd, (const char*)data, nb_bytes, 0);
#elif __APPLE__
	// NB : SO_NOSIGPIPE pour MacOS X !
	return ::send(client_socket._fd, data, nb_bytes, SO_NOSIGPIPE);
#else
	// NB : MSG_NOSIGNAL ne vaut que pour Linux, pas meme pour MacOS X !
	return ::send(client_socket._fd, data, nb_bytes, MSG_NOSIGNAL);
#endif
}

// Reception de donnees (donnees allant d'un serveur vers un client) :
ssize_t Socket::receive(void* buffer, size_t max_bytes) {
	if(_state != SOCK_CONNECTED) return -1;

#ifdef WIN32
	return recv(_fd, (char*)buffer, max_bytes, 0);
#else
	return recv(_fd, buffer, max_bytes, 0);
#endif
}

// Reception de donnees (donnees allant d'un client vers un serveur) :
ssize_t Socket::receive(Socket &client_socket, void* buffer, size_t max_bytes) {
	if(_state != SOCK_LISTENING) return -1;

#ifdef WIN32
	return recv(client_socket._fd, (char*)buffer, max_bytes, 0);
#else
	return recv(client_socket._fd, buffer, max_bytes, 0);
#endif
}

// Envoi d'un paquet (d'un client vers un serveur)
// A la difference de Send(), on rajoute un header de 4 octets indiquant la taille
// du paquet. Un SendMsg() correspond a un ReceiveMsg().
bool Socket::sendMsg(const void* data, size_t nb_bytes) {
	std::uint8_t header[4] = {
		std::uint8_t((nb_bytes >> 0*8) & 0xFF),
		std::uint8_t((nb_bytes >> 1*8) & 0xFF),
		std::uint8_t((nb_bytes >> 2*8) & 0xFF),
		std::uint8_t((nb_bytes >> 3*8) & 0xFF)};

	size_t remaining = nb_bytes;

	if(send((const void*)header, 4) <= 0) return false;

	while(remaining != 0)
		remaining = remaining - send((void*)(&((std::uint8_t *)data)[nb_bytes-remaining]), remaining);

	return true;
}

// Envoi d'un paquet (d'un serveur vers un client)
// A la difference de Send(), on rajoute un header de 4 octets indiquant la taille
// du paquet. Un SendMsg() correspond a un ReceiveMsg().
bool Socket::sendMsg(Socket &client_socket, const void* data, size_t nb_bytes) {
	std::uint8_t header[4] = {
		std::uint8_t((nb_bytes >> 0*8) & 0xFF),
		std::uint8_t((nb_bytes >> 1*8) & 0xFF),
		std::uint8_t((nb_bytes >> 2*8) & 0xFF),
		std::uint8_t((nb_bytes >> 3*8) & 0xFF)};

	ssize_t remaining = nb_bytes;

	ssize_t n = 0;
	if((n = this->send(client_socket, (const void*)header, 4)) <= 0)
		return false;

	while(remaining != 0) {
		n = this->send(client_socket, (void*)(&((std::uint8_t *)data)[nb_bytes-remaining]), remaining);
		if(n <= 0)
			return false;

		remaining = remaining - n;
	}

	return true;
}

// Reception d'un paquet (donnees allant d'un serveur vers un client)
// Correspond a un SendMsg()
ssize_t Socket::receiveMsg(void* buffer, size_t max_bytes) {
	std::uint8_t header[4];
	size_t nb_bytes=0; // Nombre d'octets dans le paquet
	size_t bytes_to_read; // Nombre d'octets à lire effectivement
	size_t remaining;
	// lorsque la taille de buffer est trop petite pour contenir tout le paquet

	ssize_t received = this->receive((void*)header, 4); // Lecture du header

	// Cas ou l'on n'a pas recu assez d'octets pour avoir un header complet :
	if(received < 4)
		return -1;

	nb_bytes = ((int)(header[0]) << 0*8) +
	((int)(header[1]) << 1*8) +
	((int)(header[2]) << 2*8) +
	((int)(header[3]) << 3*8);

	// On calcule le nombre d'octets a lire
	remaining = bytes_to_read = nb_bytes > max_bytes ? max_bytes : nb_bytes;

	// On lit les octets qui doivent etre lus dans buffer
	while(remaining != 0)
		remaining = remaining - this->receive((void*)(&((std::uint8_t *)buffer)[bytes_to_read-remaining]), remaining);

	// Si une partie du paquet ne doit pas etre lue dans buffer (max_bytes trop petit), on la lit dans
	// discarded_bytes et on les detruit.
	if(nb_bytes > max_bytes) {
		remaining = nb_bytes - bytes_to_read;
		bytes_to_read = remaining;
		auto discarded_bytes = std::make_unique<std::uint8_t[]>(bytes_to_read);
		while(remaining != 0)
			remaining = remaining - this->receive((void*)(&(discarded_bytes[bytes_to_read-remaining])), remaining);
	}

	return nb_bytes > max_bytes ? max_bytes : nb_bytes;
}

// Reception d'un paquet (donnees allant d'un client vers un serveur)
// Correspond a un Sendmsg()
ssize_t Socket::receiveMsg(Socket &client_socket, void* buffer, size_t max_bytes) {
	std::uint8_t header[4];
	size_t nb_bytes=0; // Nombre d'octets dans le paquet
	size_t bytes_to_read; // Nombre d'octets a lire effectivement
	size_t remaining;
	// lorsque la taille de buffer est trop petite pour contenir tout le paquet

	ssize_t received = receive(client_socket, (void*)header, 4); // Lecture du header

	// Cas ou l'on n'a pas recu assez d'octets pour avoir un header complet :
	if(received < 4)
		return -1;

	nb_bytes = ((int)(header[0]) << 0*8) +
	((int)(header[1]) << 1*8) +
	((int)(header[2]) << 2*8) +
	((int)(header[3]) << 3*8);

	// On calcule le nombre d'octets a lire
	remaining = bytes_to_read = nb_bytes > max_bytes ? max_bytes : nb_bytes;

	// On lit les octets qui doivent etre lus dans buffer
	while(remaining != 0)
		remaining = remaining - receive(client_socket, (void*)(&((std::uint8_t *)buffer)[bytes_to_read-remaining]), remaining);

	// Si une partie du paquet ne doit pas etre lue dans buffer (max_bytes trop petit), on la lit dans
	// discarded_bytes et on les detruit.
	if(nb_bytes > max_bytes) {
		remaining = nb_bytes - bytes_to_read;
		bytes_to_read = remaining;
		auto discarded_bytes = std::make_unique<std::uint8_t[]>(bytes_to_read);
		while(remaining != 0)
			remaining = remaining - this->receive(client_socket, (void*)(&(discarded_bytes[bytes_to_read-remaining])), remaining);
	}

	return nb_bytes > max_bytes ? max_bytes : nb_bytes;
}

// Pareil que ReceiveMsg mais alloue la memoire necessaire
std::vector<std::uint8_t> Socket::receiveNewMsg() {
	std::uint8_t header[4];
	size_t nb_bytes=0; // Nombre d'octets dans le paquet
	size_t remaining;

	ssize_t received = this->receive((void*)header, 4); // Lecture du header

	// Cas ou l'on n'a pas recu assez d'octets pour avoir un header complet :
	if(received < 4)
		return {};

	nb_bytes = ((int)(header[0]) << 0*8) +
	((int)(header[1]) << 1*8) +
	((int)(header[2]) << 2*8) +
	((int)(header[3]) << 3*8);

	// On alloue de la memoire pour les donnees :
	std::vector<std::uint8_t> data(nb_bytes);

	// On lit les octets qui doivent etre lus :
	remaining = nb_bytes;
	while(remaining != 0)
		remaining = remaining - this->receive((void*)(&data[nb_bytes-remaining]), remaining);

	return data;
}


// Pareil que ReceiveMsg mais alloue la memoire necessaire
std::vector<std::uint8_t> Socket::receiveNewMsg(Socket &client_socket) {
	std::uint8_t header[4];
	size_t nb_bytes=0; // Nombre d'octets dans le paquet
	size_t remaining;

	size_t received = this->receive(client_socket, (void*)header, 4); // Lecture du header

	// Cas ou l'on n'a pas recu assez d'octets pour avoir un header complet :
	if(received < 4)
		return {};

	nb_bytes = ((int)(header[0]) << 0*8) +
	((int)(header[1]) << 1*8) +
	((int)(header[2]) << 2*8) +
	((int)(header[3]) << 3*8);

	// On alloue de la memoire pour les donnees :
	std::vector<std::uint8_t> data(nb_bytes);

	// On lit les octets qui doivent etre lus :
	remaining = nb_bytes;
	while(remaining != 0)
		remaining = remaining - this->receive(client_socket, (void*)(&data[nb_bytes-remaining]), remaining);

	return data;
}

// Pour un client : renvoie true si le serveur nous a envoye un message, qui
// est dans la file d'attente, sinon renvoie false.
bool Socket::waitsForAMessage() {
	// On cree un fd_set, c'est a dire une liste des FDs a tester. On ne met en fait
	// dans cette liste que notre FD.
	fd_set set;

	FD_ZERO(&set);
	FD_SET(_fd, &set);

	// Le timeout du fd_set que l'on a cree est de 0s et 0 micro-seconde, ce qui signifie que
	// select() rendra la main immediatement.
	struct timeval timeout = {0, 0};

	// On fait un select(), qui permet de savoir si les FDs du fd_set ont un message
	// da la file d'attente.
	if(select(_fd+1, &set, nullptr, nullptr, &timeout) < 0) {
		fprintf(stderr, "Erreur : select() renvoie -1\n");
		return false;
	}

	return (FD_ISSET(_fd, &set) != 0);
}

// Pour un serveur : renvoie true si le client specifie nous a envoye un message,
// qui est donc dans la file d'attente, sinon renvoie false.
bool Socket::waitsForAMessageFrom(Socket &client_socket) {
	// Pour les explications, Cf au-dessus
	fd_set set;

	FD_ZERO(&set);
	FD_SET(client_socket._fd, &set);

	struct timeval timeout = {0, 0};

	if(select(client_socket._fd+1, &set, nullptr, nullptr, &timeout) < 0) {
		fprintf(stderr, "Erreur : select() renvoie -1\n");
		return false;
	}

	return (FD_ISSET(client_socket._fd, &set) != 0);
}

// Mise sur ecoute (pour un serveur) :
bool Socket::listen(std::uint16_t port, int max_queue) {
	// On remplit la structure _addr, qui correspond a l'adresse du serveur (nous)
	_addr.sin_family = AF_INET; // Adresse de type internet : on doit toujours mettre ca
	_addr.sin_port = htons(port); // Port
	_addr.sin_addr.s_addr = htonl(INADDR_ANY); // On ecoute tout le monde, quelle que soit son adresse IP
	memset(&(_addr.sin_zero), 0, sizeof(_addr.sin_zero)); // On met le reste (8 octets) a 0

	// On associe la socket a un port et a une adresse, definis dans server_addr :
	if(bind(_fd, (struct sockaddr*)&_addr, sizeof(struct sockaddr_in)) < 0) {
		fprintf(stderr, "Erreur lors de l'association de la socket avec l'adresse\n");
		_state = SOCK_FREE;
	}
	else {
		// On se met sur ecoute
		if(::listen(_fd, max_queue) < 0) {
			fprintf(stderr, "Erreur lors de la mise sur ecoute\n");
			_state = SOCK_FREE;
		}
		else
			_state = SOCK_LISTENING;
	}

	return _state == SOCK_LISTENING;
}

// Acceptation d'un nouveau client (pour un serveur)
bool Socket::accept(Socket &sock_client) {
	// On remplit la structure _addr, qui correspond a l'adresse du serveur (nous)
	sock_client._addr.sin_family = AF_INET; // Adresse de type internet : on doit toujours mettre ca
	sock_client._addr.sin_port = 0; // Port
	sock_client._addr.sin_addr.s_addr = htons(INADDR_ANY); // On ecoute tout le monde, quelle que soit son adresse IP
	memset(&(sock_client._addr.sin_zero), 0, 8); // On met le reste (8 octets) a 0

#ifdef WIN32
	int sin_size = sizeof(struct sockaddr_in);
#else
	socklen_t sin_size = sizeof(struct sockaddr_in);
#endif
	sock_client._fd = ::accept(_fd, (struct sockaddr*)&(sock_client._addr), &sin_size);

#ifdef WIN32
	if(sock_client->_fd != INVALID_SOCKET)
#else
		if(sock_client._fd != -1)
#endif
		{
			sock_client._state = SOCK_ACCEPTED;
			return true;
		}
		else {
			return false;
		}
}

// Ferme la connexion
void Socket::shutDown() {
	if(_state != SOCK_FREE) {
		shutdown(_fd, 2);
		_state = SOCK_FREE;
	}
}

// Gère la compatibilité avec l'API WinSock
#ifdef WIN32

namespace {
	struct WinSockInitializer {
		WinSockInitializer() {
			WSAData data;
			int error_code = WSAStartup(MAKEWORD(1, 1), &data);
			if(error_code != 0) {
				fprintf(stderr, "Erreur lors de l'initialisation des"
						" Windows Sockets : %d\n", error_code);
			}
		}

		~WinSockInitializer() {
			WSACleanup();
		}
	};

	// Chargé automatiquement à la création du programme, détruit automatiquement à la fin du programme.
	WinSockInitializer const winSockInitialize;
}

#endif
