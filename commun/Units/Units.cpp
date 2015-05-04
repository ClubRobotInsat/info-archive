//
//  Units.cpp
//  Club Robot
//
//  Created by Rémi on 04/05/2015.
//

#include "Units.h"
#include <iostream>

std::ostream &operator<<(std::ostream &s, Distance const &d) {
	if(abs(d._val) >= 1)
		s << d._val << " m";
	else if(abs(d._val) >= 1e-2)
		s << d._val * 1e2 << " cm";
	else
		s << d._val * 1e3 << " mm";

	return s;
}

std::ostream &operator<<(std::ostream &s, VitesseLineaire const &v) {
	if(abs(v._val) >= 1)
		s << v._val << " m/s";
	else if(abs(v._val) >= 1e-2)
		s << v._val * 1e2 << " cm/s";
	else
		s << v._val * 1e3 << " mm/s";

	return s;
}

std::ostream &operator<<(std::ostream &s, VitesseAngulaire const &v) {
	return s << v._val << " s⁻¹";
}

std::ostream &operator<<(std::ostream &stream, Surface const &s) {
	return stream << s._val << " m²";
}

std::ostream &operator<<(std::ostream &stream, Masse const &m) {
	return stream << m._val << " kg";
}

std::ostream &operator<<(std::ostream &s, Duree const &d) {
	if(abs(d._val) >= 3600)
		s << d._val / 3600 << " h";
	else if(abs(d._val) >= 60)
		s << d._val / 60 << " min";
	else if(abs(d._val) >= 1)
		s << d._val << " s";
	else if(abs(d._val) >= 1e-3)
		s << d._val * 1e3 << " ms";
	else if(abs(d._val) >= 1e-6)
		s << d._val * 1e6 << " us";
	else
		s << d._val * 1e9 << " ns";

	return s;
}

std::ostream &operator<<(std::ostream &s, Angle const &v) {
	return s << v._val;
}
