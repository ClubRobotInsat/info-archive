//
// Created by terae on 6/24/18.
//

#ifndef ROOT_COMMUNICATEURPIPE_H
#define ROOT_COMMUNICATEURPIPE_H

#include "Communicateur.h"
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

// Interface de communication inter-processus via des pipes nommés
class NamedPipe : public Communicateur {
public:
	NamedPipe(const std::string& path_read, const std::string& path_write, ITrameListener& robot);

	~NamedPipe() override;

	void envoyer(unsigned char octet) override;
	unsigned char lire() override;
	void cleanUp() override;

	// erreur si la connection n'est pas ouverte
	class ErreurOuverturePipe : public Communicateur::ErreurOuvertureCommunication {
	public:
		explicit ErreurOuverturePipe(std::string msg) : ErreurOuvertureCommunication(msg) {}
	};

	// erreur de lecture dans le canal de communication
	class ErreurReceptionPipe : public Communicateur::ErreurReceptionCommunication {
	public:
		explicit ErreurReceptionPipe(std::string msg) : ErreurReceptionCommunication(msg) {}
	};

	// erreur d'ecriture dans le canal de communication
	class ErreurEmissionPipe : public Communicateur::ErreurEmissionCommunication {
	public:
		explicit ErreurEmissionPipe(std::string msg) : ErreurEmissionCommunication(msg) {}
	};

private:
	int _fd_read;
	int _fd_write;

	std::string _path_read;
	std::string _path_write;

	bool creerDescripteur(const std::string& path);
};


#endif // ROOT_COMMUNICATEURPIPE_H
