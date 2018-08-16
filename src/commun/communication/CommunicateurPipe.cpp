//
// Created by terae on 6/24/18.
//

#include "CommunicateurPipe.h"
//#include <exception>
#include <sys/stat.h>
#include <unistd.h>
//#include <sstream>
//#include <string>

NamedPipe::NamedPipe(const std::string& path_read, const std::string& path_write, ITrameListener& robot)
        : Communicateur("named pipes", robot), _path_read(path_read), _path_write(path_write) {
	creerDescripteur(_path_read);
	creerDescripteur(_path_write);

	// Ouverture des FIFOs
	if((_fd_read = open(_path_read.c_str(), O_RDWR)) == -1) {
		throw ErreurOuverturePipe("Echec de l'ouverture du FIFO read (open).");
	}
	if((_fd_write = open(_path_write.c_str(), O_RDWR)) == -1) {
		throw ErreurOuverturePipe("Echec de l'ouverture du FIFO write (open).");
	}

	connexion();
	commencerReception();
}

NamedPipe::~NamedPipe() {}

// Ecrit un octet sur le socket
void NamedPipe::envoyer(uint8_t octet) {
	if(write(_fd_write, &octet, 1) != sizeof(octet)) {
		deconnexion();
		throw ErreurEmissionPipe("Echec d'envoi d'octet sur la communication par pipes (send).");
	}
}

// Attend un octet sur le socket et retourne le premier arrivé - BLOQUANT
uint8_t NamedPipe::lire() {
	uint8_t octet;
	if(read(_fd_read, &octet, 1) <= 0) {
		deconnexion();
		throw ErreurReceptionPipe("Echec de réception d'octet sur la communication par pipes (recv).");
	}
	return octet;
}

void NamedPipe::cleanUp() {
	_cleaningUp = true;

	// Fermeture des descripteurs de communication
	close(_fd_read);
	close(_fd_write);

	// Suppression des FIFOs
	unlink(_path_read.c_str());
	unlink(_path_write.c_str());

	attendreFinReception();

	deconnexion();
}

bool NamedPipe::creerDescripteur(const std::string& path) {
	bool pipe_created = false;
	struct stat info;
	if(lstat(path.c_str(), &info) != 0) {
		if(errno == ENOENT) {
			// n'existe pas
			if(mkfifo(path.c_str(), S_IRUSR | S_IWUSR | S_IRGRP) == -1) {
				throw ErreurOuverturePipe("Echec de la créaction du FIFO (mkfifo).");
			}
			pipe_created = true;
		} else if(errno == EACCES) {
			// pas la permission de savoir s'il existe
			throw ErreurOuverturePipe("Echec de l'ouverture du FIFO : pas les permissions nécessaires.");
		} else {
			// Autres erreurs
			throw ErreurOuverturePipe("Echec de la création du FIFO (erreurs générales).");
		}
	}

	if(!pipe_created) {
		// Un fichier existe, on vérifie que ce soit un pipe nommé
		if(!S_ISFIFO(info.st_mode)) {
			throw ErreurOuverturePipe("Echec de l'ouverture du FIFO : le chemin ne correspond pas à un pipe nommé.");
		}
	}

	return pipe_created;
}