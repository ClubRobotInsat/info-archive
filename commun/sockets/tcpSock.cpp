/******************************************************************************/
/* info/commun/tcpSock.h                                           2015-05-17 */
/* Club Robot INSA Toulouse                                      Félix Poisot */
/******************************************************************************/
#include "tcpSock.h"
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
/******************************************************************************/

using namespace std;

//-- Socket (point-à-point) --//

TcpSock::TcpSock(const Str& host, uint16 port)
{
	_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_fd < 0)
		throw ErreurSocket("Impossible de créer le socket !");

   hostent* hostinfo = gethostbyname(host.c_str());
   if( hostinfo == nullptr) {
      ::close(_fd);
      throw ErreurSocket("nom d'hote non résolu");
   }

   sockaddr_in addr = {};
   addr.sin_family = AF_INET;
   addr.sin_port = htons(port);
   addr.sin_addr = *(struct in_addr*) hostinfo->h_addr; // Adresse IP du serveur

   int rc = connect(_fd, (struct sockaddr*)&addr, sizeof(struct sockaddr_in));
   if (rc < 0)
   {
      ::close(_fd);
      throw ErreurSocket("Connection impossible");
   }
}

void TcpSock::read(void* buffer, int cBytes)
{
   byte* ptr = (byte*)buffer;
   while (cBytes > 0)
   {
      int rc = recv(_fd, ptr, cBytes, 0);
      if (rc <= 0)
         throw ErreurSocket("Recv Failed");
      cBytes -= rc;
      ptr += rc;
   }
}

Str TcpSock::readTextTo(char chr)
{
   // buffering peut-être pas optimisé, mais ça marche et j'ai la flemme.
   std::vector<char> buff;
   char c;
   do {
      read(&c, 1);
      buff.push_back(c);
   } while (c != chr);
   return Str(buff.data());
}

int TcpSock::readSome(void* buffer, int maxBytes)
{
   int rc = recv(_fd, buffer, maxBytes, 0);
   if (rc < 0)
      throw ErreurSocket("Recv Failed");
   return rc;
}

bool TcpSock::hasNext()
{
   timeval zero = {};
   fd_set fs = {};
   FD_ZERO(&fs);
   FD_SET(_fd, &fs);
   if (select(_fd+1, &fs, nullptr, nullptr, &zero) < 0)
      throw ErreurSocket("select() panic");
   return FD_ISSET(_fd, &fs);
}

void TcpSock::write(const void* data, int cBytes)
{
   const byte* ptr = (const byte*)data;
   while (cBytes > 0)
   {
      // Flag MSG_NOSIGNAL: les erreurs vont arriver dans rc au lieu de kill
      // le processus
#ifdef __APPLE__
      int rc = ::send(_fd, ptr, cBytes, SO_NOSIGPIPE);
#else
      int rc = ::send(_fd, ptr, cBytes, MSG_NOSIGNAL);
#endif
      if (rc <= 0)
         throw ErreurSocket("Send Failed");
      cBytes -= rc;
      ptr += rc;
   }
}

void TcpSock::close()
{
   if (_fd >= 0) {
      shutdown(_fd, SHUT_RDWR);
      ::close(_fd);
   }
   _fd = -1;
}

//-- ServSocket (serveur) --//

TcpServSock::TcpServSock(uint16 port)
{
   _fd = socket(AF_INET, SOCK_STREAM, 0);
   if (_fd < 0)
      throw ErreurSocket("Impossible de créer le socket !");

	sockaddr_in addr = {};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

   // Reprends le socket de force (ignore la protection TCP bloquant le port
   // pour 30s après sa dernière fermeture, des fois que des paquets soient
   // encore en vol.)
   // Ne permet PAS de prendre un socket toujours utilisé par un serveur vivant,
   // contrairement à ce que le nom peut suggérer.
   int optval = 1;
   setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

	int rc = ::bind(_fd, (struct sockaddr*)&addr, sizeof(struct sockaddr_in));
	if (rc < 0) {
      ::close(_fd);
		throw ErreurSocket("Bind Error (port déjà utilisé ?)");
	}

   if (listen(_fd, SOMAXCONN) < 0) {
	   ::close(_fd);
      throw ErreurSocket("Listen Error");
   }
}

// Acceptation d'un nouveau client (pour un serveur)
unique_ptr<TcpSock> TcpServSock::accept()
{
	sockaddr_in who = {};
   uint sWho;
	int sock = ::accept(_fd, (struct sockaddr*)&who, &sWho);

   if (sock < 0)
      throw ErreurSocket("Accept Error");

   return unique_ptr<TcpSock>(new TcpSock(sock));
}


void TcpServSock::close()
{
   if (_fd >= 0) {
      ::close(_fd);
   }
   _fd = -1;
}