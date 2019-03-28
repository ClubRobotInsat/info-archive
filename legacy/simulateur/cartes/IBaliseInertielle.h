// IBaliseInertielle.h

#ifndef IBALISE_INERTIELLE_H_
#define IBALISE_INERTIELLE_H_

#include "../PhysicalToBullet.h"

class IBaliseInertielle {
public:
	virtual ~IBaliseInertielle() {}

	virtual const Vector3m& getPositionAdv() const = 0;

	virtual Angle getAngle2dAdv() const = 0;

	virtual Speed getLinearVelocityAdv() const = 0;

	virtual AngularSpeed getAngularVelocityAdv() const = 0;
};

#endif /*IBALISE_INERTIELLE_H_*/
