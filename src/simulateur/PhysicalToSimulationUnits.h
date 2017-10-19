//
// Created by scriptopathe on 05/03/16.
//

#ifndef ROOT_PHYSICALTOSIMULATIONUNITS_H
#define ROOT_PHYSICALTOSIMULATIONUNITS_H

#include <Box2D/Common/b2Math.h>
#include <MathToolbox/MathToolbox.h>
#include <Units.h>

const Length unitarySize = 1_dm;

inline double toSimulation(Distance const& d) {
	return d.toDm<double>();
}

inline double toSimulation(Angle const& d) {
	return d.toRad<double>();
}

inline double toSimulation(Mass const& m) {
	return m.toKg<double>();
}

inline Distance fromSimulation(double d) {
	return Distance::makeFromDm(d);
}

inline Speed fromSimulationVL(double d) {
	return Speed::makeFromDm_s(d);
}

inline double toSimulation(Speed const& d) {
	return d.toDm_s<double>();
}

inline AngularSpeed fromSimulationVA(double d) {
	return AngularSpeed::makeFromRad_s(d);
}

inline double toSimulation(AngularSpeed const& d) {
	return d.toRad_s<double>();
}

/*
inline b2Vec2 toSimulation(Vector2m const& d) {
    double x,y;
    x = toSimulation(Vector2m.x);
    y = toSimulation(Vector2m.y);
    return b2Vec2(x, y);
}
*/

inline b2Vec2 toSimulation(Vector2m const& d) {
	return b2Vec2(toSimulation(d.x), toSimulation(d.y));
}

inline Vector2m fromSimulation(b2Vec2 const& d) {
	return Vector2m(fromSimulation(d.x), fromSimulation(d.y));
}

inline Vector3m fromSimulation(Vector3f const& d) {
	return Vector3m(fromSimulation(d.x), fromSimulation(d.y), fromSimulation(d.z));
}

inline Vector3f toSimulation(Vector3m const& d) {
	return Vector3f(toSimulation(d.x), toSimulation(d.y), toSimulation(d.z));
}
#endif // ROOT_PHYSICALTOSIMULATIONUNITS_	H
