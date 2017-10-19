//
// Created by paul on 04/02/16.
//

#include "Simulateur.h"
#include "SimulateurConstantes.h"
#include "robot/Robot2017.h"
#include "world/World2017.h"
#include <Box2D/Box2D.h>
#include <atomic>
#include <getopt.h>
#include <signal.h>

using namespace std;

void printHelp() {
	std::cout << "Usage : --robot [on|off] --world [on|off] --color [blue|yellow] [--no-physics]" << std::endl;
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
	Constantes::RobotColor color = Constantes::RobotColor::Undef;

	int arg;
	static struct option long_options[] = {{"robot", required_argument, 0, 'r'},
	                                       {"color", required_argument, 0, 'c'},
	                                       {"world", required_argument, 0, 'w'},
	                                       {"no-physics", optional_argument, 0, 1},
	                                       {0, 0, 0, 0}};

	int long_index = 0;
	while((arg = getopt_long(argc, argv, "r:c:w:", long_options, &long_index)) != -1) {

		switch(arg) {
			case 'r':
				if(std::string(optarg) == "off") {
					robot = false;
				}
				break;
			case 'c':
				if(std::string(optarg) == "yellow") {
					color = Constantes::RobotColor::Yellow;
				} else if(std::string(optarg) == "blue") {
					color = Constantes::RobotColor::Blue;
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
			default:
				printHelp();
				return false;
		}
	}


	if(color == RobotColor::Undef) {
		logDebug5("Pas de couleur spécifiée.");
		printHelp();
		return false;
	}

	logDebug5("Démarrage du simulateur");

	// Robot
	if(robot) {
		simulateur.addRobot(color);
		logDebug5(std::string("Couleur du robot : ") + std::string(color == RobotColor::Blue ? "bleu" : "jaune"));
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

void parseConsole() {

	while(_simuAlive) {
		// A chaque entrée console, on reset le simu
		getchar();
		_simu.setResetWorldFlag(true);
	}
}

int main(int argc, char** argv) {
	// On coupe proprement le simu.
	signal(SIGINT, [](int) {
		_simuAlive = false;
		_simu.getServer().stop();
		exit(0);
	});

	if(parseArgument(argc, argv, _simu)) {
		logDebug5("Starting simulator");
		_simu.activateWebServer();

		auto last = TimePoint::now();

		// Thread pour analyser les entrées console ("entrée" pour reset par exemple)
		std::thread consoleThread(std::bind(&parseConsole));

		while(_simuAlive) {
			while(true || _simu.getServer().isClientConnected()) {
				_simu.update(10_ms);

				// On se cale sur 60 fps.
				auto now = TimePoint::now();
				sleep(max(0_s, 1 / 60_Hz - (now - last)));
				last = now;
			}
			sleep(50_ms);
		}
		_simu.shutdownWebServer();
		_simu.endWorld();
	}

	return EXIT_SUCCESS;
}
