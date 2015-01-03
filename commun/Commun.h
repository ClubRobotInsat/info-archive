#ifndef _COMMUN_H_
#define _COMMUN_H_

#include <memory>
#include <type_traits>
#include <thread>
#include <string>

#include "Clock.h"
#include "Enum.h"
#include "Trame.h"
#include "Utils.h"
#include "MathToolbox/MathToolbox.h"
#include "log/Log.h"
#include "sockets/Socket.h"
#include "tga/TGALoader.h"


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

#endif
