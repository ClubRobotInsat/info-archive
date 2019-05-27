//
// Created by terae on 6/24/18.
//

#include "NamedPipe.h"
#include <log/Log.h>

#include <sys/stat.h> // stat, lstat, mkfifo, S_ISFIFO
#include <unistd.h>   // close, unlink, write

namespace Communication {

	NamedPipe::NamedPipe(const std::string& path_read, const std::string& path_write)
	        : _path_read(path_read), _path_write(path_write) {
		// Si les pipes n'existent pas, on les crée
		create_descriptor(_path_read);
		create_descriptor(_path_write);

		// Ouverture des FIFOs
		if((_fd_read = open(_path_read.c_str(), O_RDWR)) == -1) {
			throw ErrorPipeOpening("NamedPipe::open: Impossible to open the read FIFO.");
		}
		if((_fd_write = open(_path_write.c_str(), O_RDWR)) == -1) {
			close(_fd_read);
			unlink(_path_read.c_str());
			throw ErrorPipeOpening("NamedPipe::open: Impossible to open the write FIFO.");
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

	void NamedPipe::write_bytes(const uint8_t* bytes, std::size_t bytes_number) {
		ssize_t val = write(_fd_write, bytes, bytes_number);
		if(val == -1) {
			logError("Erreur dans write : ");
			perror("Erreur retournée");
		}
	}

	size_t NamedPipe::read_bytes(uint8_t* bytes, std::size_t bytes_number) {
		std::size_t nb_read = 0;
		while(nb_read < bytes_number) {
			ssize_t val = read(_fd_read, bytes + nb_read, bytes_number - nb_read);
			if(val == -1) {
				logError("Erreur dans read : ");
				perror("Erreur retournée");
				sleep(1_s);
			}
			if(val == 0) {
				throw ErreurEOF();
			}
			nb_read += val;
		}
		return bytes_number;
	}

	bool NamedPipe::create_descriptor(const std::string& path) {
		bool pipe_created = false;
		struct stat info {};
		if(lstat(path.c_str(), &info) != 0) {
			if(errno == ENOENT) {
				// n'existe pas
				if(mkfifo(path.c_str(), S_IRUSR | S_IWUSR | S_IRGRP) == -1) {
					throw ErrorPipeOpening("Echec de la créaction du FIFO (mkfifo).");
				}
				pipe_created = true;
			} else if(errno == EACCES) {
				// pas la permission de savoir s'il existe
				throw ErrorPipeOpening("Echec de l'ouverture du FIFO : pas les permissions nécessaires.");
			} else {
				// Autres erreurs
				throw ErrorPipeOpening("Echec de la création du FIFO (erreurs générales).");
			}
		}

		if(!pipe_created) {
			// Un fichier existe, on vérifie que ce soit un pipe nommé
			if(!S_ISFIFO(info.st_mode)) {
				throw ErrorPipeOpening("Echec de l'ouverture du FIFO : le chemin ne correspond pas à un pipe nommé.");
			}
		}

		return pipe_created;
	}

} // namespace Communication
