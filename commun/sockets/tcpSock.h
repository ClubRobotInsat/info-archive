#pragma once
/******************************************************************************/
/* info/commun/tcpSock.h                                           2015-05-17 */
/* Club Robot INSA Toulouse                                      Félix Poisot */
/******************************************************************************/
#include "../Erreur.h"
#include "Utils.h"
#include <memory>
#include <cstdint>
/******************************************************************************/

EXCEPTION_CLASS(ErreurSocket);

// Version plus saine de Socket
class TcpSock
{
public:
   // Tente une connection à un serveur. Exception si échec
	TcpSock(const std::string& host, std::uint16_t port);
	// close() implicite si nécessaire.
   ~TcpSock()   {close();}

   // Accumule 'cBytes' octets quoiqui'il arrive.
   // Exception si pas assez de données disponibles.
	void read(void* buffer, int cBytes);

   // séparateur inclut dans le résultat.
   std::string readTextTo(char chr);

	// Retourne dès que des données sont disponibles.
	// Renvoie le nb. d'octets lus, <= à 'maxBytes'.
	int readSome(void* buffer, int maxBytes);

   // Vrai si le prochain appel à read(_,1) ou readSome(_,_) va retourner sans
   // bloquer
   bool hasNext();

   void write(const void* data, int cBytes);
   void write(const std::string& s)    {write(s.c_str(), s.size());}

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
   TcpServSock(std::uint16_t port);
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
