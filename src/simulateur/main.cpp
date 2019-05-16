//
// Created by paul on 04/02/16.
//

#include <getopt.h>
#include <signal.h>

#include "Commun.h"
#include "core/Simulateur.h"
#include "core/SimulateurConstantes.h"
#include "core/World.h"
#include "log/Log.h"

using namespace std;

void printHelp(std::ostream& os = std::cout) {
	std::string colors = "["s + toString(Constants::RobotColor::Purple) + "|" + toString(Constants::RobotColor::Yellow) + "]";
	std::transform(colors.cbegin(), colors.cend(), colors.begin(), ::tolower);

	os << "Usage : --robot [name|off] --world [on|off] --color " << colors << " --load <path.json> [--no-physics]" << std::endl;
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
	std::string robot = "primary";
	bool world = true;
	std::string json_file;
	Constants::RobotColor color = Constants::RobotColor::Undef;

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
				robot = std::string(optarg);
				break;
			case 'c':
				color = Constants::string_to_color(std::string(optarg));
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

	if(color == Constants::RobotColor::Undef) {
		logError("Pas de couleur spécifiée.");
		printHelp();
		return false;
	}
	std::ifstream file(json_file.c_str());
	if(!file) {
		// Utilisation de TABLE_2018 par défaut
		logDebug4("Utilisation de TABLE_2018.json par défaut.");
		json_file = "";
	} else {
		logDebug("Utilisation d'une nouvelle table de jeu : '", json_file, "'.");
	}

	logDebug("Démarrage du simulateur");
	simulateur.setJSONFile(json_file);

	// Robot
	if(robot != "off") {
		simulateur.addRobot(robot, color);
		logDebug("Robot \"", robot, "\" ajouté ! ");
		logDebug(std::string("Couleur du robot : ") + toString(color));
	} else {
		logDebug4("Aucun robot ajouté.");
	}

	// Monde
	if(world) {
		simulateur.initWorld();
		logDebug("Ajout du monde au simulateur...");
	} else {
		logDebug4("Le monde n'a pas été ajouté.");
	}

	if(no_physics) {
		simulateur.disableSimulation();
		logDebug4("Desactivation de la physique");
	}

	return parsingOK;
}

int main(int argc, char** argv) {
	Simulateur& simu = Simulateur::getInstance();

	// On coupe proprement le simu.
	std::signal(SIGINT, [](int) {
		Simulateur::getInstance().requestStop();
		std::cout << "Demande d'arrêt du simulateur" << std::endl;
	});

	if(parseArgument(argc, argv, simu)) {
		logDebug("Starting simulator");
		simu.start();
	}

	return EXIT_SUCCESS;
}

// TODO [BIG (?) PROJECT] Gestion de l'IA au sein du simu
