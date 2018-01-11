//
// Created by paul on 10/04/16.
//

#ifndef ROOT_SIMULATIONTOWEBSERVER_H
#define ROOT_SIMULATIONTOWEBSERVER_H

#include <iomanip>
#include <sstream>

#include <Json.h>
#include <MathToolbox/MathToolbox.h>
#include <Units.h>


inline float toWebServer(const Length& l) {
	return (float)l.toDm();
}

inline Vector3f toWebServer(const Vector3m& vec) {
	return {toWebServer(vec.x), toWebServer(vec.y), toWebServer(vec.z)};
}

inline Vector3f toWebServer(const Vector3ang& vec) {
	return {(float)vec.x.toDeg(), (float)vec.y.toDeg(), (float)vec.z.toDeg()};
}

// TODO float conversion ?

#endif // ROOT_SIMULATIONTOWEBSERVER_H