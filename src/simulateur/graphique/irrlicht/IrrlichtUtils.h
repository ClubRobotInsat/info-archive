
#ifndef ROOT_IRRLICHTUTILS_H
#define ROOT_IRRLICHTUTILS_H

#include <irrlicht/irrlicht.h>

template <typename T>
std::ostream& operator<<(std::ostream& s, const irr::core::vector3d<T>& vec) {
	s << "(" << vec.X << ", " << vec.Y << ", " << vec.Z << ")";
	return s;
}


#endif // ROOT_IRRLICHTUTILS_H
