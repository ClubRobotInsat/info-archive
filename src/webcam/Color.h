// Color.h

#ifndef COLOR_H
#define COLOR_H

#include <iostream>

#define COLOR_BLACK Color(0x00, 0x00, 0x00)
#define COLOR_WHITE Color(0xFF, 0xFF, 0xFF)
#define COLOR_RED Color(0xFF, 0x00, 0x00)
#define COLOR_GREEN Color(0x00, 0xFF, 0x00)
#define COLOR_BLUE Color(0x00, 0x00, 0xFF)

struct Color {
	unsigned char r, g, b;

	Color() : r(0x00), g(0x00), b(0x00) {}
	Color(unsigned char r, unsigned char g, unsigned char b) {
		this->r = r;
		this->g = g;
		this->b = b;
	}
	Color(const Color& ref) {
		*this = ref;
	}

	Color& operator=(const Color& ref) {
		r = ref.r;
		g = ref.g;
		b = ref.b;
		return *this;
	}

	bool operator==(const Color& ref) {
		return r == ref.r && g == ref.g && b == ref.b;
	}
};

// Affichage avec iostream
inline std::ostream& operator<<(std::ostream& os, const Color& c) {
	os << std::hex << "[0x" << int(c.r) << ", 0x" << int(c.g) << ", 0x" << int(c.b) << "]" << std::dec;
	return os;
}

#endif // COLOR_H
