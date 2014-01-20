// Clock.cpp

#include "Clock.h"

Clock::Clock() {
	this->reset();
}


unsigned int Clock::getElapsedTime() const {
	return (getMSTime() - _startTime);
}


void Clock::reset() {
	_startTime = getMSTime();
}
