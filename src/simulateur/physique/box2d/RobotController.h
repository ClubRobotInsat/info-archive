
#ifndef ROOT_ROBOTCONTROLLER_H
#define ROOT_ROBOTCONTROLLER_H

#include "../IRobotController.h"

#include "MathToolbox/Repere.h"
#include "PhysicalObject.h"
#include "pid/Asservissement.h"

class RobotController : public IRobotController {
public:
	explicit RobotController(PhysicalObject& robot);

	void reset();

	void forward(Distance distance) override;

	void turn(Angle angle) override;

	void stop() override;

	void emergencyStop();

	void setFrame(const Vector2m& position, Angle angle) override;

	SimuRobotState getState() override;

	/** Methode appelée à chaque tour de boucle pour mettre à jour
	 * le PID du robot et executer la commande en cours. */
	void update(Duration elapsed) override;

private:
	PhysicalObject& _robot;

	Asservissement _asserv;
	Distance _distanceGoal;
	Distance _currentDistance;
	Angle _angleGoal;
	Angle _currentAngle;

	SimuRobotState _currentState;
	int _blockedCounter;

	Vector2m _prevPosition;
	Angle _prevAngle;
};


#endif // ROOT_ROBOTCONTROLLER_H
