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

class DateTemps {
public:
	using DateTempsClock = std::chrono::high_resolution_clock;
	using DateTempsType = std::chrono::time_point<DateTempsClock>;

	DateTemps() : _value(DateTempsClock::now()) {}
	DateTemps(DateTempsType d) : _value(d) {}
	DateTemps(DateTemps const &d) : _value(d.value()) {}

	DateTemps &operator=(DateTemps d) {
		swap(*this, d);
		return *this;
	}

	friend void swap(DateTemps &d1, DateTemps &d2) {
		using std::swap;
		swap(d1._value, d2._value);
	}

	static DateTemps now() {
		return DateTemps(DateTempsClock::now());
	}

	static DateTemps distantPast() {
		return DateTemps(DateTempsClock::now() - std::chrono::hours(24) * 365 * 100);
	}

	static DateTemps distantFuture() {
		return DateTemps(DateTempsClock::now() + std::chrono::hours(24) * 365 * 100);
	}

	DateTemps &operator-=(Duration const &d);
	DateTemps &operator+=(Duration const &d);

	DateTempsType value() const {
		return _value;
	}

private:
	DateTempsType _value;
};

inline bool operator<(DateTemps const &d1, DateTemps const &d2) {
	return d1.value() < d2.value();
}

inline bool operator==(DateTemps const &d1, DateTemps const &d2) {
	return d1.value() == d2.value();
}

inline bool operator>(DateTemps const &d1, DateTemps const &d2) {
	return d2 < d1;
}

inline bool operator!=(DateTemps const &d1, DateTemps const &d2) {
	return !(d1 == d2);
}

inline bool operator<=(DateTemps const &d1, DateTemps const &d2) {
	return !(d1 > d2);
}

inline bool operator>=(DateTemps const &d1, DateTemps const &d2) {
	return !(d1 < d2);
}

inline Duration operator-(DateTemps const &d1, DateTemps const &d2) {
	auto delta = std::chrono::duration_cast<std::chrono::nanoseconds>(d1.value() - d2.value());
	return Duration::makeFromNs(delta.count());
}

inline DateTemps operator-(DateTemps const &d1, Duration const &d2) {
	return DateTemps(d1.value() - d2.toSystemDelay());
}

inline DateTemps operator+(DateTemps const &d1, Duration const &d2) {
	return DateTemps(d1.value() + d2.toSystemDelay());
}

inline DateTemps &DateTemps::operator-=(Duration const &d) {
	*this = *this - d;
	return *this;
}

inline DateTemps &DateTemps::operator+=(Duration const &d) {
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
		auto val = std::chrono::duration_cast<std::chrono::nanoseconds>(DateTemps::DateTempsClock::now() - _startTime);
		return Duration::makeFromNs(val.count());
	}

	// Réinitialise le chrono
	void reset() {
		_startTime = DateTemps::DateTempsClock::now();
	}

private:
	DateTemps::DateTempsType _startTime;
};

#endif
