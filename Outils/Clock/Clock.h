#ifndef _CLOCK_H
#define _CLOCK_H

#include "getMSTime.h"

class Clock
{
public:
	Clock();
	unsigned int GetElapsedTime() const;	/// Retourne le temps écoulé depuis la dernière réinitialisation
	void Reset();	 /// Réinitialise le chrono

private:
	unsigned int startTime;
};

#endif
