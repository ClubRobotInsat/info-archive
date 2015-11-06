#ifndef _CLOCK_H_H
#define _CLOCK_H_H

#include <iostream>
#include <thread>
#include "Units/Time.h"

#ifndef _WIN32
// Cet en-tête définit une fonction sleep. Pour ne pas foirer la compilation en substituant la macro ci-après, on inclut
// le header d'abord.
#include <unistd.h>
#endif

#define sleep(delay) std::this_thread::sleep_for((delay).toSystemDelay())

class TimePoint {
public:
	using TimePointClock = std::chrono::high_resolution_clock;
	using TimePointType = std::chrono::time_point<TimePointClock>;

	TimePoint() : _value(TimePointClock::now()) {}
	TimePoint(TimePointType d) : _value(d) {}
	TimePoint(TimePoint const &d) : _value(d.value()) {}

	TimePoint &operator=(TimePoint d) {
		swap(*this, d);
		return *this;
	}

	friend void swap(TimePoint &d1, TimePoint &d2) {
		using std::swap;
		swap(d1._value, d2._value);
	}

	static TimePoint now() {
		return TimePoint(TimePointClock::now());
	}

	static TimePoint distantPast() {
		return TimePoint(TimePointClock::now() - std::chrono::hours(24) * 365 * 100);
	}

	static TimePoint distantFuture() {
		return TimePoint(TimePointClock::now() + std::chrono::hours(24) * 365 * 100);
	}

	TimePoint &operator-=(Duration const &d);
	TimePoint &operator+=(Duration const &d);

	TimePointType value() const {
		return _value;
	}

private:
	TimePointType _value;
};

inline bool operator<(TimePoint const &d1, TimePoint const &d2) {
	return d1.value() < d2.value();
}

inline bool operator==(TimePoint const &d1, TimePoint const &d2) {
	return d1.value() == d2.value();
}

inline bool operator>(TimePoint const &d1, TimePoint const &d2) {
	return d2 < d1;
}

inline bool operator!=(TimePoint const &d1, TimePoint const &d2) {
	return !(d1 == d2);
}

inline bool operator<=(TimePoint const &d1, TimePoint const &d2) {
	return !(d1 > d2);
}

inline bool operator>=(TimePoint const &d1, TimePoint const &d2) {
	return !(d1 < d2);
}

inline Duration operator-(TimePoint const &d1, TimePoint const &d2) {
	auto delta = std::chrono::duration_cast<std::chrono::nanoseconds>(d1.value() - d2.value());
	return Duration::makeFromNs(delta.count());
}

inline TimePoint operator-(TimePoint const &d1, Duration const &d2) {
	return TimePoint(d1.value() - d2.toSystemDelay());
}

inline TimePoint operator+(TimePoint const &d1, Duration const &d2) {
	return TimePoint(d1.value() + d2.toSystemDelay());
}

inline TimePoint &TimePoint::operator-=(Duration const &d) {
	*this = *this - d;
	return *this;
}

inline TimePoint &TimePoint::operator+=(Duration const &d) {
	*this = *this + d;
	return *this;
}

class Clock {
public:
	Clock() {
		this->reset();
	}

	// Retourne le temps écoulé depuis la dernière réinitialisation
	Duration getElapsedTime() const {
		auto val = std::chrono::duration_cast<std::chrono::nanoseconds>(TimePoint::TimePointClock::now() - _startTime);
		return Duration::makeFromNs(val.count());
	}

	// Réinitialise le chrono
	void reset() {
		_startTime = TimePoint::TimePointClock::now();
	}

private:
	TimePoint::TimePointType _startTime;
};

#endif
