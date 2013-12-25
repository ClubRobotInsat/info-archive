// Socket.cpp - classe representant une socket.
// 2005-2010 par Lionel Fuentes. Si vous utilisez cette classe dans un de vos projets, un mail
// de remerciement a funto66 at gmail.com ferait plaisir :)
// Version 1.2

#include "Socket.h"
#include <cstring>
#include <cstdio>
#include <cassert>

Socket::Socket() :
m_fd(0), m_state(SOCK_FREE), m_protocol(SOCK_TCP), m_addr() {
}

Socket::Socket(const Socket& ref) : m_fd(ref.m_fd), m_state(ref.m_state), m_protocol(ref.m_protocol),
m_addr(ref.m_addr) {
}

Socket::~Socket() {
	// On n'appelle pas Destroy() dans le destructeur : une Socket doit etre detruite
	// en appelant Destroy() directement.
}

Socket& Socket::operator=(const Socket& ref) {
	m_fd = ref.m_fd;
	m_state = ref.m_state;
	m_protocol = ref.m_protocol;
	memcpy((void*)&m_addr, (const void*)&ref.m_addr, sizeof(struct sockaddr_in));
	
	return *this;
}

bool Socket::Create(SockProtocol protocol) {
	Destroy(); // Si la socket etait deja creee, on la detruit avant.
	
	if(protocol != SOCK_TCP) {
		fprintf(stderr, "Erreur : protocole non implemente\n");
		return false;
	}
	
	m_fd = socket(AF_INET, SOCK_STREAM, 0); // AF_INET : internet; SOCK_STREAM : par flux; 0 : protocol (TCP)
	
#ifdef WIN32
	if(m_fd == INVALID_SOCKET)
#else
		if(m_fd < 0)
#endif
		{
			return false;
		}
	
	return true;
}

void Socket::Destroy() {
#ifdef WIN32
	if(m_fd == INVALID_SOCKET || m_fd == 0) return;
#else
	if(m_fd <= 0) return;
#endif
	
	ShutDown();
#ifdef WIN32
	closesocket(m_fd);
#else
	shutdown(m_fd, 2);
	close(m_fd);
#endif
	
	m_fd = 0;
	m_state = SOCK_FREE;
	m_protocol = SOCK_TCP;
}

bool Socket::Init() {
#ifdef WIN32
	WSAData data;
	int error_code = WSAStartup(MAKEWORD(1, 1), &data);
	if(error_code != 0) {
		fprintf(stderr, "Erreur lors de l'initialisation des"
			" Windows Sockets : %d\n", error_code);
		return false;
	}
#endif
	return true;
}

void Socket::CleanUp() {
#ifdef WIN32
	WSACleanup();
#endif
}

// Connexion au serveur
bool Socket::Connect(const char* server_name, int port) {
#ifdef WIN32
	if(m_fd == INVALID_SOCKET || m_fd == 0) return false;
#else
	if(m_fd <= 0) return false;
#endif
	
	hostent* hostinfo;
	// Recupere l'adresse ip correspond a l'adresse du serveur
	hostinfo = gethostbyname (server_name);
	// Impossible de recuperer l'addresse IP <=> impossible de se connecter, on sort
	if(hostinfo == nullptr)
		return false;
	
	
	// On remplit la structure m_addr
	m_addr.sin_family = AF_INET; // Adresse de type internet : on doit toujours mettre ca
	m_addr.sin_port = htons(port); // Port
	m_addr.sin_addr = *(struct in_addr*) hostinfo->h_addr; // Adresse IP du serveur
	memset(&(m_addr.sin_zero), 0, 8); // On met le reste (8 octets) a 0
	
	if(connect(m_fd, (struct sockaddr*)&m_addr, sizeof(struct sockaddr_in)) < 0)
		m_state = SOCK_FREE;
	else
		m_state = SOCK_CONNECTED;
	
	return m_state == SOCK_CONNECTED;
}

// Envoi de donnees (d'un client vers un serveur) :
int Socket::Send(const void* data, int nb_bytes) {
	if(m_state != SOCK_CONNECTED) return -1;
	
#ifdef WIN32
	return send(m_fd, (const char*)data, nb_bytes, 0);
#elif __APPLE__
	return send(m_fd, data, nb_bytes, SO_NOSIGPIPE);
#else
	// NB : MSG_NOSIGNAL ne vaut que pour Linux, pas meme pour MacOS X !
	return send(m_fd, data, nb_bytes, MSG_NOSIGNAL);
#endif
}

// Envoi de donnees (d'un serveur vers un client) :
int Socket::Send(Socket* client_socket, const void* data, int nb_bytes) {
	if(m_state != SOCK_LISTENING) return -1;
	
#ifdef WIN32
	return send(client_socket->m_fd, (const char*)data, nb_bytes, 0);
#elif __APPLE__
	// NB : SO_NOSIGPIPE pour MacOS X !
	return send(client_socket->m_fd, data, nb_bytes, SO_NOSIGPIPE);
#else
	// NB : MSG_NOSIGNAL ne vaut que pour Linux, pas meme pour MacOS X !
	return send(client_socket->m_fd, data, nb_bytes, MSG_NOSIGNAL);
#endif
}

// Reception de donnees (donnees allant d'un serveur vers un client) :
int Socket::Receive(void* buffer, int max_bytes) {
	if(m_state != SOCK_CONNECTED) return -1;
	
#ifdef WIN32
	return recv(m_fd, (char*)buffer, max_bytes, 0);
#else
	return recv(m_fd, buffer, max_bytes, 0);
#endif
}

// Reception de donnees (donnees allant d'un client vers un serveur) :
int Socket::Receive(Socket* client_socket, void* buffer, int max_bytes) {
	if(m_state != SOCK_LISTENING) return -1;
	
#ifdef WIN32
	return recv(client_socket->m_fd, (char*)buffer, max_bytes, 0);
#else
	return recv(client_socket->m_fd, buffer, max_bytes, 0);
#endif
}

// Envoi d'un paquet (d'un client vers un serveur)
// A la difference de Send(), on rajoute un header de 4 octets indiquant la taille
// du paquet. Un SendMsg() correspond a un ReceiveMsg().
bool Socket::SendMsg(const void* data, int nb_bytes) {
	unsigned char header[4] = {
		(unsigned char) ((nb_bytes >> 0*8) & 0xFF),
		(unsigned char) ((nb_bytes >> 1*8) & 0xFF),
		(unsigned char) ((nb_bytes >> 2*8) & 0xFF),
		(unsigned char) ((nb_bytes >> 3*8) & 0xFF)};
	
	int remaining = nb_bytes;
	
	if(Send((const void*)header, 4) <= 0) return false;
	
	while(remaining != 0)
		remaining = remaining - Send((void*)(&((unsigned char*)data)[nb_bytes-remaining]), remaining);
	
	return true;
}

// Envoi d'un paquet (d'un serveur vers un client)
// A la difference de Send(), on rajoute un header de 4 octets indiquant la taille
// du paquet. Un SendMsg() correspond a un ReceiveMsg().
bool Socket::SendMsg(Socket* client_socket, const void* data, int nb_bytes) {
	unsigned char header[4] = {
		(unsigned char) ((nb_bytes >> 0*8) & 0xFF),
		(unsigned char) ((nb_bytes >> 1*8) & 0xFF),
		(unsigned char) ((nb_bytes >> 2*8) & 0xFF),
		(unsigned char) ((nb_bytes >> 3*8) & 0xFF)};
	
	int remaining = nb_bytes;
	
	int n = 0;
	if((n = Send(client_socket, (const void*)header, 4)) <= 0)
		return false;
	
	while(remaining != 0) {
		n = Send(client_socket, (void*)(&((unsigned char*)data)[nb_bytes-remaining]), remaining);
		if(n <= 0)
			return false;
		
		remaining = remaining - n;
	}
	
	return true;
}

// Reception d'un paquet (donnees allant d'un serveur vers un client)
// Correspond a un SendMsg()
int Socket::ReceiveMsg(void* buffer, int max_bytes) {
	unsigned char header[4];
	int nb_bytes=0; // Nombre d'octets dans le paquet
	int bytes_to_read; // Nombre d'octets a lire effectivement
	int remaining;
	unsigned char* discarded_bytes=nullptr; // Buffer contenant les donnees du paquet non lues dans buffer
	// lorsque la taille de buffer est trop petite pour contenir tout le paquet
	
	int received = Receive((void*)header, 4); // Lecture du header
	
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
		remaining = remaining - Receive((void*)(&((unsigned char*)buffer)[bytes_to_read-remaining]), remaining);
	
	// Si une partie du paquet ne doit pas etre lue dans buffer (max_bytes trop petit), on la lit dans
	// discarded_bytes et on les detruit.
	if(nb_bytes > max_bytes) {
		remaining = nb_bytes - bytes_to_read;
		bytes_to_read = remaining;
		discarded_bytes = new unsigned char[bytes_to_read * sizeof(unsigned char)];
		while(remaining != 0)
			remaining = remaining - Receive((void*)(&((unsigned char*)discarded_bytes)[bytes_to_read-remaining]), remaining);
		delete [] discarded_bytes;
	}
	
	return nb_bytes > max_bytes ? max_bytes : nb_bytes;
}

// Reception d'un paquet (donnees allant d'un client vers un serveur)
// Correspond a un Sendmsg()
int Socket::ReceiveMsg(Socket* client_socket, void* buffer, int max_bytes) {
	unsigned char header[4];
	int nb_bytes=0; // Nombre d'octets dans le paquet
	int bytes_to_read; // Nombre d'octets a lire effectivement
	int remaining;
	unsigned char* discarded_bytes=nullptr; // Buffer contenant les donnees du paquet non lues dans buffer
	// lorsque la taille de buffer est trop petite pour contenir tout le paquet
	
	int received = Receive(client_socket, (void*)header, 4); // Lecture du header
	
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
		remaining = remaining - Receive(client_socket, (void*)(&((unsigned char*)buffer)[bytes_to_read-remaining]), remaining);
	
	// Si une partie du paquet ne doit pas etre lue dans buffer (max_bytes trop petit), on la lit dans
	// discarded_bytes et on les detruit.
	if(nb_bytes > max_bytes) {
		remaining = nb_bytes - bytes_to_read;
		bytes_to_read = remaining;
		discarded_bytes = new unsigned char[bytes_to_read * sizeof(unsigned char)];
		while(remaining != 0)
			remaining = remaining - Receive(client_socket, (void*)(&((unsigned char*)discarded_bytes)[bytes_to_read-remaining]), remaining);
		delete [] discarded_bytes;
	}
	
	return nb_bytes > max_bytes ? max_bytes : nb_bytes;
}

// Pareil que ReceiveMsg mais alloue la memoire necessaire
// pour le buffer de reception avec new[].
int Socket::ReceiveNewMsg(void** buffer) {
	unsigned char header[4];
	int nb_bytes=0; // Nombre d'octets dans le paquet
	int remaining;
	unsigned char* ptr = nullptr;
	
	assert(buffer != nullptr);
	
	int received = Receive((void*)header, 4); // Lecture du header
	
	// Cas ou l'on n'a pas recu assez d'octets pour avoir un header complet :
	if(received < 4)
		return -1;
	
	nb_bytes = ((int)(header[0]) << 0*8) +
	((int)(header[1]) << 1*8) +
	((int)(header[2]) << 2*8) +
	((int)(header[3]) << 3*8);
	
	// On alloue de la memoire pour les donnees :
	ptr = new unsigned char[nb_bytes];
	
	// On lit les octets qui doivent etre lus :
	remaining = nb_bytes;
	while(remaining != 0)
		remaining = remaining - Receive((void*)(&ptr[nb_bytes-remaining]), remaining);
	
	// On renvoie l'adresse et la taille du nouveau buffer
	*buffer = ptr;
	
	return nb_bytes;
}


// Pareil que ReceiveMsg mais alloue la memoire necessaire
// pour le buffer de reception avec new[].
int Socket::ReceiveNewMsg(Socket* client_socket, void** buffer) {
	unsigned char header[4];
	int nb_bytes=0; // Nombre d'octets dans le paquet
	int remaining;
	unsigned char* ptr = nullptr;
	
	assert(buffer != nullptr);
	
	int received = Receive(client_socket, (void*)header, 4); // Lecture du header
	
	// Cas ou l'on n'a pas recu assez d'octets pour avoir un header complet :
	if(received < 4)
		return -1;
	
	nb_bytes = ((int)(header[0]) << 0*8) +
	((int)(header[1]) << 1*8) +
	((int)(header[2]) << 2*8) +
	((int)(header[3]) << 3*8);
	
	// On alloue de la memoire pour les donnees :
	ptr = new unsigned char[nb_bytes];
	
	// On lit les octets qui doivent etre lus :
	remaining = nb_bytes;
	while(remaining != 0)
		remaining = remaining - Receive(client_socket, (void*)(&ptr[nb_bytes-remaining]), remaining);
	
	// On renvoie l'adresse et la taille du nouveau buffer
	*buffer = ptr;
	
	return nb_bytes;
}

// Pour un client : renvoie true si le serveur nous a envoye un message, qui
// est dans la file d'attente, sinon renvoie false.
bool Socket::WaitsForAMessage() {
	// On cree un fd_set, c'est a dire une liste des FDs a tester. On ne met en fait
	// dans cette liste que notre FD.
	fd_set set;
	
	FD_ZERO(&set);
	FD_SET(m_fd, &set);
	
	// Le timeout du fd_set que l'on a cree est de 0s et 0 micro-seconde, ce qui signifie que
	// select() rendra la main immediatement.
	struct timeval timeout = {0, 0};
	
	// On fait un select(), qui permet de savoir si les FDs du fd_set ont un message
	// da la file d'attente.
	if(select(m_fd+1, &set, nullptr, nullptr, &timeout) < 0) {
		fprintf(stderr, "Erreur : select() renvoie -1\n");
		return false;
	}
	
	return (FD_ISSET(m_fd, &set) != 0);
}

// Pour un serveur : renvoie true si le client specifie nous a envoye un message,
// qui est donc dans la file d'attente, sinon renvoie false.
bool Socket::WaitsForAMessageFrom(Socket* client_socket) {
	// Pour les explications, Cf au-dessus
	fd_set set;
	
	FD_ZERO(&set);
	FD_SET(client_socket->m_fd, &set);
	
	struct timeval timeout = {0, 0};
	
	if(select(client_socket->m_fd+1, &set, nullptr, nullptr, &timeout) < 0) {
		fprintf(stderr, "Erreur : select() renvoie -1\n");
		return false;
	}
	
	return (FD_ISSET(client_socket->m_fd, &set) != 0);
}

// Mise sur ecoute (pour un serveur) :
bool Socket::Listen(int port, int max_queue) {
	// On remplit la structure m_addr, qui correspond a l'adresse du serveur (nous)
	m_addr.sin_family = AF_INET; // Adresse de type internet : on doit toujours mettre ca
	m_addr.sin_port = htons(port); // Port
	m_addr.sin_addr.s_addr = htonl(INADDR_ANY); // On ecoute tout le monde, quelle que soit son adresse IP
	memset(&(m_addr.sin_zero), 0, 8); // On met le reste (8 octets) a 0
	
	// On associe la socket a un port et a une adresse, definis dans server_addr :
	if(bind(m_fd, (struct sockaddr*)&m_addr, sizeof(struct sockaddr_in)) < 0) {
		fprintf(stderr, "Erreur lors de l'association de la socket avec l'adresse\n");
		m_state = SOCK_FREE;
	}
	else {
		// On se met sur ecoute
		if(listen(m_fd, max_queue) < 0) {
			fprintf(stderr, "Erreur lors de la mise sur ecoute\n");
			m_state = SOCK_FREE;
		}
		else
			m_state = SOCK_LISTENING;
	}
	
	return m_state == SOCK_LISTENING;
}

// Acceptation d'un nouveau client (pour un serveur)
bool Socket::Accept(Socket* sock_client) {
	// On remplit la structure m_addr, qui correspond a l'adresse du serveur (nous)
	sock_client->m_addr.sin_family = AF_INET; // Adresse de type internet : on doit toujours mettre ca
	sock_client->m_addr.sin_port = 0; // Port
	sock_client->m_addr.sin_addr.s_addr = htons(INADDR_ANY); // On ecoute tout le monde, quelle que soit son adresse IP
	memset(&(sock_client->m_addr.sin_zero), 0, 8); // On met le reste (8 octets) a 0
	
#ifdef WIN32
	int sin_size = sizeof(struct sockaddr_in);
#else
	socklen_t sin_size = sizeof(struct sockaddr_in);
#endif
	sock_client->m_fd = accept(m_fd, (struct sockaddr*)&(sock_client->m_addr), &sin_size);
	
#ifdef WIN32
	if(sock_client->m_fd != INVALID_SOCKET)
#else
		if(sock_client->m_fd != -1)
#endif
		{
			sock_client->m_state = SOCK_ACCEPTED;
			return true;
		}
		else {
			return false;
		}
}

// Ferme la connexion
void Socket::ShutDown() {
	if(m_state != SOCK_FREE) {
		shutdown(m_fd, 2);
		m_state = SOCK_FREE;
	}
}
