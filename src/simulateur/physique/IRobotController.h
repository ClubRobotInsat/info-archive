#ifndef ROOT_IROBOTCONTROLLER_H
#define ROOT_IROBOTCONTROLLER_H

#include "Enum/Enum.h"
#include "MathToolbox/MathToolbox.h"
#include "Units.h"

ENUM_CLASS(SimuRobotState, Moving, Blocked, Idle, Stopped);

/** Un IRobotController commande l'entité physique correspondant au
 * robot. */
class IRobotController {
public:
	virtual ~IRobotController() = default;

	virtual void forward(Distance distance) = 0;

	virtual void turn(Angle angle) = 0;

	virtual void stop() = 0;

	virtual void setFrame(const Vector2m& position, Angle angle) = 0;

	virtual SimuRobotState getState() = 0;

	virtual void update(Duration elapsed) = 0;
};

#endif // ROOT_IROBOTCONTROLLER_H
