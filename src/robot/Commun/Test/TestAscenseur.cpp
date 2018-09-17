//
//  TestServo.cpp
//  Club Robot
//
//  Created by Rémi on 23/03/2015.
//

#include <stdio.h>

#include "../../Cartes/CarteAsservissement2009.h"
#include "../BlockingInterfaces/Ascenseur.h"
#include "../Communication/RS232.h"
#include <stdio.h>

std::vector<Carte*> cartes;

void traiterMessage(Commun::CAN* can);

void traiterMessage(Commun::CAN* can) {
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
		logError("Donne moi l'adresse ! (/dev/ttyUSB0");
		return 1;
	}
	std::string adresse = argv[1];

	auto can = std::make_unique<Commun::CAN>(std::make_unique<Commun::RS232>(adresse.c_str()));
	can->setTemporisation(10_ms);

	// ID carte en 2e paramètre
	CarteAsservissement2009 carte(*can, 5);
	cartes.push_back(&carte);

	auto reception = std::thread(&traiterMessage, can.get());

	carte.envoyerPing();

	sleep(1_s);

	if(carte.verifierPong()) {
		logSuccess("Carte : pong");
	} else {
		logFailed("Carte : KO");
	}

	std::vector<Angle> pos;
	for(int i = 0; i < 200; ++i) {
		pos.push_back(i * 360_deg);
	}

	Commun::Ascenseur asc(carte, pos);

	asc.initialiser(SensRotation::Horaire);
	logInfo("Recalage terminé.");

	for(int i = 0; i < 200; i++) {
		getchar();
		logBleu(asc.incrementer());
	}
}