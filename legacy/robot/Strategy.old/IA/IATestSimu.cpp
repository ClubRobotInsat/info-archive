/*
 * IATestSimu.cpp
 *
 *  Created on: 12 mars 2015
 *      Author: seydoux
 */

#include "IATest.h"

int main(int argc, char* argv[]) {
	Log::open(argc, argv, false);

	logDebug6("Initialisation du robot...Veuillez patienter...");
	IATestDeplacement strategie({argv, argv + argc});

	logDebug6("Démarre la stratégie");
	strategie.demarrer(MATCH_DURATION * 100);

	return 0;
}

void IATestDeplacement::executer() {
	_robot->actualiserEtLireCoordonnees();


	std::cout << "position du robot au début : " << getRobot().lireCoordonnees().getPos2D() << '\n' << std::endl;
	getchar();
	std::cout << "Test de avancer - 20_cm" << std::endl;
	getchar();
	_dep->avancer(20_cm, SensAvance::Avant, 500_ms);
	std::cout << "Test de reculer - 10_cm" << std::endl;
	getchar();
	_dep->avancer(10_cm, SensAvance::Arriere, 500_ms);

	std::cout << "Test de tourner trigo - 45_deg" << std::endl;
	getchar();
	_dep->tournerRelatif(45_deg, 500_ms);
	std::cout << "Test de tourner horaire - 45_deg" << std::endl;
	getchar();
	_dep->tournerRelatif(-45_deg, 500_ms);

	std::cout << "Test de allerA - {2_m, 0_m} ; position initiale : " << getRobot().lireCoordonnees().getPos2D() << std::endl;
	getchar();
	_dep->allerA({1_m, 1.4_m}, SensAvance::Avant, 5_s);

	std::cout << "position finale du robot : " << getRobot().lireCoordonnees().getPos2D() << '\n' << std::endl;
}
