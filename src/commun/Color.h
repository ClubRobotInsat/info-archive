#ifndef ROOT_COLOR3F_H
#define ROOT_COLOR3F_H

#include <iomanip>
#include <sstream>

#include "MathToolbox/MathToolbox.h"

struct Color3b : public Vector3ub {
	Color3b() : Vector3ub(0x00, 0x00, 0x00) {}
	Color3b(unsigned char r, unsigned char g, unsigned char b) : Vector3ub(r, g, b) {}
};

struct Color3f : public Vector3f {
	Color3f() : Vector3f(0, 0, 0) {}
	Color3f(float r, float g, float b) : Vector3f(r, g, b) {}

	/** Transforme une couleur représentée sur des floats (entre 0 et 1)
	 * en couleur hexadecimale. */
	std::string toHexStr() {
		unsigned char r = static_cast<unsigned char>((this->r * 255.99));
		unsigned char g = static_cast<unsigned char>((this->g * 255.99));
		unsigned char b = static_cast<unsigned char>((this->b * 255.99));

		std::ostringstream stream;
		stream << "0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(r) << std::setw(2)
		       << std::setfill('0') << static_cast<int>(g) << std::setw(2) << std::setfill('0') << static_cast<int>(b);
		return stream.str();
	}
};

#endif // ROOT_COLOR3F_H
