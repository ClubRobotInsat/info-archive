#ifndef _CLOCK_H_H
#define _CLOCK_H_H

#include <chrono>
#include <iostream>
#include "MathToolbox/MathUtilities.h"

#ifndef _WIN32
// Cet en-tête définit une fonction sleep. Pour ne pas foirer la compilation en substituant la macro ci-après, on inclut le header d'abord.
#include <unistd.h>
#endif

#define sleep(delay) std::this_thread::sleep_for(std::chrono::nanoseconds(static_cast<long long>((delay).value() * 1e9)))

class Clock {
public:
	Clock() {
		this->reset();
	}

	/// Retourne le temps écoulé depuis la dernière réinitialisation
	dureeS getElapsedTime() const {
		auto val = std::chrono::duration_cast<std::chrono::nanoseconds>(ClockType::now() - _startTime);
		return dureeS::makeFromNs(val.count());
	}

	void reset() { /// Réinitialise le chrono
		_startTime = ClockType::now();
	}
	
private:
	using ClockType = std::chrono::high_resolution_clock;
	std::chrono::time_point<ClockType> _startTime;
};

#endif
