//
//  CarteTestCAN.cpp
//  Club Robot
//
//  Created by Rémi on 31/01/2015.
//

#include "CarteTestCAN.h"

CarteTestCAN::CarteTestCAN(Commun::CAN& can, uint8_t id) : Carte(can, id) {}

void CarteTestCAN::traiterMessage(Trame const& message) {
	if(_listener) {
		_listener(message);
	}
}
