#pragma once
/******************************************************************************/
/* info/commun/tcpSock.h                                           2015-05-17 */
/* Club Robot INSA Toulouse                                      Félix Poisot */
/******************************************************************************/
#include "../Erreur.h"
#include "Utils.h"
#include <memory>
/******************************************************************************/

EXCEPTION_CLASS(ErreurSocket);

// Version plus saine de Socket
class TcpSock
{
public:
   // Tente une connection à un serveur. Exception si échec
   TcpSock(C Str& host, uint16 port);
	// close() implicite si nécessaire.
   ~TcpSock()   {close();}

   // Accumule 'cBytes' octets quoiqui'il arrive.
   // Exception si pas assez de données disponibles.
	void read(void* buffer, int cBytes);

	// Retourne dès que des données sont disponibles.
	// Renvoie le nb. d'octets lus, <= à 'maxBytes'.
	int readSome(void* buffer, int maxBytes);

   void write(const void* data, int cBytes);

	// Ferme la connexion. Réveille sur Exception les autres appels bloquants
	// sur cette instance. Sans effet si socket déjà fermé.
	void close();

private:
	NO_COPY_CTOR(TcpSock);
   friend class TcpServSock;
   TcpSock(int fd)   {_fd = fd;}

	int _fd;
};

// Socket en écoute pour le coté serveur.
class TcpServSock
{
public:
   TcpServSock(uint16 port);
   ~TcpServSock()    {close();}

   // Renvoie un socket valide, sinon exception.
   std::unique_ptr<TcpSock> accept();

   // Réveille sur exception les appels bloquants sur cette instance. Sans effet
   // si appelé plus d'une fois.
   void close();

private:
   NO_COPY_CTOR(TcpServSock);

   int _fd;
};
