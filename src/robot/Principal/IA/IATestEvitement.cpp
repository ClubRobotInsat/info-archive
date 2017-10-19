//
// Created by louis on 27/04/17.
//

#include "IATest.h"

int main(int argc, char* argv[]) {
	Log::open(argc, argv, false);

	logDebug6("Initialisation du robot…Veuillez patienter…");
	IATestEvitement strategie({argv, argv + argc});

	logDebug6("Démarre la stratégie");
	strategie.demarrer(MATCH_DURATION * 100);

	return 0;
}

void IATestEvitement::executer() {
	_dep->setDebugState(true);

	_robot->setAngleDetectionAdv(0.5_PI);
	_robot->setEnvoiPositionAdversaireAuto(true);

	// Test des shift
	logDebug("Angle calculé pour un angle reçu de 90 degrés : ", getEvitement().shiftAngle(90_deg).toDeg());
	logDebug("Angle calculé pour un angle reçu de 0 degrés : ", getEvitement().shiftAngle(0_deg).toDeg());
	logDebug("Angle calculé pour un angle reçu de -90 degrés : ", getEvitement().shiftAngle(-90_deg).toDeg());

	// Test évitement
	_dep->setRepere({0_m, 0_m}, -90_deg);

	while(_running) {
		std::cout << "Position de l'adversaire : " << _evitement->getPositionAdversaire() << std::endl;
		std::cout << "Angle détecté pour l'adversaire : " << _robot->getAngleAdversaireDetecte() << std::endl;
		std::cout << "Angle après décalage de la tourelle : "
		          << _evitement->getPositionAngulaireAdversaire().toMinusPiPi().toDeg() << std::endl;
		getchar();
	}
}