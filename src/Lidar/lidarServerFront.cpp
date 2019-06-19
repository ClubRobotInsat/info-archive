/******************************************************************************/
/* info/robot/Lidar/lidarServerFront.cpp                           2015-05-07 */
/* Club Robot INSA Toulouse                                      Félix Poisot */
/******************************************************************************/
#include "lidarServer.h"
#include <list>
#include <log/Log.h>
#include <sockets/tcpSock.h>
#include <sstream>
/******************************************************************************/

// main() / partie réseau pour le serveur Lidar.

using namespace std;


unique_ptr<LidarThread> gLidars;
struct Server;

class Client {
public:
	Client(Server& server, unique_ptr<TcpSock> s) : _sock(move(s)), _server(server) {}
	~Client() {
		_thread->detach();
	}

	void start() {
		_thread = make_unique<thread>([this]() { run(); });
	}
	void run();

	int getId() const {
		return _id;
	}

	Vector2m getPosition() const {
		return _pos;
	}

private:
	void printUsage() const {
		_sock->write("Non supporté. Les commandes sont :\n"
		             "- [a]juste le placement d'un lidar de façon interactive\n"
		             "- [s]tatus: indique quels sont les lidars connectés\n"
		             "- [i]d: Identifie le client avec un numéro, a priori unique\n"
		             "- [p]osition Obtient la position des clients ou définit notre position auprès du serveur\n"
		             "- [r]elevés: liste les points et leurs positions.\n");
	}

	unique_ptr<TcpSock> _sock;
	unique_ptr<thread> _thread;
	Server& _server;

	[[noreturn]] void _mainloop();

	int _id = 0;
	Vector2m _pos;
};


class Server {
public:
	Server(uint16_t port) : _serv(port) {}
	~Server();
	int mainloop();

	string clientsAsText() {
		size_t count = 0;
		std::ostringstream oss;
		for(auto& c : _clis) {
			if(c->getId() != 0) {
				oss << c->getId() << ' ' << c->getPosition().x.toMm<int>() << ' ' << c->getPosition().y.toMm<int>() << ' ';
				++count;
			}
		}

		oss << '\n';
		return std::to_string(count) + ' ' + oss.str();
	}

private:
	friend class Client;
	void removeClient(Client*); // must be called by the client thread.

private:
	TcpServSock _serv;
	mutex _lClis;
	list<unique_ptr<Client>> _clis;
};

////==== Client ====////

void Client::run() {
	logDebug("Nouveau client");

	try {
		_mainloop();
	} catch(exception& e) {
		logError(e.what());
	}

	logDebug("Au revoir client !");
	_server.removeClient(this);
}

void Client::_mainloop() {
	for(;;) {
		auto cmd = _sock->readTextTo('\n');
		// char nomLidar;
		if(cmd.size() == 0) {
			continue;
		}
		switch(cmd[0]) {
			case 's': // status
				_sock->write("Lidars connectés: ");
				_sock->write(to_string(gLidars->status()));
				_sock->write("\n");
				break;

			case 'r': // relevés
				_sock->write(gLidars->recordsAsText());
				break;

			case 'a':
				while(!_sock->hasNext()) {
					_sock->write(gLidars->recordsChrMap());
					_sock->write("\n\n");
					sleep(1_s);
				}
				// consomme le message envoyé pour interrompre
				_sock->readTextTo('\n');

				break;
			case 'p': {
				if(cmd.size() < 2) {
					printUsage();
				} else {
					if(cmd[1] == 'g') {
						_sock->write(_server.clientsAsText());
					} else if(cmd[1] == 's') {
						istringstream is(cmd);
						is.ignore();
						is.ignore();
						int x, y;
						is >> x >> y;
						_pos.x = Distance::makeFromMm(x);
						_pos.y = Distance::makeFromMm(y);
					} else {
						printUsage();
					}
				}
				break;
			}
			case 'i': {
				_id = cmd[1] - '0';
				break;
			}
			default:
				printUsage();
				break;
		}
	}
}

////==== Server ====////

Server::~Server() {
	if(_clis.size()) {
		logError("Il reste encore des clients qui ne sont pas morts correctement. Plz debug !");
	}
}

int Server::mainloop() {
	try {
		for(;;) {
			Client* c = new Client(*this, _serv.accept());
			_lClis.lock();
			_clis.push_back(unique_ptr<Client>(c));
			_lClis.unlock();
			c->start();
		}
	} catch(...) {
		// planned exit ?
	}
	// can only exit on exception

	return 0;
}

void Server::removeClient(Client* me) {
	_lClis.lock();
	auto it = _clis.begin();
	for(; it->get() != me; ++it)
		; // will always find.
	_clis.erase(it);
	_lClis.unlock();
	return; // dans une méthode d'un objet juste détruit. l'appel doit être la
	        // dernière commande.
}

////==== Main ====////

[[noreturn]] void usage() {
	printf("Usage: serverLidar [-g] <#TcpPort>");
	exit(-1);
}

// Lit la ligne de commande, ouvre le socket et démarre
int main(int argc, char** argv) {
	int port = -1;
	bool useGL = false;

	try {
		for(int i = 1; i < argc; ++i) {
			string arg(argv[i]);
			if(arg[0] == '-') {
				if(arg[1] == 'g' && arg.size() == 2)
					useGL = true;
				else
					usage();
			} else if(port == -1)
				port = Utils::strToNumber<uint16_t>(arg);
			else
				usage();
		}
		if(port == -1)
			usage();

		Server serv(port);

		gLidars = make_unique<LidarThread>(useGL);
		gLidars->start();

		int rc = serv.mainloop();

		return rc;
	} catch(std::runtime_error& e) {
		logError(e.what());
		return -1;
	} catch(std::exception& e) { // parse error sur la ligne de commande
		                         // rien, affiche usage et exit.
	}
}
