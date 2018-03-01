#include "IAPrincipal.h"
#include "../petri/Deplacement.h"
#include "../petri/Servo.h"
#include "../petri/src/IA2017.h"
#include "Parsing.h"
#include <petri/Runtime/Cpp/Petri.h>

#define CHECK_LIST 0
int main(int argc, char* argv[]) {

	// Couleur du robot
	RobotColor color = ia_parsing::parseColor(argc, argv);
	bool debugMode = ia_parsing::getDebugMode(argc, argv);

	Log::open(argc, argv, false);

	logDebug6("Initialisation du robot...Veuillez patienter...");
	IAPrincipal strategie({argv, argv + argc}, color, debugMode);

	logDebug6("Démarre la stratégie");
	if(!debugMode) {
		strategie.demarrer(MATCH_DURATION);
	} else {
		strategie.demarrer(MATCH_DURATION * 100);
	}
	logDebug9("fin initialisation");
	return 0;
}

IAPrincipal::IAPrincipal(std::vector<std::string> const& args, Constantes::RobotColor color, bool debugMode)
        : StrategiePrincipal(args, color), _debugMode(debugMode) {
	// main --> IAPrincipal() --> initialisation --> attendre tirette -> démarrer
	this->initialisation();
#ifdef HOMOLOGATION
	_petri = Petri::Generated::Homologation_createLib(".");
#else
	_petri = Petri::Generated::IA2017_createLib(".");
#endif
	_petri->load();
}

void IAPrincipal::initialisation() {
#if CHECK_LIST
	logCyan("-----------------------");
	logCyan("Carte pinguées ?");
	getchar();
	logCyan("La tourelle tourne-t-elle ? ");
	getchar();
	logCyan("Cadre recalages placés ?");
	getchar();
	logCyan("Couleur robot : ", this->getCouleurEquipe());
	getchar();
	logCyan("-----------------------");
#endif
	//_robot->getCAN().setDebug(true);
	// Recallage du robot (manuel)
	// Test pour voir si on a tout ping
	logDebug9("Initialisation");

	if(!_debugMode) {
		// se mettre en position de départ : sortirBrasGaucheMax(); ...

		//		sortirParasol();
		//		sleep(50_ms);
		//		rentrerParasol();
	}


	logCyan("Couleur robot : ", this->getCouleurEquipe());
	logDebug("Recalage du robot");

	repere::Coordonnees coords_robot(START_ROBOT_POSITION,
	                                 START_ROBOT_ANGLE,
	                                 getCouleurEquipe() == RobotColor::Orange ? REFERENCE_ORANGE : REFERENCE_GREEN);

	getDeplacement().setRepere(coords_robot);

/// Recalage
/*_dep->avancer(3_cm, SensAvance::Avant, 500_ms);
sleep(1_s);
_dep->avancer(3_cm, SensAvance::Arriere, 500_ms);*/
#if CHECK_LIST
	logCyan("-----------------------");
	logCyan("Cadre décalage enlevé ?");
	getchar();
	logCyan("-----------------------");
#endif

	// Attente pour le départ
	logDebug0("Tirette");

	if(!_debugMode) {
		if(_robot->getModeConnexion() == Commun::ModeConnexion::LOCAL) {
			// Attente en mode simu
			// TODO faire une tirette en mode simu ?
			std::cout << "Mode local : Appuyez sur une touche." << std::endl;
			getchar();
		} else {
			attendreTirette();
		}
	}
}

void IAPrincipal::executer() {
	_dep->setDebugState(true);

	logDebug5("Debut exécuter");
#ifdef HOMOLOGATION
	_robot->setAngleDetectionAdv(0.5_PI);
#else
	_robot->setAngleDetectionAdv(0.4_PI);
#endif

	lancerPetri(_debugMode);

	/*std::thread([=]() {
	    logDebug(_robot->getPositionAdversaire());
	});*/
}

void IAPrincipal::funnyAction() {
	//_meca->lancerEnginSpatial();
}

ResultatAction IAPrincipal::lancerPetri(bool debug) {
	if(debug) {
		Petri::DebugServer debugSession(*_petri);
		debugSession.start();
		debugSession.join();
	} else {
		auto ia = _petri->createPetriNet();
		ia->run();

		// Garde fou pour les 90s de match
		while(getTempsRestant() > 0_s) {
			sleep(50_ms);
		}

		ia->stop();
	}

	return ResultatAction::REUSSI;
}
