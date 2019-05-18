#ifndef ROOT_IROBOTCONTROLLER_H
#define ROOT_IROBOTCONTROLLER_H

#include "Enum/Enum.h"
#include "MathToolbox/MathToolbox.h"
#include "MathToolbox/Repere.h"
#include "Units.h"

ENUM_CLASS(SimuRobotState, Moving, Blocked, Idle, Stopped);

/** Un IRobotController commande l'entité physique correspondant au
 * robot. */
class IRobotController {
public:
	virtual ~IRobotController() = default;

	virtual void forward(Distance distance) = 0;

	/// Tourne de l'angle demandé, par rapport à l'angle actuel.
	virtual void turnRelative(Angle angle) = 0;

	virtual void stop() = 0;

	virtual void setCoordinates(const repere::Coordinates& coords) = 0;

	virtual repere::Coordinates getCoordinates() = 0;

	virtual SimuRobotState getState() = 0;

	virtual void update(Duration elapsed) = 0;
};

#endif // ROOT_IROBOTCONTROLLER_H
