#ifndef _CLOCK_H_H
#define _CLOCK_H_H

#include "Units/TimePoint.h"
#include <iostream>
#include <thread>

using Units::sleep;

class StopWatch {
public:
	StopWatch() {
		this->reset();
	}

	// Retourne le temps écoulé depuis la dernière réinitialisation
	Units::Duration getElapsedTime() const {
		auto val = std::chrono::duration_cast<std::chrono::nanoseconds>(Units::TimePoint::TimePointClock::now() - _startTime);
		return Units::Duration::makeFromNs(val.count());
	}

	// Réinitialise le chrono
	void reset() {
		_startTime = Units::TimePoint::TimePointClock::now();
	}

private:
	Units::TimePoint::TimePointType _startTime;
};
#endif
