// Clock.cpp

#include "Clock.h"

Clock::Clock() {
	Reset();
}


unsigned int Clock::GetElapsedTime() const {
	return (getMSTime() - startTime);
}


void Clock::Reset() {
	startTime = getMSTime();
}
