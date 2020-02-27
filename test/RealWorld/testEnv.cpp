//
// Created by abdelsatd on 2/13/20.
//


//#include "../../src/robot/Modules/NavigationParameters.h"

//#include "../../src/robot/Strategy/ModuleInterfacers/NavigationInterfacer.h"


#include "../../src/commun/Constants.h"
#include "../../src/commun/log/Log.h"
#include "../../src/robot/Strategy/Environment/environment.h"


int main(int argc, char** argv) {
	Distance robotWidth = 30_cm;
	Distance robotLength = 40_cm;
	Environment env({300, 200}, 1_cm, robotWidth, (sqrt(robotWidth * robotWidth + robotLength * robotLength) / 2) * 1.2, Vector2m(0_m, 1_m));

	logDebug1("Lecture de table 2020");
	JSON json = GLOBAL_CONSTANTS().TABLE_2020();

	logDebug1("Chargement de la table");
	env.loadFromJSON(json);

	logDebug1("Sauvegarde du tag");
	// env.saveToTGA("/home/abdelsaidt/.clubRobot");
}