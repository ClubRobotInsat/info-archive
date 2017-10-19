// Carte.h

#ifndef CARTE_SIMU_H
#define CARTE_SIMU_H

#include <Commun.h>
#include <log/Log.h>

class RobotLogic;

#include <mutex>
#include <queue>
#include <string>

class Carte {
public:
	// constructeur
	Carte(uint8_t id, RobotLogic& robot);
	// lève l'exception ErreurIdCarteTropGrand si l'id est trop grand

	// destructeur
	virtual ~Carte();

	// accesseur sur l'id de la carte
	uint8_t getId() const {
		return _id;
	}

	// accesseur sur le robot a qui appartient cette carte
	RobotLogic& getRobot() const {
		return _robot;
	}

	// fonction à définir dans chaque carte pour le traitement des trames
	virtual void traiterTrame(Trame const& t) = 0;
	// sera appelé automatiquement avec la prochaine trame dans le buffer de reception

	// fonction à définir dans les cartes qui ont besoin de faire un traitement d'elles-mêmes
	// ne pas oublier d'appeler le update de la classe carte sinon les trames ne seront pas traitées
	virtual void update(Duration elapsed);
	// sera appelé automatiquement à chaque calcul de frame

	// ajoute une trame dans le buffer de réception
	void onRecu(Trame const& t, bool isAck = false);
	// cette trame sera ensuite detruite apres son traitement

private:
	// numéro de la carte
	uint8_t const _id;

	// buffer des trames reçues
	std::queue<Trame> _bufferReception;

	// robot a qui appartient cette carte pour envoyer la trame
	RobotLogic& _robot;

	// fonction appelée par le update pour traiter le buffer de réception
	void traiterBuffer();
	// traite la totalité des trames contenues dans le buffer

	// mutex pour l'acces des buffers de reception
	std::mutex _mutex;

protected:
	// envoie une trame a l'IA
	void envoyer(Trame const& t, bool isAck = false);
	// cette trame sera detruite par le robot
};

#endif // CARTE_H
