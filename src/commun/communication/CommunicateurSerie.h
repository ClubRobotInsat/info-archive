// Serie.h

#ifndef _RS232_H_
#define _RS232_H_

#include "Communicateur.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifdef WIN32
#include <windows.h>
#else
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>
#endif

#include <string>


// Interface de communication série via port COM (RS232) ou USB
class Serie : public Communicateur {
public:
	// Ouvre le port série indiqué et le configure
	Serie(const std::string& port, ITrameListener& robot);
	// lève ErreurOuvertureCommunicationSerie si imposible d'ouvrir la connection

	~Serie();

	// Ecrit un octet sur le port
	void envoyer(unsigned char octet);

	// Attend un octet sur le port et retourne le premier arrivé - BLOQUANT
	unsigned char lire();

	// force une déconnexion (ferme le port), et attend la fin du thread de réception.
	virtual void cleanUp();

	// erreur si la connection n'est pas ouverte
	class ErreurOuvertureSerie : public Communicateur::ErreurOuvertureCommunication {
	public:
		ErreurOuvertureSerie(std::string msg) : ErreurOuvertureCommunication(msg) {}
	};

private:
// Descripteur du port série
#ifdef WIN32
	HANDLE _fd;
#else
	int _fd;
#endif
};

#endif
