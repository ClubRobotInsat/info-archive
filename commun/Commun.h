#ifndef _COMMUN_H_
#define _COMMUN_H_

// sleep() et msleep() : routines d'attente en secondes et en millisecondes
#ifdef WIN32

#include <windows.h>

#define sleep(t) Sleep((t)*1000)
#define msleep(t) Sleep(t)

#else // defined(WIN32)

#include <unistd.h>
#define msleep(t) usleep((t)*1000)

#endif // defined(WIN32)

#include <memory>
#include <type_traits>
#include <thread>

inline void setThreadName(char const *name) {
#if __LINUX__
	pthread_setname_np(pthread_self(), name);
#elif __APPLE__
	pthread_setname_np(name);
#endif
}

#endif
