//
// Created by scriptopathe on 05/03/16.
//

#ifndef ROOT_PHYSICALTOSIMULATIONUNITS_H
#define ROOT_PHYSICALTOSIMULATIONUNITS_H

#include <Box2D/Common/b2Math.h>
#include <MathToolbox/MathToolbox.h>
#include <Units.h>

const Length unitarySize = 1_dm;

inline double toBox2D(Distance const& d) {
	return d.toDm<double>();
}

inline double toBox2D(Angle const& d) {
	return d.toRad<double>();
}

inline double toBox2D(Mass const& m) {
	return m.toKg<double>();
}

inline Distance fromBox2D(double d) {
	return Distance::makeFromDm(d);
}

inline Speed fromBox2DVL(double d) {
	return Speed::makeFromDm_s(d);
}

inline double toBox2D(Speed const& d) {
	return d.toDm_s<double>();
}

inline AngularSpeed fromBox2DVA(double d) {
	return AngularSpeed::makeFromRad_s(d);
}

inline double toBox2D(AngularSpeed const& d) {
	return d.toRad_s<double>();
}

inline b2Vec2 toBox2D(Vector2m const& d) {
	return b2Vec2(toBox2D(d.x), toBox2D(d.y));
}

inline Vector2m fromBox2D(b2Vec2 const& d) {
	return Vector2m(fromBox2D(d.x), fromBox2D(d.y));
}

inline Vector3m fromBox2D(Vector3f const& d) {
	return Vector3m(fromBox2D(d.x), fromBox2D(d.y), fromBox2D(d.z));
}

inline Vector3f toBox2D(Vector3m const& d) {
	return Vector3f(toBox2D(d.x), toBox2D(d.y), toBox2D(d.z));
}
#endif // ROOT_PHYSICALTOSIMULATIONUNITS_	H
