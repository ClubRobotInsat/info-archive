// Carte.cpp

#include "Carte.h"
#include "../RobotLogic.h"
#include <iostream>

// constructeur
Carte::Carte(uint8_t id, RobotLogic& robot) : _id(id), _robot(robot) {
	// lève l'exception ErreurIdCarteTropGrand si l'id est trop grand
	if(_id >= Trame::NB_CARTES_MAX)
		throw Trame::ErreurIdCarteTropGrand(id);
}

// destructeur
Carte::~Carte() {
	if(!_bufferReception.empty()) {
		std::cerr << "La carte " << _id << " a " << _bufferReception.size()
		          << " trames non traitées lors de sa destruction !!!" << std::endl;
		while(!_bufferReception.empty()) {
			_bufferReception.pop();
		}
	}
}

// envoie une trame a l'IA
void Carte::envoyer(Trame const& t, bool isAck) {
	_robot.onEnvoye(t, isAck);
}

// ajoute une trame dans le buffer de reception
void Carte::onRecu(Trame const& t, bool isAck) {
	// cette trame sera ensuite detruite apres son traitement
	_mutex.lock();
	_bufferReception.push(t);
	_mutex.unlock();

	if(isAck) {
		// Renvoie les données avec le flag : ACK
		envoyer(t, true);
	}
}

// fonction appelée par le robot pour traiter le buffer de réception
void Carte::traiterBuffer() {
	// traite la totalité des trames contenues dans le buffer
	while(!_bufferReception.empty()) {
		try {
			_mutex.lock();
			Trame trame = _bufferReception.front();
			_bufferReception.pop();
			_mutex.unlock();
			this->traiterTrame(trame);
		} catch(Trame::ErreurTrameNonTraitee e) {
			std::cerr << e.what() << std::endl;
		}
	}
}

// fonction a définir dans les cartes qui ont besoin de faire un traitement d'elles-mêmes
// ne pas oublier d'appeler le update de la classe carte sinon les trames ne seront pas traitées
void Carte::update(Duration) {
	// sera appelée automatiquement a chaque calcul de frame
	traiterBuffer();
}
