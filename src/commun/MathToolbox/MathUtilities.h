// MathUtilities.h

#ifndef MATH_UTILITIES_H
#define MATH_UTILITIES_H

#include <cmath>
#include <cstdint>

#ifndef M_PI
#define M_E 2.7182818284590452354
#define M_LOG2E 1.4426950408889634074
#define M_LOG10E 0.43429448190325182765
#define M_LN2 0.69314718055994530942
#define M_LN10 2.30258509299404568402
#define M_PI 3.14159265358979323846
#define M_PI_2 1.57079632679489661923
#define M_PI_4 0.78539816339744830962
#define M_1_PI 0.31830988618379067154
#define M_2_PI 0.63661977236758134308
#define M_2_SQRTPI 1.12837916709551257390
#define M_SQRT2 1.41421356237309504880
#define M_SQRT1_2 0.70710678118654752440
#endif

// EPSILON sert lors de la comparaison approchée entre des flottants
#define EPSILON 0.01

template <typename T>
T minNonNul(T a, T b) {
	return max(min(a, b), 0);
}

template <typename T>
bool estDansIntervalOuvert(T val, T bMin, T bMax) {
	return (val > bMin) && (val < bMax);
}

uint64_t doubleToFixedPoint(double value, uint32_t pos) {
	return uint64_t(round(value * (1 << pos)));
}

#endif
