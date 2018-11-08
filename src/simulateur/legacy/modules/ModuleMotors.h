//
// Created by terae on 5/3/18.
//

#ifndef ROOT_MODULEMOTORS_H
#define ROOT_MODULEMOTORS_H

#include "../cartes/IMoteurs.h"
#include <log/Log.h>

class Robot2018;

class ModuleMotors : public IMoteurs {
public:
	ModuleMotors(Robot2018& robot2018);

	virtual void setEnginePositionAngle(uint8_t idCarte, uint8_t num_motor, Angle angle) override;
	virtual void setEnginePositionTurn(uint8_t idCarte, uint8_t num_motor, std::size_t nb_turns) override;
	virtual bool isAtPosition(uint8_t idCarte, uint8_t num_motor) const override;
	virtual void setEngineOn(uint8_t idCarte, uint8_t num_motor, SensRotation) override;
	virtual void setEngineOff(uint8_t idCarte, uint8_t num_motor) override;
	virtual void setBrushlessOn(uint8_t idCarte, uint8_t num_motor) override;
	virtual void setBrushlessOff(uint8_t idCarte, uint8_t num_motor) override;

private:
	Robot2018& _robot;
};


#endif // ROOT_MODULEMOTORS_H
