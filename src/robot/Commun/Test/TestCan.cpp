//
//  TestCan.cpp
//  Club Robot
//
//  Created by Rémi on 31/01/2015.
//

#include "../Communication/RS232.h"
#include "CarteTestCAN.h"
#include <stdio.h>

void recepTrame(Trame t);
void traiterMessage(Commun::CAN& can, CarteTestCAN& carte);

void recepTrame(Trame t) {
	static int num = 0;
	logInfo("Trame reçue n°", ++num, " ", t);
}

void traiterMessage(Commun::CAN& can, CarteTestCAN& carte) {
	setThreadName("Traiter messages");
	auto stop = false;
	while(true) {
		Trame trame = can.recevoirTrame(&stop);
		carte.traiterMessage(trame);
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

	CarteTestCAN carte(*can, 0);
	carte.setListener(&recepTrame);

	auto threadReception = std::thread(&traiterMessage, std::ref(*can), std::ref(carte));
	sleep(100_ms);
	Trame t(1, 0);
	t.addDonnees(0x55);
	can->envoyerTrame(t);

	/*for(int i = 0; i < 10; ++i) {
	    Trame t(0, 1);
	    t.addByte(i);
	    t.addByte(0xCA);
	    t.addByte(0xFE);
	    t.addByte(0xBA);
	    t.addByte(0xBE);
	    t.addByte(0xDE);
	    t.addByte(0xCA);
	    t.addByte(0xFE);

	    can->envoyerTrame(t);
	}*/

	threadReception.join();

	return 0;
}