//
// Created by scriptopathe on 26/04/16.
//

#include "IATestRobot.h"
#include "../../Principal/petri/Deplacement.h"
#include "../../Principal/petri/Servo.h"
#include "../../Principal/petri/Utils.h"
#include "IAPrincipal.h"
#include "Parsing.h"

#define getchar                              \
	[]() {                                   \
		std::cout << "Appuyez sur entrée :"; \
		getchar();                           \
	}

#ifdef SIMULATED_MAIN
int main_robot(int argc, char* argv[]) {
#else
int main(int argc, char* argv[]) {
#endif
	Log::open(argc, argv, false);

	RobotColor color = ia_parsing::parsing_function(argc, argv).color;

	logDebug6("Initialisation du robot...Veuillez patienter...");
	IATestRobot strategie({argv, argv + argc}, color);

	logDebug6("Démarre la stratégie");
	// -3s pour avoir le temps d'ouvrir les pinces ci besoin
	if(getenv("EPIA"))
		strategie.demarrer(MATCH_DURATION - 3_s);
	else {
		// logDebug3("TIRETTTEEE");
		strategie.demarrer(MATCH_DURATION * 100);
	}
	return 0;
}

/// Attention à la création
IATestRobot::IATestRobot(std::vector<std::string> const& args, Constantes::RobotColor color)
        : StrategiePrincipal(args, color) {
	this->initialisation();
}

void IATestRobot::executer() {
	_dep->setDebugState(true);

	_robot->setAngleDetectionAdv(0.5_PI);
	//_robot->setEnvoiPositionAdversaireAuto(true);

	//    while(true)
	//        logDebug("present :", _robot->adversairePresent(SensAvance::Avant));

	// this->attendreTirette();
	logDebug3("Appuyer sur entrée...");
	getchar();

	_dep->avancerDe(20_cm, SensAvance::Avant);
	_dep->tournerAbsolu(0_deg);
	_dep->allerA({2_m, 1_m}, 20_s);

#ifdef HOMOLOGATION
	_robot->setAngleDetectionAdv(1_PI);
#endif
}
