/// getMSTime() : Retourne le temps en millisecondes

#include <time.h>
#ifdef WIN32
# include <windows.h>
#else
# include <sys/time.h>
# include <sys/timeb.h>
#endif

inline long getMSTime() {
#ifdef WIN32
	return GetTickCount();
#else
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
#endif
}
