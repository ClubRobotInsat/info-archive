/**********************
 * CarteMoteurs2018.h*
 * définit la classe gérant les moteurs 2018*
***********************/

#include "CarteMoteurs2018.h"


void CarteMoteurs2018::traiterMessage(Trame const& message) {
	switch(message.getCmd()) {

		case POSITION_TERMINEE:
			if(message.getNbDonnees() == 2) {
				std::lock_guard<std::mutex> lk(_mutexPositions);
				_termine[message.getDonnee(0)] = message.getDonnee(1);
			}
			break;

		default:
			this->Carte::traiterMessage(message);
			break;
	}
}

void CarteMoteurs2018::activerMoteur(uint8_t moteur, SensRotation sens) {
	envoyerMessage(this->make_trame(ACTIVER, moteur, sens));
}

void CarteMoteurs2018::desactiverMoteur(uint8_t moteur) {
	envoyerMessage(DESACTIVER, moteur);
}

void CarteMoteurs2018::activerBrushless(uint8_t moteur) {
	envoyerMessage(ACTIVER_BRUSHLESS, moteur);
}

void CarteMoteurs2018::positionTour(uint8_t moteur, std::size_t nbr_tours, SensRotation sens) {
	bloquerVariables();
	_termine[moteur] = false;
	debloquerVariables();
	std::lock_guard<std::mutex> lk(_mutexPositions);
	envoyerMessage(this->make_trame(TOUR, moteur, nbr_tours, sens));
}

void CarteMoteurs2018::positionAngle(uint8_t moteur, Angle angle) {
	bloquerVariables();
	_termine[moteur] = false;
	debloquerVariables();
	std::lock_guard<std::mutex> lk(_mutexPositions);
	envoyerMessage(this->make_trame(POSITION_ANGLE, moteur, angle));
}

bool CarteMoteurs2018::lirePosition(uint8_t moteur) {
	if(moteur >= NB_MAX_MOTEURS) {
		logError("Moteur demande", (int)moteur, ">=", NB_MAX_MOTEURS);
		throw std::runtime_error("Moteur Inexistant");
	}
	std::lock_guard<std::mutex> lk(_mutexPositions);
	return _termine[moteur];
}
