// main.cpp

#include <iostream>
#include <exception>

#include "MathToolbox.h"

int main() {
	Vector3d v1(4, 2, 6);
	Vector3d v2(1, 3, 5);
	Vector3d v3 = v1 + 2 * v2;

	// v3.tourner(M_PI);

	std::cout << v1 << std::endl;
	std::cout << v2 << std::endl;
	// cout << v3 << endl;
	// cout << det(v1, v2, v3) << endl;

	Vector4<double> v5;
	v5[0] = 1.0;
	v5[1] = 2.0;
	v5[2] = 3.0;
	v5[3] = 4.0;

	Vector4<double> v6;
	v6[0] = 5.0;
	v6[1] = 6.0;
	v6[2] = 7.0;
	v6[3] = 8.0;

	double d = v5 * v6;

	std::cout << v5 << std::endl;
	std::cout << v6 << std::endl;
	std::cout << d << std::endl;
	std::cout << v1 *v2 << std::endl;

	return 0;
}
