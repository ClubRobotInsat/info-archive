//
// Created by terae on 6/24/18.
//

#include "NamedPipe.h"
#include <log/Log.h>

//#include <exception>
#include <sys/stat.h> // stat, lstat, mkfifo, S_ISFIFO
#include <unistd.h>   // close, unlink, write
//#include <sstream>
//#include <string>

namespace Communication {
	NamedPipe::NamedPipe(const std::string& path_read, const std::string& path_write)
	        : _path_read(path_read), _path_write(path_write) {
		// Si les pipes n'existent pas, on les crée
		creerDescripteur(_path_read);
		creerDescripteur(_path_write);

		// Ouverture des FIFOs
		if((_fd_read = open(_path_read.c_str(), O_RDWR)) == -1) {
			std::cout << "impossible to open read" << std::endl;
			throw ErreurOuverturePipe("Echec de l'ouverture du FIFO read (open).");
		}
		if((_fd_write = open(_path_write.c_str(), O_RDWR)) == -1) {
			std::cout << "impossible to open write" << std::endl;
			throw ErreurOuverturePipe("Echec de l'ouverture du FIFO write (open).");
		}
	}

	NamedPipe::~NamedPipe() {
		// Fermeture des descripteurs de communication
		close(_fd_read);
		close(_fd_write);

		// Suppression des FIFOs
		unlink(_path_read.c_str());
		unlink(_path_write.c_str());
	}

	void NamedPipe::ecrireOctets(uint8_t const* octets, std::size_t nombre) {
		ssize_t val = write(_fd_write, octets, nombre);
		if(val == -1) {
			logError("Erreur dans write : ");
			perror("Erreur retournée");
		}
	}

	void NamedPipe::lireOctets(uint8_t* octets, std::size_t nombre) {
		std::size_t lus = 0;
		while(lus < nombre) {
			ssize_t val = read(_fd_read, octets + lus, nombre - lus);
			if(val == -1) {
				logError("Erreur dans read : ");
				perror("Erreur retournée");
				sleep(1_s);
			}
			if(val == 0)
				throw ErreurEOF();
			lus += val;
		}
	}

	bool NamedPipe::creerDescripteur(const std::string path) {
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
				throw ErreurOuverturePipe(
				    "Echec de l'ouverture du FIFO : le chemin ne correspond pas à un pipe nommé.");
			}
		}

		return pipe_created;
	}
} // namespace Communication
