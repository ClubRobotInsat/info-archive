#include "CarteEvitement2012.h"
#include "CarteDeplacement2009.h"

CarteEvitement2012::CarteEvitement2012(Commun::CAN& can, uint8_t id, Commun::ICapteurs* robot)
        : Carte(can, id), _robot(robot), _estPret(false) {}

/// Traite les messages de la carte
void CarteEvitement2012::traiterMessage(Trame const& message) {
	switch(message.getCmd()) {
		case EST_PRET: // EST_PRET
			if(message.getNbDonnees() == 1) {
				if(message.getDonnee(0) == 1) {
					_estPret = true;
				} else {
					_estPret = false;
				}
			}
			break;
		case ROBOT0: // POSITION ROBOT 0
			if(message.getNbDonnees() == 8) {
				bloquerVariables();
				_positionAdversaire[0].x = message.getDonnee<float>(0);
				_positionAdversaire[0].y = message.getDonnee<float>(4);
				debloquerVariables();
			}
			break;
		case ROBOT1: // POSITION ROBOT 1
			if(message.getNbDonnees() == 8) {
				bloquerVariables();
				_positionAdversaire[1].x = message.getDonnee<float>(0);
				_positionAdversaire[1].y = message.getDonnee<float>(4);
				debloquerVariables();
			}
			break;
		default:
			this->Carte::traiterMessage(message);
			break;
	}
}

void CarteEvitement2012::reset() {
	envoyerMessage(RESET, 0xFF_b);
}

void CarteEvitement2012::demandeEstPret() {
	envoyerMessage(EST_PRET, 0x0F_b); // Peu importe, elle n'est pas interprétée par la carte
}

bool CarteEvitement2012::getEstPret() {
	return _estPret;
}

void CarteEvitement2012::demandePositionAdv(uint8_t numAdv) {
	envoyerMessage(DEMANDE, numAdv);
}

void CarteEvitement2012::envoyerPositionHyperion() {
	bloquerVariables();
	Coordonnees CoordHyp = _robot->lireCoordonnees();
	debloquerVariables();

	Trame t = this->make_trame(ROBOT0, (CoordHyp.getX().toMm<std::int16_t>()), (CoordHyp.getY().toMm<std::int16_t>()));
	envoyerMessage(t);
}

void CarteEvitement2012::setConfig(uint8_t envoiAuto, uint8_t petitRobot, uint8_t cataRobots, uint8_t cataExterne) {
	bloquerVariables();
	envoyerMessage(this->make_trame(CONFIG, envoiAuto, petitRobot, cataRobots, cataExterne));
	debloquerVariables();
}

double CarteEvitement2012::getPositionAdversaireX(uint8_t numAdv) {
	double resultat;

	bloquerVariables();
	resultat = _positionAdversaire[numAdv].x;
	debloquerVariables();

	return resultat;
}

double CarteEvitement2012::getPositionAdversaireY(uint8_t numAdv) {
	double resultat;

	bloquerVariables();
	resultat = _positionAdversaire[numAdv].y;
	debloquerVariables();

	return resultat;
}
