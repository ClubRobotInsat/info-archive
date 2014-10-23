#ifndef _CLOCK_H
#define _CLOCK_H

#include <chrono>
#include <iostream>

#ifndef _WIN32
// Cet en-tête définit une fonction sleep. Pour ne pas foirer la compilation en substituant la macro ci-après, on inclut le header d'abord.
#include <unistd.h>
#endif

#define sleep(delay) std::this_thread::sleep_for(delay)

/* 
 * On définit ci-dessous les suffixes _ns, _us, _ms et _s pour créer des nano, micro, milli secondes et secondes.
 * On peut aussi utiliser les suffixes standards ns, us, ms et s, mais ceux-ci ne sont pas en harmonie avec les _mm, _deg etc.
 */

inline constexpr auto operator"" _ns(long double duration) {
	return std::chrono::duration<long long, std::nano>(static_cast<long long>(duration));
}

inline constexpr auto operator"" _ns(unsigned long long duration) {
	return std::chrono::duration<long long, std::nano>(static_cast<long long>(duration));
}

inline constexpr auto operator"" _us(long double duration) {
	return std::chrono::duration<long long, std::nano>(static_cast<long long>(duration) * 1000);
}

inline constexpr auto operator"" _us(unsigned long long duration) {
	return std::chrono::duration<long long, std::nano>(static_cast<long long>(duration) * 1000);
}

inline constexpr auto operator"" _ms(long double duration) {
	return std::chrono::duration<long long, std::nano>(static_cast<long long>(duration) * 1000000);
}

inline constexpr auto operator"" _ms(unsigned long long duration) {
	return std::chrono::duration<long long, std::nano>(static_cast<long long>(duration) * 1000000);
}

inline constexpr auto operator"" _s(long double duration) {
	return std::chrono::duration<long long, std::nano>(static_cast<long long>(duration) * 1000000000);
}

inline constexpr auto operator"" _s(unsigned long long duration) {
	return std::chrono::duration<long long, std::nano>(static_cast<long long>(duration) * 1000000000);
}

typedef std::chrono::duration<long long, std::nano> dureeS;
//typedef std::chrono::time_point<std::chrono::high_resolution_clock> date;

inline std::ostream &operator<<(std::ostream &s, dureeS const &d) {
	if(d >= 1_s)
		s << d.count() << "s";
	else if(d >= 1_ms)
		s << d.count() << "ms";
	else if(d >= 1_us)
		s << d.count() << "us";
	else
		s << d.count() << "ns";

	return s;
}

class Clock {
public:
	Clock() {
		this->reset();
	}

	/// Retourne le temps écoulé depuis la dernière réinitialisation
	dureeS getElapsedTime() const {
		return std::chrono::duration_cast<dureeS>(ClockType::now() - _startTime);
	}

	void reset() { /// Réinitialise le chrono
		_startTime = ClockType::now();
	}
	
private:
	using ClockType = std::chrono::high_resolution_clock;
	std::chrono::time_point<ClockType> _startTime;
};

#endif
