#ifndef _CLOCK_H
#define _CLOCK_H

#include "getMSTime.h"

class Clock {
public:
	Clock();
	unsigned int getElapsedTime() const; /// Retourne le temps écoulé depuis la dernière réinitialisation
	void reset(); /// Réinitialise le chrono
	
private:
	unsigned int _startTime;
};

#endif
