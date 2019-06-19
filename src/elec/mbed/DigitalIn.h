//
// Created by paul on 24/04/18.
//

#ifndef ROOT_DIGITALIN_H
#define ROOT_DIGITALIN_H


#include "PinMode.h"
#include "PinName.h"
#include <iostream>

class DigitalIn {
public:
	DigitalIn(PinName pin, PinMode mode = PullUp) : _mode(mode), _pin(pin) {}

	int read() {
		std::cout << "Lecture sur le pin : " << _pin << std::endl;
		// TODO : faire un random ?
		return 1;
	}

private:
	PinMode _mode;
	PinName _pin;
};

#endif // ROOT_DIGITALIN_H
