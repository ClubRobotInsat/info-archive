#ifndef _COMMUN_H_
#define _COMMUN_H_

// sleep() et msleep() : routines d'attente en secondes et en millisecondes
#ifdef WIN32
#include <windows.h>
#define sleep(t) Sleep((t)*1000)
#define msleep(t) Sleep(t)
#else
#include <unistd.h>
#define msleep(t) usleep((t)*1000)
#endif

#endif
