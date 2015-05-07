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

TcpSock::TcpSock(C Str& host, uint16 port)
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

int TcpSock::readSome(void* buffer, int maxBytes)
{
   int rc = recv(_fd, buffer, maxBytes, 0);
   if (rc < 0)
      throw ErreurSocket("Recv Failed");
   return rc;
}

void TcpSock::write(const void* data, int cBytes)
{
   const byte* ptr = (const byte*)data;
   while (cBytes > 0)
   {
      int rc = ::send(_fd, ptr, cBytes, 0);
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

   int rc = bind(_fd, (struct sockaddr*)&addr, sizeof(struct sockaddr_in));
   if (rc < 0) {
      ::close(_fd);
		throw ErreurSocket("Bind Error (port déjà utilisé ?)");
	}

   if(listen(_fd, SOMAXCONN) < 0) {
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