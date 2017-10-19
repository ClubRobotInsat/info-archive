//
// Created by benjamin on 17/04/16.
//

#ifndef ROOT_MODULESERVOS_H
#define ROOT_MODULESERVOS_H
#include "../../cartes/IServosIntelligents.h"
#include <log/Log.h>

class Robot2017;

class ModuleServos : public IServosIntelligents {
public:
	ModuleServos(Robot2017& robot2017);

	virtual void setAngle(uint8_t idCarte, uint8_t numServo, Angle angle) override;
	virtual Angle getAngle(uint8_t idCarte, uint8_t numServo) override;
	virtual bool isMovementCompleted(uint8_t idCarte, uint8_t numServo) override;
	virtual void setSpeed(uint8_t idCarte, uint8_t numServo, unsigned char speed) override;
	virtual bool isBlocked(uint8_t idCarte, uint8_t numServo) const override;

private:
	Robot2017& _robot;
};


#endif // ROOT_MODULESERVOS_H
