//
// Created by scriptopathe on 13/03/16.
//

#ifndef ROOT_MODULEMOVE_H
#define ROOT_MODULEMOVE_H

#include <memory>

#include "../RobotLogic.h"
#include "../cartes/IDeplacement.h"

class ModuleMove : public IDeplacement {
public:
	ModuleMove(FormeRobot* obj);
	void setPhysicalObject(FormeRobot* obj);
	// Implementation de IDeplacement
	virtual Vector2m getPosition() override;
	virtual Speed getLinearVelocity() const override;
	virtual Angle getAngle() override;
	virtual AngularSpeed getAngularVelocity() override;
	virtual void setLinearVelocity(Speed) override;
	virtual void setAngularVelocity(AngularSpeed) override;

private:
	FormeRobot* _obj;
};


#endif // ROOT_MODULEMOVE_H
