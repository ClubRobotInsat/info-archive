#ifndef _COMMUN_H_
#define _COMMUN_H_

#include "Clock.h"
#include "Enum/Enum.h"
#include "MathToolbox/MathToolbox.h"
#include "ResultatAction.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wint-in-bool-context"
#include "communication/Trame.h"
#pragma GCC diagnostic pop

#include "Units.h"
#include "Utils.h"
#include <cstdint>
#include <functional>
#include <thread>

using namespace std::string_literals;

inline void setThreadName(char const* name) {
#if __linux__
	pthread_setname_np(pthread_self(), name);
#elif __APPLE__
	pthread_setname_np(name);
#endif
}

inline void setThreadName(std::string const& name) {
	setThreadName(name.c_str());
}

using std::uint8_t;
using std::int8_t;
using std::uint16_t;
using std::int16_t;
using std::uint32_t;
using std::int32_t;
using std::uint64_t;
using std::int64_t;

// écriture allégée pour définir une sous-classe d'exception.
#define EXCEPTION_CLASS(name)                                     \
	class name : public std::runtime_error {                      \
	public:                                                       \
		name() : std::runtime_error("") {}                        \
		name(std::string s) : std::runtime_error(std::move(s)) {} \
	};

#define logRouge(...) logError(__VA_ARGS__)
#define logVert(...) logDebug0(__VA_ARGS__)
#define logBleu(...) logDebug1(__VA_ARGS__)
#define logCyan(...) logDebug3(__VA_ARGS__)
#define logMagenta(...) logDebug2(__VA_ARGS__)
#define logJaune(...) logWarn(__VA_ARGS__)
#define logNoir(...) logDebug4(__VA_ARGS__)


#endif
