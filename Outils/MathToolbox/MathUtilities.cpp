// MathUtilities.cpp

#include "MathUtilities.h"

// Conversions d'angles degr�s <-> radians
double DegToRad(double angleDeg)
{
	return (M_PI*angleDeg) / 180.0;
}

double RadToDeg(double angleRad)
{
	return M_1_PI*(angleRad * 180.0);
}

double ToMoinsPiPi(double angle)
{
	float temp = fmod(angle, 2 * M_PI);

	if(temp >= M_PI)
		temp -= 2.0 * M_PI;
	if(temp < -M_PI)
		temp +=  2.0 * M_PI;
	
	return temp;
}
