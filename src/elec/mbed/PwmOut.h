//
// Created by paul on 24/04/18.
//

#ifndef ROOT_PWMOUT_H
#define ROOT_PWMOUT_H


#include "PinName.h"
#include <iostream>

class PwmOut {
public:
	PwmOut(PinName pin) : _pin(pin) {}

	void write(float value) {}

	void period(float seconds) {}
	void period_ms(int ms) {}
	void period_us(int us) {}
	void pulsewidth(float seconds) {}
	void pulsewidth_ms(int ms) {}
	void pulsewidth_us(int us) {}

private:
	PinName _pin;
};


#endif // ROOT_PWMOUT_H
