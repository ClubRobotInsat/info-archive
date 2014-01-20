#ifndef _COMMUN_H_
#define _COMMUN_H_

// PrintError(), PrintWarning(), PrintStatus(), PrintDebug() : variantes de printf() prefixees
#include <cstdio>
#define PrintError(...) printf("[ERREUR] " __VA_ARGS__)
#define PrintWarning(...) printf("[ATTENTION] " __VA_ARGS__)
#define PrintStatus(...) printf("[STATUT] " __VA_ARGS__)
#define PrintDebug(...) printf("[DEBUG] " __VA_ARGS__)

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
