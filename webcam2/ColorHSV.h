// ColorHSV.h
// Couleur en HSV :
// 0.f <= H <= 360.f : teinte (hue) (== -1.f si undefined)
// 0.f <= S <= 100.f : saturation
// 0.f <= V <= 100.f : valeur (value)
// Source : http://fr.wikipedia.org/wiki/Teinte_Saturation_Valeur

#ifndef COLOR_HSV_H
#define COLOR_HSV_H

#include "ColorRGB.h"

#include <iostream>
#include <algorithm>

struct ColorHSV
{
	short h, s, v;

	ColorHSV() : h(0), s(0), v(0) {}
	ColorHSV(short h, short s, short v) {this->h = h; this->s = s; this->v = v;}
	ColorHSV(const ColorHSV& ref) {*this = ref;}
	ColorHSV(const ColorRGB& ref) {*this = ref;}

	ColorHSV& operator=(const ColorHSV& ref)
	{
		h = ref.h;
		s = ref.s;
		v = ref.v;
		return *this;
	}

	// Conversion RGB -> HSV
	ColorHSV& operator=(const ColorRGB& ref)
	{
		unsigned char max_rgb = std::max(ref.r, std::max(ref.g, ref.b));
		unsigned char min_rgb = std::min(ref.r, std::min(ref.g, ref.b));

		float fmax_rgb = float(max_rgb) / 255.f;
		float fmin_rgb = float(min_rgb) / 255.f;
		float fr = float(ref.r) / 255.f;
		float fg = float(ref.g) / 255.f;
		float fb = float(ref.b) / 255.f;

		// Teinte H :
		float fh = -1.f; // undefined

		if(max_rgb != min_rgb)
		{
			if(max_rgb == ref.r)
			{
				fh = 60.f * (fg - fb) / (fmax_rgb - fmin_rgb);
				if(ref.g < ref.b)
					fh += 360.f;
			}
			else if(max_rgb == ref.g)
				fh = (60.f * (fb - fr) / (fmax_rgb - fmin_rgb)) + 120.f;
			else if(max_rgb == ref.b)
				fh = (60.f * (fr - fg) / (fmax_rgb - fmin_rgb)) + 240.f;

			// On repasse à [0, 360] (ne devrait pas avoir lieu... et pourtant ^^)
			while(fh > 360.0f)
				fh -= 360.0f;

			while (fh < 0.0f)
				fh += 360.0f;
		}

		// Saturation S :
		float fs = 0.f;
		if(max_rgb != 0x00)
			fs = 100.f * (1.f - fmin_rgb / fmax_rgb);

		// Valeur V :
		float fv = 100.f * fmax_rgb;

		// Conversion des flottants vers les shorts :
		this->h = short(fh + 0.5f);
		this->s = short(fs + 0.5f);
		this->v = short(fv + 0.5f);

		return *this;
	}

	bool operator==(const ColorHSV& ref)
	{
		return h==ref.h && s==ref.s && v==ref.v;
	}
};

#endif // COLOR_HSV_H
