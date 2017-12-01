//
// Created by louis on 16/11/17.
//

#ifndef ROOT_IPHYSICALBEHAVIOR_H
#define ROOT_IPHYSICALBEHAVIOR_H

#include "../core/IBehavior.h"

class IPhysicalInstance : public IBehavior {
public:
	virtual ~IPhysicalInstance() = default;

	virtual int getId() const = 0;

	virtual void enableSimulation(bool enable) = 0;
	virtual Mass getMass() = 0;

	virtual Vector2m getPosition() = 0;
	virtual void setPosition(Vector2m position) = 0;

	virtual Angle getAngle() = 0;
	virtual void setAngle(Angle angle) = 0;

	virtual Speed getLinearVelocity() = 0;
	virtual void setLinearVelocity(Speed speed) = 0;

	virtual AngularSpeed getAngularVelocity() = 0;
	virtual void setAngularVelocity(AngularSpeed speed) = 0;
};

#endif // ROOT_IPHYSICALBEHAVIOR_H
