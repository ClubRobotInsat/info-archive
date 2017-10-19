// ColorPercent.h

#ifndef COLOR_PERCENT_H
#define COLOR_PERCENT_H

#define COLOR_PERCENT_BLACK ColorPercent(0.0f, 0.0f, 0.0f)
#define COLOR_PERCENT_WHITE ColorPercent(1.0f, 1.0f, 1.0f)
#define COLOR_PERCENT_RED ColorPercent(1.0f, 0.0f, 0.0f)
#define COLOR_PERCENT_GREEN ColorPercent(0.0f, 1.0f, 0.0f)
#define COLOR_PERCENT_BLUE ColorPercent(0.0f, 0.0f, 1.0f)

struct ColorPercent {
	float r, g, b;

	ColorPercent() : r(0.0f), g(0.0f), b(0.0f) {}
	ColorPercent(float r, float g, float b) {
		this->r = r;
		this->g = g;
		this->b = b;
	}
	ColorPercent(const ColorPercent& ref) {
		*this = ref;
	}

	ColorPercent& operator=(const ColorPercent& ref) {
		r = ref.r;
		g = ref.g;
		b = ref.b;
		return *this;
	}

	bool operator==(const ColorPercent& ref) {
		return r == ref.r && g == ref.g && b == ref.b;
	}
};

#endif // COLOR_PERCENT_H
