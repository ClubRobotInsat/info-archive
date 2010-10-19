// ColorRGB.h

#ifndef COLOR_RGB_H
#define COLOR_RGB_H

#include <iostream>

struct ColorRGB
{
	unsigned char r, g, b;

	ColorRGB() : r(0x00), g(0x00), b(0x00) {}
	ColorRGB(unsigned char r, unsigned char g, unsigned char b) {this->r = r; this->g = g; this->b = b;}
	ColorRGB(const ColorRGB& ref) {*this = ref;}

	ColorRGB& operator=(const ColorRGB& ref)
	{
		r = ref.r;
		g = ref.g;
		b = ref.b;
		return *this;
	}

	bool operator==(const ColorRGB& ref)
	{
		return r==ref.r && g==ref.g && b==ref.b;
	}
};

// Affichage avec iostream
inline std::ostream& operator<<(std::ostream& os, const ColorRGB& c)
{
	os << std::hex <<  "[0x" << int(c.r) << ", 0x" << int(c.g) << ", 0x" << int(c.b) << "]" << std::dec;
	return os;
}

#endif // COLOR_RGB_H
