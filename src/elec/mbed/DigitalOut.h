//
// Created by paul on 24/04/18.
//

#ifndef ROOT_DIGITALOUT_H
#define ROOT_DIGITALOUT_H


#include "PinName.h"
#include <iostream>

class DigitalOut {

public:
	DigitalOut(PinName pin) : _pin(pin) {}

	void write(int value) {
		std::cout << "Mise du pin " << _pin << " a " << value << std::endl;
	}

private:
	PinName _pin;
};


#endif // ROOT_DIGITALOUT_H
