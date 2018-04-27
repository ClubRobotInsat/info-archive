//
// Created by paul on 04/02/16.
//

#include <atomic>
#include <getopt.h>
#include <signal.h>

//#include "communication/Robot2017.h"
#include "Commun.h"
#include "ConstantesCommunes.h"
#include "core/Simulateur.h"
#include "core/SimulateurConstantes.h"
#include "core/World.h"
#include "log/Log.h"

using namespace std;

void printHelp() {
	std::cout << "Usage : --robot [on|off] --world [on|off] --color [green|orange] --load <path.json> [--no-physics]" << std::endl;
}

/** Cette fonction parse les arguments envoyés au simu et
 * définit les variables en conséquence.
 * @return true si le simu peut être lancé suite aux arguments
 * proposés, false si les arguments ne permettent pas de lancer
 * le simulateur.
 */
bool parseArgument(int argc, char** argv, Simulateur& simulateur) {
	bool parsingOK = true;

	bool no_physics = false;
	bool robot = true;
	bool world = true;
	std::string json_file;
	Constantes::RobotColor color = Constantes::RobotColor::Undef;

	int arg;
	static struct option long_options[] = {{"robot", required_argument, 0, 'r'},
	                                       {"color", required_argument, 0, 'c'},
	                                       {"world", required_argument, 0, 'w'},
	                                       {"load", required_argument, 0, 'l'},
	                                       {"no-physics", optional_argument, 0, 1},
	                                       {"help", no_argument, 0, 'h'},
	                                       {0, 0, 0, 0}};

	int long_index = 0;
	while((arg = getopt_long(argc, argv, "r:c:w:l:h:", long_options, &long_index)) != -1) {

		switch(arg) {
			case 'r':
				if(std::string(optarg) == "off") {
					robot = false;
				}
				break;
			case 'c':
				if(std::string(optarg) == "orange") {
					color = Constantes::RobotColor::Orange;
				} else if(std::string(optarg) == "green") {
					color = Constantes::RobotColor::Green;
				}
				break;
			case 'w':
				if(std::string(optarg) == "off") {
					world = false;
				}
				break;
			case 1:
				no_physics = true;
				break;
			case 'l':
				json_file = std::string(optarg);
				break;
			case 'h':
			default:
				printHelp();
				return false;
		}
	}

	// TODO virer les vieux namespaces tous pourris comme Constantes (remplacer par des trucs logiques)
	if(color == Constantes::RobotColor::Undef) {
		logDebug5("Pas de couleur spécifiée.");
		printHelp();
		return false;
	}
	std::ifstream file(json_file.c_str());
	if(!file) {
		// Utilisation de TABLE_2018 par défaut
		json_file = "";
	}

	logDebug5("Démarrage du simulateur");
	simulateur.setJSONFile(json_file);

	// Robot
	if(robot) {
		simulateur.addRobot(color);
		logDebug5(std::string("Couleur du robot : ") + std::string(color == Constantes::RobotColor::Orange ? "orange" : "vert"));
	} else {
		logDebug4("Aucun robot ajouté.");
	}

	// Monde
	if(world) {
		simulateur.initWorld();
		logDebug5("Ajout du monde au simulateur...");
	} else {
		logDebug4("Le monde n'a pas été ajouté.");
	}

	if(no_physics) {
		simulateur.disableSimulation();
		logDebug4("Desactivation de la physique");
	}

	return parsingOK;
}

namespace {
	std::atomic_bool _simuAlive = {true};
	Simulateur _simu;
}

void stopSimu() {
	_simuAlive = false;
}

void parseConsole() {

	while(_simuAlive) {
		// Obtention de la commande de l'utilisateur.
		std::string input;
		std::getline(std::cin, input);

		// quitter
		if(input == std::string("q")) {
			_simuAlive = false;
			// reset
		} else if(input == std::string("r")) {
			_simu.setResetWorldFlag(true);
			// commande inconnue, afficher l'aide
		} else {
			std::cout << "help : q (quit), r (reset)"
			          << std::endl; // TO DO Afficher l'aide de façon synchrone avec un buffer
		}
	}
}

int main(int argc, char** argv) {
	// On coupe proprement le simu.
	signal(SIGINT, [](int) {
		_simuAlive = false;
		std::cout << "Demande d'arrêt du simulateur" << std::endl;
	});

	if(parseArgument(argc, argv, _simu)) {
		logDebug5("Starting simulator");

		auto last = TimePoint::now();

		// Thread pour analyser les entrées console ("entrée" pour reset par exemple)
		std::thread consoleThread(std::bind(&parseConsole));
		consoleThread.detach();

		while(_simuAlive) {
			_simu.update(10_ms);

			// On se cale sur 60 fps.
			auto now = TimePoint::now();
			sleep(max(0_s, 1 / 60_Hz - (now - last)));
			last = now;
		}
	}

	return EXIT_SUCCESS;
}

// TODO [BIG PROJECT] Lecture de la table en Json
// TODO [BIG (?) PROJECT] Gestion de l'IA au sein du simu
