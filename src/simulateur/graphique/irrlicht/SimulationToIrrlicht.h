//
// Created by denis on 07/12/17.
//

#ifndef ROOT_SIMULATIONTOIRRLICHT_H
#define ROOT_SIMULATIONTOIRRLICHT_H
#include <MathToolbox/MathToolbox.h>
#include <Units.h>
#include <irrlicht/irrlicht.h>
#include <string>
#include <vector>


class SimulationToIrrlicht {

public:
	// une unite de irrlicht correspond a un decimetre

	static float ToIrrlicht(const Length& l) {
		return (float)l.toDm();
	};

	static irr::core::vector3df VectorIrr(const Vector3m& vect) {
		return irr::core::vector3df(ToIrrlicht(vect.x), ToIrrlicht(vect.z), ToIrrlicht(vect.y));
	};

	static irr::core::vector3df VectorIrr(const Vector3f& vect) {
		return irr::core::vector3df(vect.x, vect.y, vect.z);
	}
};


#endif // ROOT_SIMULATIONTOIRRLICHT_H
