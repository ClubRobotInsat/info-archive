// main.cpp

#include <iostream>
#include <exception>

#include "MathToolbox.h"

int main()
{
	Vecteur3d v1(4, 2, 6);
	Vecteur3d v2(1, 3, 5);
	Vecteur3d v3 = v1 + 2*v2;

//	v3.tourner(M_PI);

	cout << v1 << endl;
	cout << v2 << endl;
	//cout << v3 << endl;
	//cout << det(v1, v2, v3) << endl;

	Vecteur<4, double> v5;
	v5[0] = 1.0; v5[1] = 2.0; v5[2] = 3.0; v5[3] = 4.0;

	Vecteur<4, double> v6;
	v6[0] = 5.0; v6[1] = 6.0; v6[2] = 7.0; v6[3] = 8.0;

	double d = v5 * v6;

	cout << v5 << endl;
	cout << v6 << endl;
	cout << d << endl;
	cout << v1 * v2 << endl;
	cout << Det(v1, v2, v3) << endl;

	return 0;
}
