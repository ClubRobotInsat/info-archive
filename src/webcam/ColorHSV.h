// ColorHSV.h
// Couleur en HSV :
// 0.f <= H <= 360.f : teinte (hue) (== -1.f si undefined)
// 0.f <= S <= 100.f : saturation
// 0.f <= V <= 100.f : valeur (value)
// Source : http://fr.wikipedia.org/wiki/Teinte_Saturation_Valeur

#ifndef COLOR_HSV_H
#define COLOR_HSV_H

#include "Color.h"

#include <iostream>
using namespace std;

struct ColorHSV {
	float h, s, v;

	ColorHSV() : h(0.0f), s(0.0f), v(0.0f) {}
	ColorHSV(float h, float s, float v) {
		this->h = h;
		this->s = s;
		this->v = v;
	}
	ColorHSV(const ColorHSV& ref) {
		*this = ref;
	}
	ColorHSV(const Color& ref) {
		*this = ref;
	}

	ColorHSV& operator=(const ColorHSV& ref) {
		h = ref.h;
		s = ref.s;
		v = ref.v;
		return *this;
	}

	// Conversion RGB -> HSV
	ColorHSV& operator=(const Color& ref) {
		unsigned char max_rg = (ref.r > ref.g ? ref.r : ref.g);
		unsigned char max_rgb = (max_rg > ref.b ? max_rg : ref.b);

		unsigned char min_rg = (ref.r < ref.g ? ref.r : ref.g);
		unsigned char min_rgb = (min_rg < ref.b ? min_rg : ref.b);

		float fmax_rgb = float(max_rgb) / 255.f;
		float fmin_rgb = float(min_rgb) / 255.f;
		float fr = float(ref.r) / 255.f;
		float fg = float(ref.g) / 255.f;
		float fb = float(ref.b) / 255.f;

		// Teinte H :
		h = -1.f; // undefined

		if(max_rgb != min_rgb) {
			if(max_rgb == ref.r) {
				h = 60.f * (fg - fb) / (fmax_rgb - fmin_rgb);
				if(ref.g < ref.b)
					h += 360.f;
			} else if(max_rgb == ref.g)
				h = (60.f * (fb - fr) / (fmax_rgb - fmin_rgb)) + 120.f;
			else if(max_rgb == ref.b)
				h = (60.f * (fr - fg) / (fmax_rgb - fmin_rgb)) + 240.f;

			// On repasse à [0, 360] (ne devrait pas avoir lieu... et pourtant ^^)
			while(h > 360.0f)
				h -= 360.0f;

			while(h < 0.0f)
				h += 360.0f;
		}

		// Saturation S :
		if(max_rgb == 0x00)
			s = 0.f;
		else
			s = 100.f * (1.f - fmin_rgb / fmax_rgb);

		// Valeur V :
		v = 100.f * fmax_rgb;
		return *this;
	}

	bool operator==(const ColorHSV& ref) {
		return h == ref.h && s == ref.s && v == ref.v;
	}
};

#endif // COLOR_HSV_H
