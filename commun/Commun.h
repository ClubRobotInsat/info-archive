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

namespace std {
	// Pallie à l'absence de std::make_unique dans C++11, à venir dans C++14 cependant.
	// Source : http://stackoverflow.com/a/13512344/1276072

	template <typename T, typename... Args>
	unique_ptr<T> make_unique_helper(false_type, Args&&... args) {
		return unique_ptr<T>(new T(forward<Args>(args)...));
	}

	template <typename T, typename... Args>
	unique_ptr<T> make_unique_helper(true_type, Args&&... args) {
		static_assert(extent<T>::value == 0, "make_unique<T[N]>() is forbidden, please use make_unique<T[]>().");

		return unique_ptr<T>(new typename remove_extent<T>::type[sizeof...(Args)]{forward<Args>(args)...});
	}

	template <typename T, typename... Args>
	unique_ptr<T> make_unique(Args&&... args) {
		return make_unique_helper<T>(is_array<T>(), forward<Args>(args)...);
	}
}

inline void setThreadName(char const *name) {
#if __LINUX__
	pthread_setname_np(pthread_self(), name);
#elif __APPLE__
	pthread_setname_np(name);
#endif
}

#endif
