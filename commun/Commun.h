#ifndef _COMMUN_H_
#define _COMMUN_H_

#include <memory>
#include <type_traits>
#include <thread>
#include <string>

#include "Clock.h"
#include "Enum.h"

ENUM_CLASS(ResultatAction,
		   REUSSI, RATE, BLOQUE, BLOQUE_PAR_ADV, TIMEOUT
		   )

inline void setThreadName(char const *name) {
#if __LINUX__
	pthread_setname_np(pthread_self(), name);
#elif __APPLE__
	pthread_setname_np(name);
#endif
}

inline void setThreadName(std::string const &name) {
	setThreadName(name.c_str());
}

struct IDWrapper {
	explicit IDWrapper(std::uint32_t id) : _id(id) {}
	explicit IDWrapper(std::uint8_t id) : _id(id) {}

	std::uint32_t _id;
};

struct EnumID {
	constexpr EnumID(std::uint32_t enum_, std::uint8_t id_) : _enum(enum_), _id(id_) {}

	std::uint32_t _enum;
	std::uint8_t _id;
};

#endif
