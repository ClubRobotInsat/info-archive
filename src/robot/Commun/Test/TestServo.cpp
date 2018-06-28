//
//  TestServo.cpp
//  Club Robot
//
//  Created by Rémi on 23/03/2015.
//

#include <stdio.h>

#include "../../Cartes/CarteServosNova2017.h"
#include "../Communication/RS232.h"
#include "../Servo.h"
#include <stdio.h>

using namespace Commun;

std::vector<Carte*> cartes;

void traiterMessage(Commun::CAN* can);

void traiterMessage(CAN* can) {
	setThreadName("Traiter messages Robot");

	while(true) {
		Trame trame = can->recevoirTrameBloquant();
		try {
			for(auto c : cartes) {
				if(c->getId() == trame.getId()) {
					c->traiterMessage(trame);
				}
			}
		} catch(std::exception& e) {
			logError("Carte invalide !!!");
			logError("Exception rencontrée : ", e.what());
		}
	}
}

int main(int argc, char** argv) {
	if(argc < 2) {
		logError("Donne moi l'adresse !");
		return 1;
	}
	std::string adresse = argv[1];

	auto can = std::make_unique<CAN>(std::make_unique<RS232>(adresse.c_str()));
	can->setTemporisation(10_ms);

	// ID carte en 2e paramètre
	CarteServosNova2017 carte(*can, 6);
	cartes.push_back(&carte);

	auto reception = std::thread(&traiterMessage, can.get());

	carte.envoyerPing();

	sleep(1_s);

	if(carte.verifierPong()) {
		logSuccess("Carte : pong");
	} else {
		logFailed("Carte : KO");
	}

	ServoManager s(carte, nullptr);

	Angle a1, a2;
	double aa;
	while(true) {
		logInfo("Angle ?");
		std::cin >> aa;
		a1 = Angle::makeFromDeg(aa);
		logInfo("Je vais à ", a1.toDeg(), " deg, ", a2.toDeg(), " deg");
		// ID servo en 1er paramètre
		logInfo(s.positionnerServoBloquant(1, a1));
	}
}