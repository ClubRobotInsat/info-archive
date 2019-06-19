//
//  TestServo.cpp
//  Club Robot
//
//  Created by Rémi on 23/03/2015.
//

#include <stdio.h>

#include "../../Cartes/CarteIO2018.h"
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
		logError("Donne moi l'adresse !");
		return 1;
	}
	std::string adresse = argv[1];

	auto can = std::make_unique<Commun::CAN>(std::make_unique<Commun::RS232>(adresse.c_str()));
	can->setTemporisation(10_ms);

	auto reception = std::thread(&traiterMessage, can.get());

	// ID carte en 2e paramètre
	CarteIO2018 carte(*can, 4);
	cartes.push_back(&carte);

	carte.envoyerPing();

	sleep(1_s);

	if(carte.verifierPong()) {
		logSuccess("Carte : pong");
	} else {
		logFailed("Carte : KO");
	}

	while(true) {
		for(int i = 0; i < CarteIO2018::NBCAPTEURS; ++i) {
			carte.actualiserUnCapteur(i);
			logBleu(carte.lireUnCapteur(i));
		}
		std::cout << "\n\n";
		getchar();
	}
}